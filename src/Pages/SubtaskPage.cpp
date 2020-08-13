/*
 * Copyright 2020 Yufan You
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SubtaskPage.hpp"

#include <QHeaderView>
#include <QTableWidget>
#include <QVBoxLayout>

#include "Models/ConvertedTestCaseModel.hpp"
#include "Models/SubtaskModel.hpp"
#include "TestCaseConvertPage.hpp"
#include "Widgets/ErrorLabel.hpp"

SubtaskPage::SubtaskPage(SubtaskModel *subtaskModel, ConvertedTestCaseModel *convertedTestCaseModel,
                         QWidget *parent)
    : QWizardPage(parent),
      m_subtaskModel(subtaskModel),
      m_convertedTestCaseModel(convertedTestCaseModel)
{
    Q_ASSERT(m_subtaskModel != nullptr);
    Q_ASSERT(m_convertedTestCaseModel != nullptr);

    setTitle("子任务设置");

    auto mainLayout = new QVBoxLayout(this);

    errorLabel = new ErrorLabel(this);
    mainLayout->addWidget(errorLabel);

    table = new QTableWidget(this);
    mainLayout->addWidget(table);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked |
                           QAbstractItemView::AnyKeyPressed);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"测试点范围", "分数", "依赖"});
    table->horizontalHeaderItem(1)->setToolTip(
        "子任务的总分。每个子任务的分数都需要是一个非负整数，总和需要是 100。");
    table->horizontalHeaderItem(2)->setToolTip(
        "一个子任务所依赖的子任务，是一个用逗号隔开的列表。");
}

void SubtaskPage::initializePage()
{
    m_subtaskModel->m_subtasks.clear();

    for (const auto &subtask : m_convertedTestCaseModel->testCases())
    {
        m_subtaskModel->m_subtasks.push_back({subtask.front().id, subtask.back().id, 0, {}});
    }

    disconnect(updateConnection);

    table->clearContents();
    table->setRowCount(m_subtaskModel->m_subtasks.count());

    for (int i = 0; i < m_subtaskModel->m_subtasks.count(); ++i)
    {
        auto rangeItem = new QTableWidgetItem(QString("%1-%2")
                                                  .arg(m_subtaskModel->m_subtasks[i].start)
                                                  .arg(m_subtaskModel->m_subtasks[i].end));
        rangeItem->setFlags(rangeItem->flags() & ~Qt::ItemIsEditable);
        table->setItem(i, 0, rangeItem);
        table->setItem(i, 1, new QTableWidgetItem("0"));
        table->setItem(i, 2, new QTableWidgetItem());
    }

    updateSubtasks();

    updateConnection =
        connect(table, &QTableWidget::itemChanged, this, &SubtaskPage::updateSubtasks);
}

void SubtaskPage::cleanupPage()
{
    m_subtaskModel->m_subtasks.clear();
}

bool SubtaskPage::isComplete() const
{
    return valid;
}

void SubtaskPage::updateSubtasks()
{
#define ERR(x)                \
    errorLabel->showError(x); \
    valid = false;            \
    emit completeChanged();   \
    return;

    errorLabel->hide();

    long long sum = 0;

    for (int i = 0; i < m_subtaskModel->m_subtasks.count(); ++i)
    {
        bool success = false;
        m_subtaskModel->m_subtasks[i].score = table->item(i, 1)->text().toInt(&success);
        if (!success)
        {
            ERR(QString("子任务 %1 的分数 \"%2\" 不是一个合法的数字")
                    .arg(i + 1)
                    .arg(table->item(i, 1)->text()));
        }
        if (m_subtaskModel->m_subtasks[i].score < 0)
        {
            ERR(QString("子任务 %1 的分数为负").arg(i + 1));
        }
        sum += m_subtaskModel->m_subtasks[i].score;

        m_subtaskModel->m_subtasks[i].dependency.clear();

        auto depends = table->item(i, 2)->text().split(',');

        for (auto depend : depends)
        {
            if (depend.trimmed().isEmpty())
                continue;

            int dependId = depend.toInt(&success);
            if (!success)
            {
                ERR(QString("子任务 %1 的依赖 [%2] 不是一个合法的数字").arg(i + 1).arg(depend));
            }
            if (dependId < 1 || dependId > i)
            {
                ERR(QString("子任务 %1 的依赖 [%2] 不在 1 到 %3 之间")
                        .arg(i + 1)
                        .arg(dependId)
                        .arg(i));
            }
            if (m_subtaskModel->m_subtasks[i].dependency.contains(dependId))
            {
                ERR(QString("子任务 %1 的依赖 [%2] 出现了超过一次").arg(i + 1).arg(dependId));
            }
            m_subtaskModel->m_subtasks[i].dependency.push_back(dependId);
        }

        for (auto x : m_subtaskModel->m_subtasks[i].dependency)
        {
            for (auto y : m_subtaskModel->m_subtasks[x - 1].dependency)
            {
                if (!m_subtaskModel->m_subtasks[i].dependency.contains(y))
                {
                    ERR(QString("子任务 %1 依赖 [%2] 而不依赖 [%3]，但 UOJ "
                                "的子任务依赖是不具有传递性的")
                            .arg(i + 1)
                            .arg(x)
                            .arg(y));
                }
            }
        }
    }

    if (sum != 100)
    {
        ERR(QString("子任务的分数之和为 %1 而不是 100").arg(sum));
    }

    valid = true;
    emit completeChanged();

#undef ERR
}

#include "SubtaskPage.hpp"

#include <QDebug>
#include <QHeaderView>
#include <QTableWidget>
#include <QVBoxLayout>

#include "ErrorLabel.hpp"
#include "TestCaseConvertPage.hpp"

SubtaskPage::SubtaskPage(TestCaseConvertPage *testCaseConvertPage, QWidget *parent)
    : QWizardPage(parent), convertPage(testCaseConvertPage)
{
    Q_ASSERT(convertPage != nullptr);

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
}

void SubtaskPage::initializePage()
{
    subtasks.clear();

    for (const auto &subtask : convertPage->getTestCases())
    {
        subtasks.push_back({subtask.front().id, subtask.back().id, 0, {}});
    }

    disconnect(updateConnection);

    table->clearContents();
    table->setRowCount(subtasks.count());

    for (int i = 0; i < subtasks.count(); ++i)
    {
        auto rangeItem =
            new QTableWidgetItem(QString("%1-%2").arg(subtasks[i].start).arg(subtasks[i].end));
        rangeItem->setFlags(rangeItem->flags() & ~Qt::ItemIsEditable);
        table->setItem(i, 0, rangeItem);
        table->setItem(i, 1, new QTableWidgetItem("0"));
        table->setItem(i, 2, new QTableWidgetItem());
    }

    updateSubtasks();

    updateConnection =
        connect(table, &QTableWidget::itemChanged, this, &SubtaskPage::updateSubtasks);
}

bool SubtaskPage::isComplete() const
{
    return valid;
}

QVector<SubtaskPage::Subtask> SubtaskPage::getSubtasks() const
{
    return subtasks;
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

    for (int i = 0; i < subtasks.count(); ++i)
    {
        bool success = false;
        subtasks[i].score = table->item(i, 1)->text().toInt(&success);
        if (!success)
        {
            ERR(QString("子任务 %1 的分数 \"%2\" 不是一个合法的数字")
                    .arg(i + 1)
                    .arg(table->item(i, 1)->text()));
        }
        if (subtasks[i].score < 0)
        {
            ERR(QString("子任务 %1 的分数为负").arg(i + 1));
        }
        sum += subtasks[i].score;

        subtasks[i].dependency.clear();

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
            if (subtasks[i].dependency.contains(dependId))
            {
                ERR(QString("子任务 %1 的依赖 [%2] 出现了超过一次").arg(i + 1).arg(dependId));
            }
            subtasks[i].dependency.push_back(dependId);
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

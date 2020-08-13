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

#include "TestCaseChoosePage.hpp"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "Models/OriginalTestCaseModel.hpp"
#include "Widgets/ListWidget.hpp"

using TestCaseChoose::TestCaseChooseLayout;

TestCaseChooseLayout::TestCaseChooseLayout(const QString &name, const QString &filter,
                                           TestCaseChoosePage *parent)
    : QVBoxLayout(), m_name(name), m_filter(filter)
{
    auto label = new QLabel(name, parent);
    addWidget(label);
    list = new ListWidget(parent);
    connect(list, &ListWidget::itemChanged, parent, &QWizardPage::completeChanged);
    addWidget(list);
    auto buttonLayout = new QHBoxLayout();
    addLayout(buttonLayout);
    auto addButton = new QPushButton("添加", parent);
    connect(addButton, &QPushButton::clicked, this, &TestCaseChooseLayout::addTestCase);
    buttonLayout->addWidget(addButton);
    deleteButton = new QPushButton("删除", parent);
    connect(deleteButton, &QPushButton::clicked, list, &ListWidget::deleteSelectedItems);
    connect(list, &QListWidget::itemSelectionChanged, this,
            &TestCaseChooseLayout::updateDeleteButton);
    buttonLayout->addWidget(deleteButton);
    auto sortButton = new QPushButton("排序", parent);
    connect(sortButton, &QPushButton::clicked, list, &ListWidget::naturalSort);
    buttonLayout->addWidget(sortButton);
    updateDeleteButton();
}

void TestCaseChooseLayout::addTestCase()
{
    const auto paths = QFileDialog::getOpenFileNames(
        parentWidget(), QString("添加%1文件").arg(m_name), QString(),
        QString("%1文件 (%2);;任何文件 (*)").arg(m_name).arg(m_filter));
    for (auto path : paths)
    {
        if (!list->itemLabels().contains(path))
            list->addPath(path);
    }
}

void TestCaseChooseLayout::updateDeleteButton()
{
    deleteButton->setDisabled(list->selectedItems().isEmpty());
}

TestCaseChoosePage::TestCaseChoosePage(OriginalTestCaseModel *originalTestCaseModel,
                                       QWidget *parent)
    : QWizardPage(parent), m_originalTestCaseModel(originalTestCaseModel)
{
    Q_ASSERT(m_originalTestCaseModel != nullptr);

    setTitle("选择数据文件");

    auto mainLayout = new QHBoxLayout(this);

    inputLayout = new TestCaseChooseLayout("输入", "*.in", this);
    mainLayout->addLayout(inputLayout);

    outputLayout = new TestCaseChooseLayout("输出", "*.out *.ans", this);
    mainLayout->addLayout(outputLayout);
}

bool TestCaseChoosePage::isComplete() const
{
    return inputLayout->list->count() && inputLayout->list->count() == outputLayout->list->count();
}

bool TestCaseChoosePage::validatePage()
{
    if (!isComplete())
        return false;

    m_originalTestCaseModel->m_inputs = inputLayout->list->itemLabels();
    m_originalTestCaseModel->m_outputs = outputLayout->list->itemLabels();
    return true;
}

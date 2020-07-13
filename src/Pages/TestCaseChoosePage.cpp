#include "TestCaseChoosePage.hpp"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

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
            list->addItem(path);
    }
}

void TestCaseChooseLayout::updateDeleteButton()
{
    deleteButton->setDisabled(list->selectedItems().isEmpty());
}

TestCaseChoosePage::TestCaseChoosePage(QWidget *parent) : QWizardPage(parent)
{
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

QStringList TestCaseChoosePage::inputs() const
{
    return inputLayout->list->itemLabels();
}

QStringList TestCaseChoosePage::outputs() const
{
    return outputLayout->list->itemLabels();
}

#include "TestCaseChoosePage.hpp"

#include <QCollator>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QMimeData>
#include <QPushButton>
#include <QVBoxLayout>
#include <algorithm>

using TestCaseChoose::ListWidget;
using TestCaseChoose::TestCaseChooseLayout;

ListWidget::ListWidget(QWidget *parent) : QListWidget(parent)
{
    setAcceptDrops(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void ListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->accept();
        event->acceptProposedAction();
    }
}

void ListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->accept();
        event->acceptProposedAction();
    }
}

void ListWidget::dropEvent(QDropEvent *event)
{
    const auto urls = event->mimeData()->urls();
    if (!urls.isEmpty())
    {
        for (auto url : urls)
        {
            const auto path = url.toLocalFile();
            if (QFileInfo(path).isFile() && !itemLabels().contains(path))
                addItem(url.toLocalFile());
        }
        event->acceptProposedAction();
    }
}

void ListWidget::addItem(const QString &title)
{
    QListWidget::addItem(title);
    emit itemChanged();
}

void ListWidget::deleteSelectedItems()
{
    for (auto item : selectedItems())
        delete item;
    emit itemChanged();
}

void ListWidget::naturalSort()
{
    QStringList labels;

    while (count())
    {
        auto item = takeItem(0);
        labels.push_back(item->text());
        delete item;
    }

    QCollator collator;
    collator.setNumericMode(true);
    std::sort(labels.begin(), labels.end(), [&collator](const QString &x, const QString &y) {
        return collator.compare(x, y) < 0;
    });

    addItems(labels);

    emit itemChanged();
}

QStringList ListWidget::itemLabels() const
{
    QStringList res;

    for (int i = 0; i < count(); ++i)
        res.push_back(item(i)->text());

    return res;
}

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

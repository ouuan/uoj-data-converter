#include "TestCaseChoosePage.hpp"

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

using TestCaseChoose::ListWidget;

ListWidget::ListWidget(QWidget *parent) : QListWidget(parent)
{
    setSortingEnabled(true);
    setAcceptDrops(true);
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

void ListWidget::deleteCurrentItem()
{
    auto item = currentItem();
    if (item)
    {
        delete item;
        emit itemChanged();
    }
}

QStringList ListWidget::itemLabels() const
{
    QStringList res;

    for (int i = 0; i < count(); ++i)
        res.push_back(item(i)->text());

    return res;
}

TestCaseChoosePage::TestCaseChoosePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("选择数据文件");

    auto mainLayout = new QHBoxLayout(this);

    auto inputLayout = new QVBoxLayout();
    mainLayout->addLayout(inputLayout);
    auto inputLabel = new QLabel("输入", this);
    inputLayout->addWidget(inputLabel);
    inputList = new ListWidget(this);
    connect(inputList, &ListWidget::itemChanged, this, &QWizardPage::completeChanged);
    inputLayout->addWidget(inputList);
    auto inputButtonLayout = new QHBoxLayout();
    inputLayout->addLayout(inputButtonLayout);
    auto addInputButton = new QPushButton("添加", this);
    connect(addInputButton, &QPushButton::clicked, this, &TestCaseChoosePage::addInput);
    inputButtonLayout->addWidget(addInputButton);
    auto deleteInputButton = new QPushButton("删除", this);
    connect(deleteInputButton, &QPushButton::clicked, inputList, &ListWidget::deleteCurrentItem);
    inputButtonLayout->addWidget(deleteInputButton);

    auto outputLayout = new QVBoxLayout();
    mainLayout->addLayout(outputLayout);
    auto outputLabel = new QLabel("输出", this);
    outputLayout->addWidget(outputLabel);
    outputList = new ListWidget(this);
    outputLayout->addWidget(outputList);
    auto outputButtonLayout = new QHBoxLayout();
    outputLayout->addLayout(outputButtonLayout);
    auto addOutputButton = new QPushButton("添加", this);
    connect(addOutputButton, &QPushButton::clicked, this, &TestCaseChoosePage::addOutput);
    outputButtonLayout->addWidget(addOutputButton);
    auto deleteOutputButton = new QPushButton("删除", this);
    connect(deleteOutputButton, &QPushButton::clicked, outputList, &ListWidget::deleteCurrentItem);
    outputButtonLayout->addWidget(deleteOutputButton);
}

bool TestCaseChoosePage::isComplete() const
{
    return inputList->count() && inputList->count() == outputList->count();
}

QStringList TestCaseChoosePage::inputs() const
{
    return inputList->itemLabels();
}

QStringList TestCaseChoosePage::outputs() const
{
    return outputList->itemLabels();
}

void TestCaseChoosePage::addInput()
{
    for (auto path : QFileDialog::getOpenFileNames(this, "添加输入文件"))
        inputList->addItem(path);
}

void TestCaseChoosePage::addOutput()
{
    for (auto path : QFileDialog::getOpenFileNames(this, "添加输出文件"))
        outputList->addItem(path);
}

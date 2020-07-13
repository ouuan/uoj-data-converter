#include "ExamplePage.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QSplitter>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "Widgets/AcceptsDropEdit.hpp"

ExamplePage::ExamplePage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("添加样例");

    auto mainLayout = new QHBoxLayout(this);

    auto splitter = new QSplitter(this);
    mainLayout->addWidget(splitter);

    auto leftWidget = new QWidget(this);
    splitter->addWidget(leftWidget);

    auto leftLayout = new QVBoxLayout(leftWidget);

    list = new QListWidget(leftWidget);
    connect(list, &QListWidget::itemActivated, this, &ExamplePage::switchToExample);
    connect(list, &QListWidget::itemClicked, this, &ExamplePage::switchToExample);
    leftLayout->addWidget(list);

    auto buttonLayout = new QHBoxLayout();
    leftLayout->addLayout(buttonLayout);

    auto addButton = new QPushButton("添加", leftWidget);
    connect(addButton, &QPushButton::clicked, this, &ExamplePage::addExample);
    buttonLayout->addWidget(addButton);

    deleteButton = new QPushButton("删除", leftWidget);
    connect(deleteButton, &QPushButton::clicked, this, &ExamplePage::deleteExample);
    buttonLayout->addWidget(deleteButton);

    stackedWidget = new QStackedWidget(this);
    connect(stackedWidget, &QStackedWidget::currentChanged, this, &ExamplePage::updateDeleteButton);
    splitter->addWidget(stackedWidget);

    splitter->setSizes({1000000, 3000000});
}

QVector<ExamplePage::Example> ExamplePage::getExamples() const
{
    QVector<Example> examples;
    for (int i = 0; i < list->count(); ++i)
        examples.push_back(exampleEditsForItem[list->item(i)]->getExample());
    return examples;
}

void ExamplePage::switchToExample(QListWidgetItem *item)
{
    if (item)
        stackedWidget->setCurrentWidget(exampleEditsForItem[item]);
}

void ExamplePage::addExample()
{
    auto item = new QListWidgetItem(QString::number(list->count() + 1), list);
    list->addItem(item);
    auto edits = new ExampleEdits(this);
    exampleEditsForItem[item] = edits;
    itemForExampleEdits[edits] = item;
    stackedWidget->addWidget(edits);
    switchToExample(item);
}

void ExamplePage::deleteExample()
{
    auto edits = qobject_cast<ExampleEdits *>(stackedWidget->currentWidget());
    auto item = itemForExampleEdits[edits];

    exampleEditsForItem.remove(item);
    itemForExampleEdits.remove(edits);
    stackedWidget->removeWidget(edits);

    delete edits;
    delete item;

    for (int i = 0; i < list->count(); ++i)
        list->item(i)->setText(QString::number(i + 1));

    switchToExample(list->currentItem());
}

void ExamplePage::updateDeleteButton()
{
    deleteButton->setDisabled(stackedWidget->count() == 0);
}

ExampleEdits::ExampleEdits(QWidget *parent) : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);

    auto inputLabel = new QLabel("输入", this);
    inputLabel->setToolTip("样例的输入，可以手动输入也可以将文件拖入");
    mainLayout->addWidget(inputLabel);

    inputEdit = new AcceptsDropEdit(this);
    mainLayout->addWidget(inputEdit);

    auto outputLabel = new QLabel("输出", this);
    inputLabel->setToolTip("样例的输出，可以手动输入也可以将文件拖入");
    mainLayout->addWidget(outputLabel);

    outputEdit = new AcceptsDropEdit(this);
    mainLayout->addWidget(outputEdit);
}

ExamplePage::Example ExampleEdits::getExample()
{
    return {inputEdit->toPlainText(), outputEdit->toPlainText()};
}

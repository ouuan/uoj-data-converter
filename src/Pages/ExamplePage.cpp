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

#include "ExamplePage.hpp"

#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QSplitter>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "Widgets/AcceptsDropEdit.hpp"

static const int MAX_DISPLAY_LENGTH = 10000;

ExamplePage::ExamplePage(ExampleModel *exampleModel, QWidget *parent)
    : QWizardPage(parent), m_exampleModel(exampleModel)
{
    Q_ASSERT(m_exampleModel != nullptr);

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

    updateDeleteButton();

    splitter->setSizes({1000000, 3000000});
}

bool ExamplePage::validatePage()
{
    m_exampleModel->m_examples.clear();
    for (int i = 0; i < list->count(); ++i)
        m_exampleModel->m_examples.push_back(exampleEditsForItem[list->item(i)]->getExample());
    return true;
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

ExampleEdit::ExampleEdit(const QString &name, const QString &filter, QWidget *parent)
    : QWidget(parent), m_name(name), m_filter(filter)
{
    auto mainLayout = new QVBoxLayout(this);

    auto upLayout = new QHBoxLayout();
    mainLayout->addLayout(upLayout);

    auto label = new QLabel(name, this);
    label->setToolTip(QString("样例的%1，可以手动输入也可以将文件拖入").arg(name));
    upLayout->addWidget(label);

    auto addButton = new QPushButton("选择文件", this);
    connect(addButton, &QPushButton::clicked, this, &ExampleEdit::chooseFile);
    upLayout->addWidget(addButton);

    auto clearButton = new QPushButton("清空", this);
    connect(clearButton, &QPushButton::clicked, this, &ExampleEdit::clearContent);
    upLayout->addWidget(clearButton);

    edit = new AcceptsDropEdit(MAX_DISPLAY_LENGTH, this);
    mainLayout->addWidget(edit);
}

QString ExampleEdit::getContent() const
{
    return edit->getContent();
}

void ExampleEdit::chooseFile()
{
    auto path = QFileDialog::getOpenFileName(
        this, QString("选择%1文件").arg(m_name), QString(),
        QString("%1文件 (%2);;任何文件 (*)").arg(m_name).arg(m_filter));
    if (!path.isEmpty())
    {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            edit->modifyText(file.readAll());
    }
}

void ExampleEdit::clearContent()
{
    auto res = QMessageBox::question(this, QString("清空%1").arg(m_name),
                                     QString("你确定要清空%1吗？").arg(m_name),
                                     QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::No)
        return;
    edit->modifyText(QString());
}

ExampleEdits::ExampleEdits(QWidget *parent) : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);

    inputEdit = new ExampleEdit("输入", "*.in", this);
    mainLayout->addWidget(inputEdit);

    outputEdit = new ExampleEdit("输出", "*.out *.ans", this);
    mainLayout->addWidget(outputEdit);
}

ExampleModel::Example ExampleEdits::getExample()
{
    return {inputEdit->getContent(), outputEdit->getContent()};
}

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

#pragma once

#include <QMap>
#include <QWizardPage>

#include "Models/ExampleModel.hpp"

class AcceptsDropEdit;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QStackedWidget;

class ExampleEdits;

class ExamplePage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit ExamplePage(ExampleModel *exampleModel, QWidget *parent = nullptr);

    bool validatePage() override;

   private slots:
    void switchToExample(QListWidgetItem *item);

    void addExample();

    void deleteExample();

    void updateDeleteButton();

   private:
    QListWidget *list = nullptr;
    QPushButton *deleteButton = nullptr;
    QStackedWidget *stackedWidget = nullptr;

    QMap<QListWidgetItem *, ExampleEdits *> exampleEditsForItem;
    QMap<ExampleEdits *, QListWidgetItem *> itemForExampleEdits;

    ExampleModel *m_exampleModel = nullptr;
};

class ExampleEdit : public QWidget
{
    Q_OBJECT

   public:
    explicit ExampleEdit(const QString &name, const QString &filter, QWidget *parent = nullptr);

    QString getContent() const;

   private slots:
    void chooseFile();

    void clearContent();

   private:
    AcceptsDropEdit *edit = nullptr;

    QString m_name;
    QString m_filter;
};

class ExampleEdits : public QWidget
{
    Q_OBJECT

   public:
    explicit ExampleEdits(QWidget *parent = nullptr);

    ExampleModel::Example getExample();

   private:
    ExampleEdit *inputEdit = nullptr;
    ExampleEdit *outputEdit = nullptr;
};

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

#include <QWizardPage>

class ErrorLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;

class ConvertedTestCaseModel;
class OriginalTestCaseModel;

class TestCaseConvertPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit TestCaseConvertPage(ConvertedTestCaseModel *convertedTestCaseModel,
                                 OriginalTestCaseModel *originalTestCaseModel,
                                 QWidget *parent = nullptr);

    void initializePage() override;

    bool isComplete() const override;

    int nextId() const override;

   private slots:
    void updateResult();

    void setSubtask();

    void updateSubtaskButton();

   private:
    QLineEdit *problemNameEdit = nullptr;
    QLineEdit *inputPatternEdit = nullptr;
    QLineEdit *outputPatternEdit = nullptr;
    ErrorLabel *errorLabel = nullptr;
    QTableWidget *table = nullptr;
    QPushButton *subtaskButton = nullptr;

    QVector<bool> subtaskBegins;  // whether a test case is the beginning of a subtask

    ConvertedTestCaseModel *m_convertedTestCaseModel = nullptr;
    OriginalTestCaseModel *m_originalTestCaseModel = nullptr;
};

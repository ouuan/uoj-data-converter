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
class QRadioButton;
class QLineEdit;

class ConvertedTestCaseModel;
class ExampleModel;
class ProblemConfModel;
class ResultModel;
class StdModel;

class CommitOperationPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit CommitOperationPage(ResultModel *resultModel,
                                 ConvertedTestCaseModel *convertedTestCaseModel,
                                 ExampleModel *exampleModel, ProblemConfModel *problemConfModel,
                                 StdModel *stdModel, QWidget *parent = nullptr);

    void initializePage() override;

    // It not only validates this page, but also commits the operations
    bool validatePage() override;

   private slots:
    void choosePath();

   private:
    QRadioButton *copyButton = nullptr;
    QRadioButton *moveButton = nullptr;
    QLineEdit *pathEdit = nullptr;
    ErrorLabel *errorLabel = nullptr;

    ResultModel *m_resultModel = nullptr;
    ConvertedTestCaseModel *m_convertedTestCaseModel = nullptr;
    ExampleModel *m_exampleModel = nullptr;
    ProblemConfModel *m_problemConfModel = nullptr;
    StdModel *m_stdModel = nullptr;
};

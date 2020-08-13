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

class QComboBox;
class QPlainTextEdit;
class QSpinBox;

class ConvertedTestCaseModel;
class ExampleModel;
class ProblemConfModel;
class SubtaskModel;

class ProblemConfPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit ProblemConfPage(ProblemConfModel *problemConfModel,
                             ConvertedTestCaseModel *convertedTestCaseModel,
                             SubtaskModel *subtaskModel, ExampleModel *exampleModel,
                             QWidget *parent = nullptr);

    void initializePage() override;

    bool validatePage() override;

   private slots:
    void updateProblemConf();

   private:
    QSpinBox *timeLimitSpinBox = nullptr;
    QSpinBox *memoryLimitSpinBox = nullptr;
    QComboBox *checkerComboBox = nullptr;
    QPlainTextEdit *problemConfEdit = nullptr;

    ProblemConfModel *m_problemConfModel = nullptr;
    ConvertedTestCaseModel *m_convertedTestCaseModel = nullptr;
    SubtaskModel *m_subtaskModel = nullptr;
    ExampleModel *m_exampleModel = nullptr;
};

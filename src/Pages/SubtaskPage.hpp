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
class QTableWidget;

class ConvertedTestCaseModel;
class SubtaskModel;

class SubtaskPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit SubtaskPage(SubtaskModel *subtaskModel, ConvertedTestCaseModel *convertedTestCaseModel,
                         QWidget *parent = nullptr);

    void initializePage() override;

    void cleanupPage() override;

    bool isComplete() const override;

   private slots:
    void updateSubtasks();

   private:
    ErrorLabel *errorLabel = nullptr;
    QTableWidget *table = nullptr;

    QMetaObject::Connection updateConnection;

    bool valid = false;

    SubtaskModel *m_subtaskModel = nullptr;
    ConvertedTestCaseModel *m_convertedTestCaseModel = nullptr;
};

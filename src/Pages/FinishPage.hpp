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

class QCheckBox;

class ResultModel;
class StdModel;

class FinishPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit FinishPage(ResultModel *resultModel, StdModel *stdModel, QWidget *parent = nullptr);

    void initializePage() override;

    bool validatePage() override;

   private:
    QCheckBox *openOutputCheckBox = nullptr;
    QCheckBox *copyStdCheckBox = nullptr;

    ResultModel *m_resultModel = nullptr;
    StdModel *m_stdModel = nullptr;
};

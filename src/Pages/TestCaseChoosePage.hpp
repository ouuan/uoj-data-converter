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

#include <QVBoxLayout>
#include <QWizardPage>

class ListWidget;
class QPushButton;

class OriginalTestCaseModel;

class TestCaseChoosePage;

namespace TestCaseChoose
{
class TestCaseChooseLayout : public QVBoxLayout
{
    Q_OBJECT

   public:
    explicit TestCaseChooseLayout(const QString &name, const QString &filter,
                                  TestCaseChoosePage *parent);

   private slots:
    void addTestCase();

    void updateDeleteButton();

   private:
    ListWidget *list = nullptr;
    QPushButton *deleteButton = nullptr;

    QString m_name;
    QString m_filter;

    friend class ::TestCaseChoosePage;
};
}  // namespace TestCaseChoose

class TestCaseChoosePage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit TestCaseChoosePage(OriginalTestCaseModel *testCaseModel, QWidget *parent = nullptr);

    bool isComplete() const override;

    bool validatePage() override;

   private:
    TestCaseChoose::TestCaseChooseLayout *inputLayout = nullptr;
    TestCaseChoose::TestCaseChooseLayout *outputLayout = nullptr;

    OriginalTestCaseModel *m_originalTestCaseModel = nullptr;
};

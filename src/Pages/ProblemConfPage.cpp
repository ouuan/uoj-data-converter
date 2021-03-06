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

#include "ProblemConfPage.hpp"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

#include "Models/ConvertedTestCaseModel.hpp"
#include "Models/ExampleModel.hpp"
#include "Models/ProblemConfModel.hpp"
#include "Models/SubtaskModel.hpp"

ProblemConfPage::ProblemConfPage(ProblemConfModel *problemConfModel,
                                 ConvertedTestCaseModel *convertedTestCaseModel,
                                 SubtaskModel *subtaskModel, ExampleModel *exampleModel,
                                 QWidget *parent)
    : QWizardPage(parent),
      m_problemConfModel(problemConfModel),
      m_convertedTestCaseModel(convertedTestCaseModel),
      m_subtaskModel(subtaskModel),
      m_exampleModel(exampleModel)
{
    Q_ASSERT(m_problemConfModel != nullptr);
    Q_ASSERT(m_convertedTestCaseModel != nullptr);
    Q_ASSERT(m_subtaskModel != nullptr);
    Q_ASSERT(m_exampleModel != nullptr);

    setTitle("生成 problem.conf");

    auto mainLayout = new QVBoxLayout(this);

    auto settingsLayout = new QHBoxLayout();
    mainLayout->addLayout(settingsLayout);

    auto timeLimitLabel = new QLabel("时间限制", this);
    settingsLayout->addWidget(timeLimitLabel);

    timeLimitSpinBox = new QSpinBox(this);
    timeLimitSpinBox->setRange(1, 10000);
    timeLimitSpinBox->setValue(1);
    settingsLayout->addWidget(timeLimitSpinBox);

    settingsLayout->addStretch();

    auto memoryLimitLabel = new QLabel("空间限制", this);
    settingsLayout->addWidget(memoryLimitLabel);

    memoryLimitSpinBox = new QSpinBox(this);
    memoryLimitSpinBox->setRange(1, 10000);
    memoryLimitSpinBox->setValue(512);
    settingsLayout->addWidget(memoryLimitSpinBox);

    settingsLayout->addStretch();

    auto checkerLabel = new QLabel("Checker", this);
    settingsLayout->addWidget(checkerLabel);

    checkerComboBox = new QComboBox(this);
    checkerComboBox->addItems({"ncmp", "rcmp4", "rcmp6", "rcmp9", "wcmp", "nyesno", "自定义"});
    checkerComboBox->setCurrentText("ncmp");
    settingsLayout->addWidget(checkerComboBox);

    settingsLayout->addStretch();

    auto updateButton = new QPushButton("更新", this);
    updateButton->setToolTip(
        "根据前几页的设置、时空限制以及 Checker 更新 "
        "problem.conf。你在下面的编辑框中做出的所有修改都会被覆盖。");
    connect(updateButton, &QPushButton::clicked, this, &ProblemConfPage::updateProblemConf);
    settingsLayout->addWidget(updateButton);

    problemConfEdit = new QPlainTextEdit(this);
    mainLayout->addWidget(problemConfEdit);
}

void ProblemConfPage::initializePage()
{
    updateProblemConf();
}

bool ProblemConfPage::validatePage()
{
    m_problemConfModel->m_problemConf = problemConfEdit->toPlainText();
    return true;
}

void ProblemConfPage::updateProblemConf()
{
    auto problemConf = QString(
                           "n_tests %1\n"
                           "n_ex_tests %2\n"
                           "n_sample_tests %2\n"
                           "input_pre %3\n"
                           "input_suf in\n"
                           "output_pre %3\n"
                           "output_suf ans\n"
                           "time_limit %4\n"
                           "memory_limit %5\n"
                           "output_limit 64\n"
                           "use_builtin_judger on\n")
                           .arg(m_convertedTestCaseModel->testCases().back().back().id)
                           .arg(m_exampleModel->examples().count())
                           .arg(m_convertedTestCaseModel->name())
                           .arg(timeLimitSpinBox->value())
                           .arg(memoryLimitSpinBox->value());

    if (checkerComboBox->currentText() != "自定义")
        problemConf += QString("use_builtin_checker %6\n").arg(checkerComboBox->currentText());

    auto subtasks = m_subtaskModel->subtasks();

    if (!subtasks.isEmpty())
    {
        problemConf += QString("n_subtasks %1\n").arg(subtasks.count());

        for (int i = 0; i < subtasks.count(); ++i)
        {
            problemConf += QString(
                               "subtask_end_%1 %2\n"
                               "subtask_score_%1 %3\n")
                               .arg(i + 1)
                               .arg(subtasks[i].end)
                               .arg(subtasks[i].score);

            if (!subtasks[i].dependency.isEmpty())
            {
                problemConf += QString("subtask_dependence_%1 many\n").arg(i + 1);
                for (int j = 0; j < subtasks[i].dependency.count(); ++j)
                    problemConf += QString("subtask_dependence_%1_%2 %3\n")
                                       .arg(i + 1)
                                       .arg(j + 1)
                                       .arg(subtasks[i].dependency[j]);
            }
        }
    }

    auto cursor = problemConfEdit->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.insertText(problemConf);
}

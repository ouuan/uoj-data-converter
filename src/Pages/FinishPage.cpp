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

#include "FinishPage.hpp"

#include <QCheckBox>
#include <QClipboard>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QUrl>
#include <QVBoxLayout>

#include "Models/ResultModel.hpp"
#include "Models/StdModel.hpp"

FinishPage::FinishPage(ResultModel *resultModel, StdModel *stdModel, QWidget *parent)
    : QWizardPage(parent), m_resultModel(resultModel), m_stdModel(stdModel)
{
    Q_ASSERT(m_resultModel != nullptr);
    Q_ASSERT(m_stdModel != nullptr);

    setTitle("所有操作均已完成");
    setFinalPage(true);

    auto mainLayout = new QVBoxLayout(this);

    openOutputCheckBox = new QCheckBox("在完成后打开输出文件夹");
    mainLayout->addWidget(openOutputCheckBox);

    copyStdCheckBox = new QCheckBox("复制 std 到剪贴板");
    mainLayout->addWidget(copyStdCheckBox);
}

void FinishPage::initializePage()
{
    if (m_stdModel->std().isNull())
    {
        copyStdCheckBox->setChecked(false);
        copyStdCheckBox->hide();
    }
    else
        copyStdCheckBox->show();
}

bool FinishPage::validatePage()
{
    if (copyStdCheckBox->isChecked())
        QGuiApplication::clipboard()->setText(m_stdModel->std());

    if (openOutputCheckBox->isChecked())
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_resultModel->outputPath()));

    return true;
}

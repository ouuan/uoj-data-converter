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

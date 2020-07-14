#include "FinishPage.hpp"

#include <QCheckBox>
#include <QClipboard>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QUrl>
#include <QVBoxLayout>

#include "CommitOperationPage.hpp"
#include "StdPage.hpp"

FinishPage::FinishPage(CommitOperationPage *commitOperationPage, StdPage *stdPage, QWidget *parent)
    : QWizardPage(parent), m_commitOperationPage(commitOperationPage), m_stdPage(stdPage)
{
    Q_ASSERT(commitOperationPage != nullptr);
    Q_ASSERT(stdPage != nullptr);

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
    if (m_stdPage->getStd().isNull())
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
        QGuiApplication::clipboard()->setText(m_stdPage->getStd());

    if (openOutputCheckBox->isChecked())
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_commitOperationPage->getOutputPath()));

    return true;
}

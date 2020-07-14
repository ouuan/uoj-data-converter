#include "FinishPage.hpp"

#include <QCheckBox>
#include <QDesktopServices>
#include <QUrl>
#include <QVBoxLayout>

#include "CommitOperationPage.hpp"

FinishPage::FinishPage(CommitOperationPage *commitOperationPage, QWidget *parent)
    : QWizardPage(parent), m_commitOperationPage(commitOperationPage)
{
    Q_ASSERT(commitOperationPage != nullptr);

    setTitle("所有操作均已完成");
    setFinalPage(true);

    auto mainLayout = new QVBoxLayout(this);

    openOutputCheckBox = new QCheckBox("在完成后打开输出文件夹");
    mainLayout->addWidget(openOutputCheckBox);
}

bool FinishPage::validatePage()
{
    if (openOutputCheckBox->isChecked())
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_commitOperationPage->getOutputPath()));

    return true;
}

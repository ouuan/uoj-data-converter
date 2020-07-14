#include "StdPage.hpp"

#include <QApplication>
#include <QCheckBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRegularExpression>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>

#include "Widgets/ErrorLabel.hpp"

static const QString freopenRegexStr(
    R"RAW(freopen\s*\(\s*".+"\s*,\s*".+"\s*,\s*(?:stdin|stdout)\s*\)\s*;)RAW");

StdPage::StdPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("选择 std");

    auto mainLayout = new QVBoxLayout(this);

    auto pathLayout = new QHBoxLayout();
    mainLayout->addLayout(pathLayout);

    auto pathLabel = new QLabel("std 路径", this);
    pathLayout->addWidget(pathLabel);

    pathEdit = new QLineEdit(this);
    connect(pathEdit, &QLineEdit::textChanged, this, &StdPage::updateStd);
    pathLayout->addWidget(pathEdit);

    auto pathButton = new QToolButton(this);
    pathButton->setToolTip("选择文件");
    pathButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton));
    connect(pathButton, &QToolButton::clicked, this, &StdPage::chooseStd);
    pathLayout->addWidget(pathButton);

    errorLabel = new ErrorLabel(this);
    mainLayout->addWidget(errorLabel);

    removeFreopenCheckBox = new QCheckBox("删除代码中的 freopen", this);
    removeFreopenCheckBox->setToolTip(
        QString("更准确地说，是将 /%1/ 替换为空").arg(freopenRegexStr));
    removeFreopenCheckBox->setChecked(true);
    mainLayout->addWidget(removeFreopenCheckBox);
}

QString StdPage::getStd() const
{
    return stdContent;
}

bool StdPage::isComplete() const
{
    if (pathEdit->text().trimmed().isEmpty() || QFileInfo(pathEdit->text()).isReadable())
    {
        errorLabel->hide();
        return true;
    }
    else
    {
        errorLabel->showError(QString("[%1] 不是可读的文件").arg(pathEdit->text()));
        return false;
    }
}

void StdPage::chooseStd()
{
    const auto path =
        QFileDialog::getOpenFileName(this, "选择 std", QString(), "C++ 源文件 (*.cpp *.cc *.cxx)");
    if (!path.isEmpty())
        pathEdit->setText(path);
}

void StdPage::updateStd()
{
    if (!QFile::exists(pathEdit->text()))
        stdContent.clear();
    else
    {
        QFile stdFile(pathEdit->text());
        if (stdFile.open(QIODevice::ReadOnly | QIODevice::Text))
            stdContent = stdFile.readAll();
        if (removeFreopenCheckBox->isChecked())
            stdContent.replace(QRegularExpression(freopenRegexStr), "");
    }
    emit completeChanged();
}

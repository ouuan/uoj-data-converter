#pragma once

#include <QWizardPage>

class CommitOperationPage;
class QCheckBox;

class FinishPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit FinishPage(CommitOperationPage *commitOperationPage, QWidget *parent = nullptr);

    bool validatePage() override;

   private:
    CommitOperationPage *m_commitOperationPage = nullptr;
    QCheckBox *openOutputCheckBox = nullptr;
};

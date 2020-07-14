#pragma once

#include <QWizardPage>

class CommitOperationPage;
class QCheckBox;
class StdPage;

class FinishPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit FinishPage(CommitOperationPage *commitOperationPage, StdPage *stdPage,
                        QWidget *parent = nullptr);

    void initializePage() override;

    bool validatePage() override;

   private:
    CommitOperationPage *m_commitOperationPage = nullptr;
    StdPage *m_stdPage = nullptr;
    QCheckBox *openOutputCheckBox = nullptr;
    QCheckBox *copyStdCheckBox = nullptr;
};

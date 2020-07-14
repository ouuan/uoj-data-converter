#pragma once

#include <QWizardPage>

class ErrorLabel;
class ProblemConfPage;
class QRadioButton;
class QLineEdit;
class StdPage;

class CommitOperationPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit CommitOperationPage(ProblemConfPage *problemConfPage, StdPage *stdPage,
                                 QWidget *parent = nullptr);

    void initializePage() override;

    // It not only validates this page, but also commits the operations
    bool validatePage() override;

    QString getOutputPath() const;

   private slots:
    void choosePath();

   private:
    ProblemConfPage *m_problemConfPage = nullptr;
    StdPage *m_stdPage = nullptr;
    QRadioButton *copyButton = nullptr;
    QRadioButton *moveButton = nullptr;
    QLineEdit *pathEdit = nullptr;
    ErrorLabel *errorLabel = nullptr;
};

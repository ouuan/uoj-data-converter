#pragma once

#include <QWizardPage>

class ErrorLabel;
class QRadioButton;
class QLineEdit;

class ConvertedTestCaseModel;
class ExampleModel;
class ProblemConfModel;
class ResultModel;
class StdModel;

class CommitOperationPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit CommitOperationPage(ResultModel *resultModel,
                                 ConvertedTestCaseModel *convertedTestCaseModel,
                                 ExampleModel *exampleModel, ProblemConfModel *problemConfModel,
                                 StdModel *stdModel, QWidget *parent = nullptr);

    void initializePage() override;

    // It not only validates this page, but also commits the operations
    bool validatePage() override;

   private slots:
    void choosePath();

   private:
    QRadioButton *copyButton = nullptr;
    QRadioButton *moveButton = nullptr;
    QLineEdit *pathEdit = nullptr;
    ErrorLabel *errorLabel = nullptr;

    ResultModel *m_resultModel = nullptr;
    ConvertedTestCaseModel *m_convertedTestCaseModel = nullptr;
    ExampleModel *m_exampleModel = nullptr;
    ProblemConfModel *m_problemConfModel = nullptr;
    StdModel *m_stdModel = nullptr;
};

#pragma once

#include <QWizardPage>

class QComboBox;
class QPlainTextEdit;
class QSpinBox;

class ConvertedTestCaseModel;
class ExampleModel;
class ProblemConfModel;
class SubtaskModel;

class ProblemConfPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit ProblemConfPage(ProblemConfModel *problemConfModel,
                             ConvertedTestCaseModel *convertedTestCaseModel,
                             SubtaskModel *subtaskModel, ExampleModel *exampleModel,
                             QWidget *parent = nullptr);

    void initializePage() override;

    bool validatePage() override;

   private slots:
    void updateProblemConf();

   private:
    QSpinBox *timeLimitSpinBox = nullptr;
    QSpinBox *memoryLimitSpinBox = nullptr;
    QComboBox *checkerComboBox = nullptr;
    QPlainTextEdit *problemConfEdit = nullptr;

    ProblemConfModel *m_problemConfModel = nullptr;
    ConvertedTestCaseModel *m_convertedTestCaseModel = nullptr;
    SubtaskModel *m_subtaskModel = nullptr;
    ExampleModel *m_exampleModel = nullptr;
};

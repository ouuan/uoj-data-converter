#pragma once

#include <QWizardPage>

#include "Pages/ExamplePage.hpp"
#include "Pages/SubtaskPage.hpp"
#include "Pages/TestCaseConvertPage.hpp"

class QComboBox;
class QPlainTextEdit;
class QSpinBox;

class ProblemConfPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit ProblemConfPage(TestCaseConvertPage *testCaseConvertPage, SubtaskPage *subtaskPage,
                             ExamplePage *examplePage, QWidget *parent = nullptr);

    void initializePage() override;

    QString getProblemConf() const;

   private slots:
    void updateProblemConf();

   private:
    QSpinBox *timeLimitSpinBox = nullptr;
    QSpinBox *memoryLimitSpinBox = nullptr;
    QComboBox *checkerComboBox = nullptr;
    QPlainTextEdit *problemConfEdit = nullptr;

    TestCaseConvertPage *m_testCaseConvertPage;
    SubtaskPage *m_subtaskPage;
    ExamplePage *m_examplePage;

    QString name;
    QVector<QVector<TestCaseConvertPage::TestCase>> testCases;
    QVector<SubtaskPage::Subtask> subtasks;
    QVector<ExamplePage::Example> examples;
};

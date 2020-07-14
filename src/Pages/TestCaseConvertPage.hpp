#pragma once

#include <QWizardPage>

class ErrorLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;
class TestCaseChoosePage;

class TestCaseConvertPage : public QWizardPage
{
    Q_OBJECT

   public:
    struct TestCase
    {
        int id;
        QString originalInput;
        QString originalOutput;
        QString convertedInput;
        QString convertedOutput;
    };

    explicit TestCaseConvertPage(TestCaseChoosePage *testCaseChoosePage, QWidget *parent = nullptr);

    void initializePage() override;

    bool isComplete() const override;

    int nextId() const override;

    QString getProblemName() const;

    QVector<QVector<TestCase>> getTestCases() const;

   private slots:
    void updateResult();

    void setSubtask();

    void updateSubtaskButton();

   private:
    TestCaseChoosePage *choosePage = nullptr;
    QLineEdit *problemNameEdit = nullptr;
    QLineEdit *inputPatternEdit = nullptr;
    QLineEdit *outputPatternEdit = nullptr;
    ErrorLabel *errorLabel = nullptr;
    QTableWidget *table = nullptr;
    QPushButton *subtaskButton = nullptr;

    QVector<bool> subtaskBegins;  // whether a test case is the beginning of a subtask
    QStringList inputs;
    QStringList outputs;
    QVector<QVector<TestCase>> testCases;
};

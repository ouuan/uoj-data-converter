#pragma once

#include <QWizardPage>

class ErrorLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;

class ConvertedTestCaseModel;
class OriginalTestCaseModel;

class TestCaseConvertPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit TestCaseConvertPage(ConvertedTestCaseModel *convertedTestCaseModel,
                                 OriginalTestCaseModel *originalTestCaseModel,
                                 QWidget *parent = nullptr);

    void initializePage() override;

    bool isComplete() const override;

    int nextId() const override;

   private slots:
    void updateResult();

    void setSubtask();

    void updateSubtaskButton();

   private:
    QLineEdit *problemNameEdit = nullptr;
    QLineEdit *inputPatternEdit = nullptr;
    QLineEdit *outputPatternEdit = nullptr;
    ErrorLabel *errorLabel = nullptr;
    QTableWidget *table = nullptr;
    QPushButton *subtaskButton = nullptr;

    QVector<bool> subtaskBegins;  // whether a test case is the beginning of a subtask

    ConvertedTestCaseModel *m_convertedTestCaseModel = nullptr;
    OriginalTestCaseModel *m_originalTestCaseModel = nullptr;
};

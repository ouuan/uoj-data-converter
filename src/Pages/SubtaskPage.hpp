#pragma once

#include <QWizardPage>

class ErrorLabel;
class QTableWidget;
class TestCaseConvertPage;

class SubtaskPage : public QWizardPage
{
    Q_OBJECT

   public:
    struct Subtask
    {
        int start;
        int end;
        int score;
        QVector<int> dependency;
    };

    explicit SubtaskPage(TestCaseConvertPage *testCaseConvertPage, QWidget *parent = nullptr);

    void initializePage() override;

    void cleanupPage() override;

    bool isComplete() const override;

    QVector<Subtask> getSubtasks() const;

   private slots:
    void updateSubtasks();

   private:
    TestCaseConvertPage *convertPage = nullptr;
    ErrorLabel *errorLabel = nullptr;
    QTableWidget *table = nullptr;

    QMetaObject::Connection updateConnection;

    bool valid = false;
    QVector<Subtask> subtasks;
};

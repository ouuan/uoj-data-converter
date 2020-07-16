#pragma once

#include <QWizardPage>

class ErrorLabel;
class QTableWidget;

class ConvertedTestCaseModel;
class SubtaskModel;

class SubtaskPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit SubtaskPage(SubtaskModel *subtaskModel, ConvertedTestCaseModel *convertedTestCaseModel,
                         QWidget *parent = nullptr);

    void initializePage() override;

    void cleanupPage() override;

    bool isComplete() const override;

   private slots:
    void updateSubtasks();

   private:
    ErrorLabel *errorLabel = nullptr;
    QTableWidget *table = nullptr;

    QMetaObject::Connection updateConnection;

    bool valid = false;

    SubtaskModel *m_subtaskModel = nullptr;
    ConvertedTestCaseModel *m_convertedTestCaseModel = nullptr;
};

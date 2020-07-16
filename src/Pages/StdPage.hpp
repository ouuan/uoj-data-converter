#pragma once

#include <QWizardPage>

class ErrorLabel;
class QCheckBox;
class QLineEdit;

class StdModel;

class StdPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit StdPage(StdModel *stdModel, QWidget *parent = nullptr);

    bool isComplete() const override;

   private slots:
    void chooseStd();

    void updateStd();

   private:
    QLineEdit *pathEdit = nullptr;
    ErrorLabel *errorLabel = nullptr;
    QCheckBox *removeFreopenCheckBox = nullptr;

    StdModel *m_stdModel = nullptr;
};

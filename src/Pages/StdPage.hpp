#pragma once

#include <QWizardPage>

class ErrorLabel;
class QCheckBox;
class QLineEdit;

class StdPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit StdPage(QWidget *parent = nullptr);

    bool isComplete() const override;

    QString getStd() const;

   private slots:
    void chooseStd();

    void updateStd();

   private:
    QLineEdit *pathEdit = nullptr;
    ErrorLabel *errorLabel = nullptr;
    QCheckBox *removeFreopenCheckBox = nullptr;

    QString stdContent;
};

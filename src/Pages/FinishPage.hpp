#pragma once

#include <QWizardPage>

class QCheckBox;

class ResultModel;
class StdModel;

class FinishPage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit FinishPage(ResultModel *resultModel, StdModel *stdModel, QWidget *parent = nullptr);

    void initializePage() override;

    bool validatePage() override;

   private:
    QCheckBox *openOutputCheckBox = nullptr;
    QCheckBox *copyStdCheckBox = nullptr;

    ResultModel *m_resultModel = nullptr;
    StdModel *m_stdModel = nullptr;
};

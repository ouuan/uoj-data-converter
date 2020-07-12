#pragma once

#include <QListWidget>
#include <QWizardPage>

class QPushButton;

namespace TestCaseChoose
{
class ListWidget : public QListWidget
{
    Q_OBJECT

   public:
    explicit ListWidget(QWidget *parent = nullptr);

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dragMoveEvent(QDragMoveEvent *event) override;

    void dropEvent(QDropEvent *event) override;

    void addItem(const QString &label);

    QStringList itemLabels() const;

   public slots:
    void deleteSelectedItems();

   signals:
    void itemChanged();
};
}  // namespace TestCaseChoose

class TestCaseChoosePage : public QWizardPage
{
    Q_OBJECT

   public:
    explicit TestCaseChoosePage(QWidget *parent = nullptr);

    bool isComplete() const override;

    QStringList inputs() const;

    QStringList outputs() const;

   private slots:
    void addInput();

    void addOutput();

    void updateButtons();

   private:
    TestCaseChoose::ListWidget *inputList = nullptr;
    TestCaseChoose::ListWidget *outputList = nullptr;
    QPushButton *deleteInputButton = nullptr;
    QPushButton *deleteOutputButton = nullptr;
};
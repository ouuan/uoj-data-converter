#pragma once

#include <QListWidget>
#include <QVBoxLayout>
#include <QWizardPage>

class QPushButton;

class TestCaseChoosePage;

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

    void naturalSort();

   signals:
    void itemChanged();
};

class TestCaseChooseLayout : public QVBoxLayout
{
    Q_OBJECT

   public:
    explicit TestCaseChooseLayout(const QString &name, const QString &filter,
                                  TestCaseChoosePage *parent);

   private slots:
    void addTestCase();

    void updateDeleteButton();

   private:
    ListWidget *list = nullptr;
    QPushButton *deleteButton = nullptr;

    QString m_name;
    QString m_filter;

    friend class ::TestCaseChoosePage;
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

   private:
    TestCaseChoose::TestCaseChooseLayout *inputLayout = nullptr;
    TestCaseChoose::TestCaseChooseLayout *outputLayout = nullptr;
};

#pragma once

#include <QMap>
#include <QWizardPage>

class AcceptsDropEdit;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QStackedWidget;

class ExampleEdits;

class ExamplePage : public QWizardPage
{
    Q_OBJECT

   public:
    struct Example
    {
        QString input;
        QString output;
    };

    explicit ExamplePage(QWidget *parent = nullptr);

    QVector<Example> getExamples() const;

   private slots:
    void switchToExample(QListWidgetItem *item);

    void addExample();

    void deleteExample();

    void updateDeleteButton();

   private:
    QListWidget *list = nullptr;
    QPushButton *deleteButton = nullptr;
    QStackedWidget *stackedWidget = nullptr;

    QMap<QListWidgetItem *, ExampleEdits *> exampleEditsForItem;
    QMap<ExampleEdits *, QListWidgetItem *> itemForExampleEdits;
};

class ExampleEdits : public QWidget
{
    Q_OBJECT

   public:
    explicit ExampleEdits(QWidget *parent = nullptr);

    ExamplePage::Example getExample();

   private:
    AcceptsDropEdit *inputEdit = nullptr;
    AcceptsDropEdit *outputEdit = nullptr;
};

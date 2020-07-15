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

class ExampleEdit : public QWidget
{
    Q_OBJECT

   public:
    explicit ExampleEdit(const QString &name, const QString &filter, QWidget *parent = nullptr);

    QString getContent() const;

   private slots:
    void chooseFile();

    void clearContent();

   private:
    AcceptsDropEdit *edit = nullptr;

    QString m_name;
    QString m_filter;
};

class ExampleEdits : public QWidget
{
    Q_OBJECT

   public:
    explicit ExampleEdits(QWidget *parent = nullptr);

    ExamplePage::Example getExample();

   private:
    ExampleEdit *inputEdit = nullptr;
    ExampleEdit *outputEdit = nullptr;
};

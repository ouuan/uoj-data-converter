#pragma once

#include <QPlainTextEdit>

class AcceptsDropEdit : public QPlainTextEdit
{
    Q_OBJECT

   public:
    explicit AcceptsDropEdit(int maxDisplayLength, QWidget *parent = nullptr);

    void modifyText(const QString &text);

    QString getContent();

   protected:
    void dragEnterEvent(QDragEnterEvent *event) override;

    void dragMoveEvent(QDragMoveEvent *event) override;

    void dropEvent(QDropEvent *event) override;

   private:
    int maxLength;
    QString content;
};

#pragma once

#include <QPlainTextEdit>

class AcceptsDropEdit : public QPlainTextEdit
{
    Q_OBJECT

   public:
    explicit AcceptsDropEdit(QWidget *parent = nullptr);

   protected:
    void dragEnterEvent(QDragEnterEvent *event) override;

    void dragMoveEvent(QDragMoveEvent *event) override;

    void dropEvent(QDropEvent *event) override;
};

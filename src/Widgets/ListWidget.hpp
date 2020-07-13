#pragma once

#include <QListWidget>

class ListWidget : public QListWidget
{
    Q_OBJECT

   public:
    explicit ListWidget(QWidget *parent = nullptr);

    void addItem(const QString &label);

    QStringList itemLabels() const;

   public slots:
    void deleteSelectedItems();

    void naturalSort();

   protected:
    void dragEnterEvent(QDragEnterEvent *event) override;

    void dragMoveEvent(QDragMoveEvent *event) override;

    void dropEvent(QDropEvent *event) override;

   signals:
    void itemChanged();
};
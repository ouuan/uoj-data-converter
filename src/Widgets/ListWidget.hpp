#pragma once

#include <QListWidget>

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
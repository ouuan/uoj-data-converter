#include "ListWidget.hpp"

#include <QCollator>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <algorithm>

ListWidget::ListWidget(QWidget *parent) : QListWidget(parent)
{
    setAcceptDrops(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void ListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->accept();
        event->acceptProposedAction();
    }
}

void ListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->accept();
        event->acceptProposedAction();
    }
}

void ListWidget::dropEvent(QDropEvent *event)
{
    const auto urls = event->mimeData()->urls();
    if (!urls.isEmpty())
    {
        for (auto url : urls)
        {
            const auto path = url.toLocalFile();
            if (QFileInfo(path).isFile() && !itemLabels().contains(path))
                addItem(url.toLocalFile());
        }
        event->acceptProposedAction();
    }
}

void ListWidget::addItem(const QString &title)
{
    QListWidget::addItem(title);
    emit itemChanged();
}

void ListWidget::deleteSelectedItems()
{
    for (auto item : selectedItems())
        delete item;
    emit itemChanged();
}

void ListWidget::naturalSort()
{
    QStringList labels;

    while (count())
    {
        auto item = takeItem(0);
        labels.push_back(item->text());
        delete item;
    }

    QCollator collator;
    collator.setNumericMode(true);
    std::sort(labels.begin(), labels.end(), [&collator](const QString &x, const QString &y) {
        return collator.compare(x, y) < 0;
    });

    addItems(labels);

    emit itemChanged();
}

QStringList ListWidget::itemLabels() const
{
    QStringList res;

    for (int i = 0; i < count(); ++i)
        res.push_back(item(i)->text());

    return res;
}
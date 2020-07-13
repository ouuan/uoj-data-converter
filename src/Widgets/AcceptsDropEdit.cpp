#include "AcceptsDropEdit.hpp"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFile>
#include <QMimeData>

AcceptsDropEdit::AcceptsDropEdit(QWidget *parent) : QPlainTextEdit(parent)
{}

void AcceptsDropEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->accept();
        event->acceptProposedAction();
    }
}

void AcceptsDropEdit::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->accept();
        event->acceptProposedAction();
    }
}

void AcceptsDropEdit::dropEvent(QDropEvent *event)
{
    const auto urls = event->mimeData()->urls();
    if (!urls.isEmpty())
    {
        for (auto url : urls)
        {
            QFile file(url.toLocalFile());
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                auto cursor = textCursor();
                cursor.select(QTextCursor::Document);
                cursor.insertText(file.readAll());
                event->acceptProposedAction();
                break;
            }
        }
    }
}

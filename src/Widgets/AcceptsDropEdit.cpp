#include "AcceptsDropEdit.hpp"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFile>
#include <QMimeData>

AcceptsDropEdit::AcceptsDropEdit(int maxDisplayLength, QWidget *parent)
    : QPlainTextEdit(parent), maxLength(maxDisplayLength)
{
    setAcceptDrops(true);
    setWordWrapMode(QTextOption::NoWrap);
}

void AcceptsDropEdit::modifyText(const QString &text)
{
    content = text;

    QString displayText;

    if (text.length() <= maxLength)
    {
        displayText = text;
        setReadOnly(false);
    }
    else
    {
        displayText =
            QString("内容过长，只显示了前 %1 个字符，且无法手动修改，只能再次载入文件或清空\n\n")
                .arg(maxLength) +
            text.left(maxLength) + "...";
        setReadOnly(true);
    }

    auto cursor = textCursor();
    cursor.select(QTextCursor::Document);
    cursor.insertText(displayText);
}

QString AcceptsDropEdit::getContent()
{
    if (!isReadOnly())
        content = toPlainText();
    return content;
}

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
                modifyText(file.readAll());
                event->acceptProposedAction();
                break;
            }
        }
    }
}

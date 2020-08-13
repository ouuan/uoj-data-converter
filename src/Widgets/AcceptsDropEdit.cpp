/*
 * Copyright 2020 Yufan You
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

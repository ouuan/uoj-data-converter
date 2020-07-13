#include "ErrorLabel.hpp"

ErrorLabel::ErrorLabel(QWidget *parent) : QLabel(parent)
{
    setWordWrap(true);
    setStyleSheet("QLabel { color: red }");
}

void ErrorLabel::showError(const QString &error)
{
    if (error.isEmpty())
    {
        clear();
        hide();
    }
    else
    {
        setText(error);
        show();
    }
}

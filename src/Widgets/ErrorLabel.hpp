#pragma once

#include <QLabel>

class ErrorLabel : public QLabel
{
    Q_OBJECT

   public:
    explicit ErrorLabel(QWidget *parent = nullptr);

    void showError(const QString &error);
};

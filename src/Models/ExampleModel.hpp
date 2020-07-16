#pragma once

#include <QString>
#include <QVector>

class ExampleModel
{
   public:
    struct Example
    {
        QString input;
        QString output;
    };

    QVector<Example> examples() const
    {
        return m_examples;
    }

   private:
    QVector<Example> m_examples;

    friend class ExamplePage;
};
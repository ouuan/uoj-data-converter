#pragma once

#include <QString>
#include <QVector>

class ConvertedTestCaseModel
{
   public:
    struct TestCase
    {
        int id;
        QString originalInput;
        QString originalOutput;
        QString convertedInput;
        QString convertedOutput;
    };

    QVector<QVector<TestCase>> testCases() const
    {
        return m_testCases;
    }

    QString name() const
    {
        return m_name;
    }

   private:
    QVector<QVector<TestCase>> m_testCases;
    QString m_name;

    friend class TestCaseConvertPage;
};
#pragma once

#include <QStringList>

class OriginalTestCaseModel
{
   public:
    QStringList inputs() const
    {
        return m_inputs;
    }
    QStringList outputs() const
    {
        return m_outputs;
    }

   private:
    QStringList m_inputs;
    QStringList m_outputs;

    friend class TestCaseChoosePage;
};

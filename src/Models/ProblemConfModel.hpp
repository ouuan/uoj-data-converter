#pragma once

#include <QString>

class ProblemConfModel
{
   public:
    QString problemConf() const
    {
        return m_problemConf;
    }

   private:
    QString m_problemConf;

    friend class ProblemConfPage;
};

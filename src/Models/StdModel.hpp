#pragma once

#include <QString>

class StdModel
{
   public:
    QString std() const
    {
        return m_std;
    }

   private:
    QString m_std;

    friend class StdPage;
};

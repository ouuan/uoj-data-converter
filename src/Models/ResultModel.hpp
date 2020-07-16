#pragma once

#include <QString>

class ResultModel
{
   public:
    QString outputPath() const
    {
        return m_outputPath;
    }

   private:
    QString m_outputPath;

    friend class CommitOperationPage;
};

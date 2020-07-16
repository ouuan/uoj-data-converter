#pragma once

#include <QVector>

class SubtaskModel
{
   public:
    struct Subtask
    {
        int start;
        int end;
        int score;
        QVector<int> dependency;
    };

    QVector<Subtask> subtasks() const
    {
        return m_subtasks;
    }

   private:
    QVector<Subtask> m_subtasks;

    friend class SubtaskPage;
};

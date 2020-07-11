#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

   public:
    explicit MainWindow();

   private slots:
    void showBuildInfo();

    void showAbout();

    void startGuide();
};

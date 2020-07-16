#include "MainWindow.hpp"

#include <QApplication>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWizard>

#include "Models/ConvertedTestCaseModel.hpp"
#include "Models/ExampleModel.hpp"
#include "Models/OriginalTestCaseModel.hpp"
#include "Models/ProblemConfModel.hpp"
#include "Models/ResultModel.hpp"
#include "Models/StdModel.hpp"
#include "Models/SubtaskModel.hpp"
#include "Pages/CommitOperationPage.hpp"
#include "Pages/ExamplePage.hpp"
#include "Pages/FinishPage.hpp"
#include "Pages/ProblemConfPage.hpp"
#include "Pages/StdPage.hpp"
#include "Pages/SubtaskPage.hpp"
#include "Pages/TestCaseChoosePage.hpp"
#include "Pages/TestCaseConvertPage.hpp"
#include "generated/version.hpp"

MainWindow::MainWindow()
{
    setWindowTitle("UOJ Data Converter");

    auto actionMenu = menuBar()->addMenu("动作");
    actionMenu->addAction("运行向导", this, &MainWindow::startGuide, {"Ctrl+N"});
    actionMenu->addAction("退出", qApp, &QApplication::quit, {"Ctrl+Q"});

    auto aboutMenu = menuBar()->addMenu("帮助");

    aboutMenu->addAction("构建信息", this, &MainWindow::showBuildInfo);
    aboutMenu->addAction("关于", this, &MainWindow::showAbout);
    aboutMenu->addAction("关于 Qt", qApp, &QApplication::aboutQt);

    auto widget = new QWidget(this);
    setCentralWidget(widget);

    auto layout = new QVBoxLayout(widget);

    auto label = new QLabel("UOJ Data Converter", widget);
    layout->addWidget(label, 0, Qt::AlignCenter);

    auto startButton = new QPushButton("开始", widget);
    startButton->setMinimumHeight(180);
    startButton->setMinimumWidth(320);
    layout->addWidget(startButton, 0, Qt::AlignCenter);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startGuide);
}

void MainWindow::showBuildInfo()
{
    QMessageBox::about(this, "构建信息",
                       QString("版本：%1\n构建时间：%2\n操作系统：%3")
                           .arg(DISPLAY_VERSION)
                           .arg(__DATE__ " " __TIME__)
#if defined(Q_OS_UNIX)
                           .arg("Linux")
#elif defined(Q_OS_WIN)
                           .arg("Windows")
#elif defined(Q_OS_MACOS)
                           .arg("macOS")
#else
                           .arg("Unknown")
#endif
    );
}

void MainWindow::showAbout()
{
    QMessageBox::about(
        this, "关于 UOJ Data Converter",
        "UOJ Data Converter 可以将各种格式的数据转换为 UOJ 的格式。\n\n本软件是自由软件，以 Apache "
        "2.0 License 开源，你可以在 https://github.com/ouuan/uoj-data-converter 获取源码。");
}

void MainWindow::startGuide()
{
    QWizard guide(this);

    auto convertedTestCaseModel = new ConvertedTestCaseModel();
    auto exampleModel = new ExampleModel();
    auto originalTestCaseModel = new OriginalTestCaseModel();
    auto problemConfModel = new ProblemConfModel();
    auto resultModel = new ResultModel();
    auto stdModel = new StdModel();
    auto subtaskModel = new SubtaskModel();

    auto testCaseChoosePage = new TestCaseChoosePage(originalTestCaseModel, &guide);
    auto testCaseConvertPage =
        new TestCaseConvertPage(convertedTestCaseModel, originalTestCaseModel, &guide);
    auto subtaskPage = new SubtaskPage(subtaskModel, convertedTestCaseModel, &guide);
    auto examplePage = new ExamplePage(exampleModel, &guide);
    auto problemConfPage = new ProblemConfPage(problemConfModel, convertedTestCaseModel,
                                               subtaskModel, exampleModel, &guide);
    auto stdPage = new StdPage(stdModel, &guide);
    auto commitOperationPage = new CommitOperationPage(
        resultModel, convertedTestCaseModel, exampleModel, problemConfModel, stdModel, &guide);
    auto finishPage = new FinishPage(resultModel, stdModel, &guide);

    guide.addPage(testCaseChoosePage);
    guide.addPage(testCaseConvertPage);
    guide.addPage(subtaskPage);
    guide.addPage(examplePage);
    guide.addPage(problemConfPage);
    guide.addPage(stdPage);
    guide.addPage(commitOperationPage);
    guide.addPage(finishPage);
    guide.setWindowTitle("数据转换向导");

    guide.exec();
}

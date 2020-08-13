/*
 * Copyright 2020 Yufan You
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

    ConvertedTestCaseModel convertedTestCaseModel;
    ExampleModel exampleModel;
    OriginalTestCaseModel originalTestCaseModel;
    ProblemConfModel problemConfModel;
    ResultModel resultModel;
    StdModel stdModel;
    SubtaskModel subtaskModel;

    TestCaseChoosePage testCaseChoosePage(&originalTestCaseModel, &guide);
    TestCaseConvertPage testCaseConvertPage(&convertedTestCaseModel, &originalTestCaseModel,
                                            &guide);
    SubtaskPage subtaskPage(&subtaskModel, &convertedTestCaseModel, &guide);
    ExamplePage examplePage(&exampleModel, &guide);
    ProblemConfPage problemConfPage(&problemConfModel, &convertedTestCaseModel, &subtaskModel,
                                    &exampleModel, &guide);
    StdPage stdPage(&stdModel, &guide);
    CommitOperationPage commitOperationPage(&resultModel, &convertedTestCaseModel, &exampleModel,
                                            &problemConfModel, &stdModel, &guide);
    FinishPage finishPage(&resultModel, &stdModel, &guide);

    guide.addPage(&testCaseChoosePage);
    guide.addPage(&testCaseConvertPage);
    guide.addPage(&subtaskPage);
    guide.addPage(&examplePage);
    guide.addPage(&problemConfPage);
    guide.addPage(&stdPage);
    guide.addPage(&commitOperationPage);
    guide.addPage(&finishPage);
    guide.setWindowTitle("数据转换向导");

    guide.exec();
}

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

#include <QApplication>
#include <QCommandLineParser>

#include "MainWindow.hpp"
#include "generated/version.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("UOJ Data Converter");
    QCoreApplication::setApplicationVersion(DISPLAY_VERSION);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setWindowIcon(QIcon(":/icon.png"));

    QCommandLineParser parser;
    parser.setApplicationDescription("将各种各样格式的数据转换为 UOJ 的格式。");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(app);

    MainWindow mainWindow;

    mainWindow.show();

    return app.exec();
}

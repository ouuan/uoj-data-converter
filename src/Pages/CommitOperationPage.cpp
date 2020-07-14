#include "CommitOperationPage.hpp"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QRadioButton>
#include <QStyle>
#include <QTemporaryDir>
#include <QToolButton>
#include <QVBoxLayout>

#include "ProblemConfPage.hpp"
#include "Widgets/ErrorLabel.hpp"

CommitOperationPage::CommitOperationPage(ProblemConfPage *problemConfPage, QWidget *parent)
    : QWizardPage(parent), m_problemConfPage(problemConfPage)
{
    Q_ASSERT(problemConfPage != nullptr);

    setTitle("应用操作");
    setCommitPage(true);

    auto mainLayout = new QVBoxLayout(this);

    copyButton = new QRadioButton("复制数据文件", this);
    mainLayout->addWidget(copyButton);

    moveButton = new QRadioButton("移动数据文件", this);
    mainLayout->addWidget(moveButton);

    auto pathLayout = new QHBoxLayout();
    mainLayout->addLayout(pathLayout);

    auto pathLabel = new QLabel("输出路径", this);
    pathLabel->setToolTip("转换后文件的输出文件夹");
    pathLayout->addWidget(pathLabel);

    pathEdit = new QLineEdit(this);
    pathLayout->addWidget(pathEdit);

    auto pathButton = new QToolButton(this);
    pathButton->setToolTip("选择文件夹");
    pathButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton));
    connect(pathButton, &QToolButton::clicked, this, &CommitOperationPage::choosePath);
    pathLayout->addWidget(pathButton);

    errorLabel = new ErrorLabel(this);
    mainLayout->addWidget(errorLabel);
}

void CommitOperationPage::initializePage()
{
    copyButton->setChecked(true);
    pathEdit->setText(
        QFileInfo(m_problemConfPage->getProblem().testCases.front().front().originalInput)
            .canonicalPath());
}

bool CommitOperationPage::validatePage()
{
#define ERR(x)                \
    errorLabel->showError(x); \
    return false;

    errorLabel->hide();

    if (!QFileInfo(pathEdit->text()).isDir())
    {
        ERR("输出路径不是一个文件夹")
    }

    auto problem = m_problemConfPage->getProblem();

    // 检查输出的文件是否在输出路径中存在

    QDir outputDir(pathEdit->text());

    QStringList exists;

#define CHECK_EXISTS(x)      \
    if (outputDir.exists(x)) \
    exists.push_back(x)

    CHECK_EXISTS("problem.conf");

    for (int i = 0; i < problem.testCases.back().back().id; ++i)
    {
        CHECK_EXISTS(QString("%1%2.in").arg(problem.name).arg(i + 1));
        CHECK_EXISTS(QString("%1%2.ans").arg(problem.name).arg(i + 1));
    }

    for (int i = 0; i < problem.examples.count(); ++i)
    {
        CHECK_EXISTS(QString("ex_%1%2.in").arg(problem.name).arg(i + 1));
        CHECK_EXISTS(QString("ex_%1%2.ans").arg(problem.name).arg(i + 1));
    }

#undef CHECK_EXISTS

    // 若输出路径中有会被覆盖的文件，询问用户是否覆盖

    if (!exists.isEmpty())
    {
        auto res =
            QMessageBox::question(this, "覆盖文件",
                                  QString("下列文件在输出路径 [%1] 中存在，要覆盖它们吗？\n\n%2")
                                      .arg(pathEdit->text())
                                      .arg(exists.join("\n")),
                                  QMessageBox::Yes | QMessageBox::No);
        if (res == QMessageBox::No)
            return false;
    }

    // 将数据复制/移动到临时文件夹，避免在过程中被覆盖

    QTemporaryDir tempDir;
    tempDir.setAutoRemove(false);

    if (!tempDir.isValid())
    {
        ERR("无法创建临时文件夹，错误：" + tempDir.errorString())
    }

    for (auto subtask : problem.testCases)
    {
        for (auto testCase : subtask)
        {
            if (copyButton->isChecked())
            {
                if (!QFile::copy(
                        testCase.originalInput,
                        tempDir.filePath(QString("%1%2.in").arg(problem.name).arg(testCase.id))))
                {
                    ERR(QString("将 [%1] 复制到 [%2] 时失败")
                            .arg(testCase.originalInput)
                            .arg(tempDir.path()))
                }
                if (!QFile::copy(
                        testCase.originalOutput,
                        tempDir.filePath(QString("%1%2.ans").arg(problem.name).arg(testCase.id))))
                {
                    ERR(QString("将 [%1] 复制到 [%2] 时失败")
                            .arg(testCase.originalOutput)
                            .arg(tempDir.path()))
                }
            }
            else
            {
                if (!QFile::rename(
                        testCase.originalInput,
                        tempDir.filePath(QString("%1%2.in").arg(problem.name).arg(testCase.id))))
                {
                    ERR(QString("将 [%1] 移动到 [%2] 时失败")
                            .arg(testCase.originalInput)
                            .arg(tempDir.path()))
                }
                if (!QFile::rename(
                        testCase.originalOutput,
                        tempDir.filePath(QString("%1%2.ans").arg(problem.name).arg(testCase.id))))
                {
                    ERR(QString("将 [%1] 移动到 [%2] 时失败")
                            .arg(testCase.originalOutput)
                            .arg(tempDir.path()))
                }
            }
        }
    }

    // 将数据从临时文件夹移动到输出路径

#define DELETE_IF_EXISTS(x)                     \
    if (QFile::exists(x) && !QFile(x).remove()) \
    {                                           \
        ERR(QString("删除 [%1] 时失败").arg(x)) \
    }

    QDir sourceDir(tempDir.path());

    for (auto info : sourceDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files))
    {
        QFile file(info.filePath());
        const QString mvPath = outputDir.filePath(info.fileName());
        DELETE_IF_EXISTS(mvPath);
        if (!file.rename(mvPath))
        {
            ERR(QString("将 [%1] 移动到 [%2] 时失败").arg(info.filePath()).arg(mvPath))
        }
    }

    // 在输出路径写入样例

    for (int i = 0; i < problem.examples.count(); ++i)
    {
        const QString inputPath =
            outputDir.filePath(QString("ex_%1%2.in").arg(problem.name).arg(i + 1));
        DELETE_IF_EXISTS(inputPath);
        QFile inputFile(inputPath);
        if (!inputFile.open(QIODevice::WriteOnly | QIODevice::Text) ||
            inputFile.write(problem.examples[i].input.toUtf8()) == -1)
        {
            ERR(QString("写入 [%1] 时失败").arg(inputPath))
        }

        const QString outputPath =
            outputDir.filePath(QString("ex_%1%2.ans").arg(problem.name).arg(i + 1));
        DELETE_IF_EXISTS(outputPath);
        QFile outputFile(outputPath);
        if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text) ||
            outputFile.write(problem.examples[i].output.toUtf8()) == -1)
        {
            ERR(QString("写入 [%1] 时失败").arg(outputPath))
        }
    }

    // 在输出路径写入 problem.conf

    const QString problemConfPath = outputDir.filePath("problem.conf");
    DELETE_IF_EXISTS(problemConfPath);
    QFile problemConfFile(problemConfPath);
    if (!problemConfFile.open(QIODevice::WriteOnly | QIODevice::Text) ||
        problemConfFile.write(problem.problemConf.toUtf8()) == -1)
    {
        ERR(QString("写入 [%1] 时失败").arg(problemConfPath))
    }

#undef DELETE_IF_EXISTS

    tempDir.remove();
    return true;

#undef ERR
}

QString CommitOperationPage::getOutputPath() const
{
    return pathEdit->text();
}

void CommitOperationPage::choosePath()
{
    auto path = QFileDialog::getExistingDirectory(this, "选择输出路径");
    if (!path.isEmpty())
        pathEdit->setText(path);
}

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

#include "Models/ConvertedTestCaseModel.hpp"
#include "Models/ExampleModel.hpp"
#include "Models/ProblemConfModel.hpp"
#include "Models/ResultModel.hpp"
#include "Models/StdModel.hpp"
#include "Widgets/ErrorLabel.hpp"

CommitOperationPage::CommitOperationPage(ResultModel *resultModel,
                                         ConvertedTestCaseModel *convertedTestCaseModel,
                                         ExampleModel *exampleModel,
                                         ProblemConfModel *problemConfModel, StdModel *stdModel,
                                         QWidget *parent)
    : QWizardPage(parent),
      m_resultModel(resultModel),
      m_convertedTestCaseModel(convertedTestCaseModel),
      m_exampleModel(exampleModel),
      m_problemConfModel(problemConfModel),
      m_stdModel(stdModel)
{
    Q_ASSERT(m_resultModel != nullptr);
    Q_ASSERT(m_convertedTestCaseModel != nullptr);
    Q_ASSERT(m_exampleModel != nullptr);
    Q_ASSERT(m_problemConfModel != nullptr);
    Q_ASSERT(m_stdModel != nullptr);

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
    pathEdit->setText(QFileInfo(m_convertedTestCaseModel->testCases().front().front().originalInput)
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

    auto testCases = m_convertedTestCaseModel->testCases();
    auto name = m_convertedTestCaseModel->name();
    auto examples = m_exampleModel->examples();

    // 检查输出的文件是否在输出路径中存在

    QDir outputDir(pathEdit->text());

    QStringList exists;

#define CHECK_EXISTS(x)      \
    if (outputDir.exists(x)) \
    exists.push_back(x)

    CHECK_EXISTS("problem.conf");
    CHECK_EXISTS("std.cpp");

    for (int i = 0; i < testCases.back().back().id; ++i)
    {
        CHECK_EXISTS(QString("%1%2.in").arg(name).arg(i + 1));
        CHECK_EXISTS(QString("%1%2.ans").arg(name).arg(i + 1));
    }

    for (int i = 0; i < examples.count(); ++i)
    {
        CHECK_EXISTS(QString("ex_%1%2.in").arg(name).arg(i + 1));
        CHECK_EXISTS(QString("ex_%1%2.ans").arg(name).arg(i + 1));
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

    for (auto subtask : testCases)
    {
        for (auto testCase : subtask)
        {
            if (copyButton->isChecked())
            {
                if (!QFile::copy(testCase.originalInput,
                                 tempDir.filePath(QString("%1%2.in").arg(name).arg(testCase.id))))
                {
                    ERR(QString("将 [%1] 复制到 [%2] 时失败")
                            .arg(testCase.originalInput)
                            .arg(tempDir.path()))
                }
                if (!QFile::copy(testCase.originalOutput,
                                 tempDir.filePath(QString("%1%2.ans").arg(name).arg(testCase.id))))
                {
                    ERR(QString("将 [%1] 复制到 [%2] 时失败")
                            .arg(testCase.originalOutput)
                            .arg(tempDir.path()))
                }
            }
            else
            {
                if (!QFile::rename(testCase.originalInput,
                                   tempDir.filePath(QString("%1%2.in").arg(name).arg(testCase.id))))
                {
                    ERR(QString("将 [%1] 移动到 [%2] 时失败")
                            .arg(testCase.originalInput)
                            .arg(tempDir.path()))
                }
                if (!QFile::rename(
                        testCase.originalOutput,
                        tempDir.filePath(QString("%1%2.ans").arg(name).arg(testCase.id))))
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

#undef DELETE_IF_EXISTS

    tempDir.remove();

#define OVERWRITE(path, content)                                    \
    QFile path##File(path);                                         \
    if (!path##File.open(QIODevice::WriteOnly | QIODevice::Text) || \
        path##File.write(content.toUtf8()) == -1)                   \
    {                                                               \
        ERR(QString("写入 [%1] 时失败").arg(path))                  \
    }

    // 在输出路径写入样例

    for (int i = 0; i < examples.count(); ++i)
    {
        const QString inputPath = outputDir.filePath(QString("ex_%1%2.in").arg(name).arg(i + 1));
        OVERWRITE(inputPath, examples[i].input)

        const QString outputPath = outputDir.filePath(QString("ex_%1%2.ans").arg(name).arg(i + 1));
        OVERWRITE(outputPath, examples[i].output)
    }

    // 在输出路径写入 problem.conf

    const QString problemConfPath = outputDir.filePath("problem.conf");
    OVERWRITE(problemConfPath, m_problemConfModel->problemConf());

    // 在输出路径中写入 std.cpp

    const QString stdPath = outputDir.filePath("std.cpp");
    const QString stdContent = m_stdModel->std();
    if (!stdContent.isNull())
    {
        OVERWRITE(stdPath, stdContent)
    }

    m_resultModel->m_outputPath = pathEdit->text();

    return true;

#undef ERR
}

void CommitOperationPage::choosePath()
{
    auto path = QFileDialog::getExistingDirectory(this, "选择输出路径");
    if (!path.isEmpty())
        pathEdit->setText(path);
}

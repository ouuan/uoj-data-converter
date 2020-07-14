#include "TestCaseConvertPage.hpp"

#include <QFileInfo>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QTableWidget>
#include <QVBoxLayout>

#include "TestCaseChoosePage.hpp"
#include "Widgets/ErrorLabel.hpp"

TestCaseConvertPage::TestCaseConvertPage(TestCaseChoosePage *testCaseChoosePage, QWidget *parent)
    : QWizardPage(parent), choosePage(testCaseChoosePage)
{
    Q_ASSERT(choosePage != nullptr);

    setTitle("数据格式转换");

    auto mainLayout = new QVBoxLayout(this);

    auto lineEditLayout = new QHBoxLayout();
    mainLayout->addLayout(lineEditLayout);

    lineEditLayout->addStretch();

    auto problemNameLabel = new QLabel("题目名称", this);
    problemNameLabel->setToolTip("转换后文件名的前缀");
    lineEditLayout->addWidget(problemNameLabel);

    problemNameEdit = new QLineEdit(this);
    connect(problemNameEdit, &QLineEdit::textChanged, this, &TestCaseConvertPage::updateResult);
    lineEditLayout->addWidget(problemNameEdit);

    lineEditLayout->addStretch();

    auto inputPatternLabel = new QLabel("输入模式", this);
    inputPatternLabel->setToolTip(
        "输入文件名的正则表达式，若输入和输出文件的顺序都是正确的则不需要填写");
    lineEditLayout->addWidget(inputPatternLabel);

    inputPatternEdit = new QLineEdit(this);
    connect(inputPatternEdit, &QLineEdit::textChanged, this, &TestCaseConvertPage::updateResult);
    lineEditLayout->addWidget(inputPatternEdit);

    lineEditLayout->addStretch();

    auto outputPatternLabel = new QLabel("输出模式", this);
    outputPatternLabel->setToolTip(
        "输出文件名的模式，可以用 \"\\1\" "
        "表示输入正则的第一个捕获组，若输入和输出文件的顺序都是正确的则不需要填写");
    lineEditLayout->addWidget(outputPatternLabel);

    outputPatternEdit = new QLineEdit(this);
    connect(outputPatternEdit, &QLineEdit::textChanged, this, &TestCaseConvertPage::updateResult);
    lineEditLayout->addWidget(outputPatternEdit);

    lineEditLayout->addStretch();

    errorLabel = new ErrorLabel(this);
    mainLayout->addWidget(errorLabel);

    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels(
        {"原输入文件", "原输出文件", "转换后输入文件", "转换后输出文件", "子任务"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionMode(QAbstractItemView::ContiguousSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(table);

    subtaskButton = new QPushButton("将选中的测试点设置为一个子任务");
    connect(subtaskButton, &QPushButton::clicked, this, &TestCaseConvertPage::setSubtask);
    connect(table, &QTableWidget::itemSelectionChanged, this,
            &TestCaseConvertPage::updateSubtaskButton);
    mainLayout->addWidget(subtaskButton);
}

void TestCaseConvertPage::initializePage()
{
    inputs = choosePage->inputs();
    outputs = choosePage->outputs();
    Q_ASSERT(!inputs.empty());
    Q_ASSERT(inputs.count() == outputs.count());
    subtaskBegins = QVector<bool>(inputs.size());
    subtaskBegins[0] = true;
    table->setRowCount(inputs.count());

    // guess the problem name by the longest common prefix consisting of letters
    QString name = QFileInfo(inputs[0]).fileName();
    for (auto input : inputs)
    {
        auto baseName = QFileInfo(input).fileName();
        while (!baseName.startsWith(name))
            name.chop(1);
    }
    for (auto output : outputs)
    {
        auto baseName = QFileInfo(output).fileName();
        while (!baseName.startsWith(name))
            name.chop(1);
    }
    QString problemName;
    for (auto c : name)
    {
        if (c.isLetter())
            problemName.push_back(c);
        else
            break;
    }
    problemNameEdit->setText(problemName);
    updateResult();
}

bool TestCaseConvertPage::isComplete() const
{
    return errorLabel->isHidden();
}

int TestCaseConvertPage::nextId() const
{
    return QWizardPage::nextId() + (testCases.size() == 1);
}

QString TestCaseConvertPage::getProblemName() const
{
    return problemNameEdit->text();
}

QVector<QVector<TestCaseConvertPage::TestCase>> TestCaseConvertPage::getTestCases() const
{
    return testCases;
}

void TestCaseConvertPage::updateResult()
{
    errorLabel->hide();

    if (problemNameEdit->text().trimmed().isEmpty())
        errorLabel->showError("题目名称为空");
    else if (problemNameEdit->text().contains(QRegularExpression("\\s")))
        errorLabel->showError("题目名称包含空白字符");

    QStringList pairedOutputs;

    while (!inputPatternEdit->text().isEmpty() || !outputPatternEdit->text().isEmpty())
    {
        QRegularExpression inputRegex(QString("^%1$").arg(inputPatternEdit->text()));
        if (!inputRegex.isValid())
        {
            errorLabel->showError(
                QString("输入模式 [%1] 不是合法的正则表达式。").arg(inputPatternEdit->text()));
            break;
        }

        for (const auto &inputPath : inputs)
        {
            const auto input = QFileInfo(inputPath).fileName();

            if (!inputRegex.match(input).hasMatch())
            {
                errorLabel->showError(QString("输入模式 [%1] 与输入文件名 [%2] 不匹配。")
                                          .arg(inputPatternEdit->text())
                                          .arg(input));
                break;
            }

            auto output = QString(input).replace(inputRegex, outputPatternEdit->text());

            auto find = [](const QStringList &list, const QString &text) {
                for (int i = 0; i < list.count(); ++i)
                {
                    if (QFileInfo(list[i]).fileName() == text)
                    {
                        return i;
                    }
                }
                return -1;
            };

            if (find(outputs, output) == -1)
            {
                errorLabel->showError(
                    QString("输入文件名 [%1] 对应的输出 [%2] 不在输入文件名列表中。")
                        .arg(input)
                        .arg(output));
                break;
            }
            if (find(pairedOutputs, output) != -1)
            {
                errorLabel->showError(
                    QString("输入 [%1] 和输入 [%2] 对应的输出文件名相同，都是 [%3]。")
                        .arg(input)
                        .arg(QFileInfo(inputs[find(pairedOutputs, output)]).fileName())
                        .arg(output));
                break;
            }

            pairedOutputs.push_back(output);
        }

        break;
    }

    if (pairedOutputs.count() != outputs.count())
        pairedOutputs = outputs;

    testCases.clear();

    for (int i = 0; i < inputs.count(); ++i)
    {
        if (subtaskBegins[i])
            testCases.push_back(QVector<TestCase>());
        TestCase testCase = {i + 1, inputs[i], pairedOutputs[i],
                             QString("%1%2.in").arg(getProblemName()).arg(i + 1),
                             QString("%1%2.ans").arg(getProblemName()).arg(i + 1)};
        testCases.back().push_back(testCase);

        auto setText = [this, i](int column, const QString &text) {
            auto item = table->item(i, column);
            if (!item)
            {
                item = new QTableWidgetItem(text);
                table->setItem(i, column, item);
            }
            else
                item->setText(text);
        };

        setText(0, QFileInfo(testCase.originalInput).fileName());
        setText(1, QFileInfo(testCase.originalOutput).fileName());
        setText(2, testCase.convertedInput);
        setText(3, testCase.convertedOutput);
        setText(4, QString::number(testCases.count()));
    }

    emit completeChanged();
}

void TestCaseConvertPage::setSubtask()
{
    const auto items = table->selectedItems();
    if (items.isEmpty())
        return;
    int min = items[0]->row();
    int max = items[0]->row();
    for (int i = 1; i < items.count(); ++i)
    {
        min = qMin(min, items[i]->row());
        max = qMax(max, items[i]->row());
    }
    for (int i = min + 1; i <= max; ++i)
        subtaskBegins[i] = false;
    subtaskBegins[min] = true;
    if (max + 1 < inputs.count())
        subtaskBegins[max + 1] = true;

    updateResult();
}

void TestCaseConvertPage::updateSubtaskButton()
{
    subtaskButton->setDisabled(table->selectedItems().isEmpty());
}

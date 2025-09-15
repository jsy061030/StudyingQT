#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>
#include <QtCore>
#include <QMainWindow>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtGui/QFontMetrics>
#include <QtGui/QIcon>
#include <QDate>

#include "datejsonparser.h"
#include "platform.h"

DateJsonParser::DateJsonParser(QObject* parent) : QObject(parent)
{
}

QString DateJsonParser::extractMonthDay(const QString& date)
{
    // 确保日期格式正确 (YYYY-MM-DD)
    if (date.length() < 10) return "";

    // 提取月份和日期部分 (MM/DD格式)
    QString month = date.mid(5, 2);
    QString day = date.mid(8, 2);
    return month + "/" + day;
}

QStringList DateJsonParser::parseDoc1(const QJsonObject& doc1, const QString& date)
{
    QString monthDay = extractMonthDay(date);
    if (monthDay.isEmpty()) {
        qWarning() << "Invalid date format:" << date;
        return QStringList();
    }

    if (doc1.contains(monthDay)) {
        QJsonValue value = doc1.value(monthDay);
        if (value.isArray()) {
            QJsonArray array = value.toArray();
            QStringList characters;
            for (const QJsonValue& item : array) {
                characters.append(item.toString());
            }
            return characters;
        }
    }

    return QStringList(); // 返回空列表
}

QString DateJsonParser::parseDoc2(const QJsonObject& doc2, const QString& date)
{
    // 遍历所有键值对
    for (auto it = doc2.constBegin(); it != doc2.constEnd(); ++it) {
        QJsonObject entry = it.value().toObject();
        if (entry.contains("signindate") && entry.contains("signinword")) {
            QString signinDate = entry["signindate"].toString();
            // 比较日期部分 (忽略时间)
            if (signinDate.left(10) == date) {
                return entry["signinword"].toString();
            }
        }
    }

    return QString(); // 返回空字符串
}

void DateJsonParser::parseBothDocuments(const QJsonObject& doc1, const QJsonObject& doc2, const QString& date)
{
    QStringList characters = parseDoc1(doc1, date);
    QString signinWord = parseDoc2(doc2, date);

    if (!characters.isEmpty()) {
        qDebug() << "Characters for" << date << ":" << characters.join(", ");
    }
    else {
        qDebug() << "No characters found for" << date;
    }

    if (!signinWord.isEmpty()) {
        qDebug() << "Signin word for" << date << ":" << signinWord;
    }
    else {
        qDebug() << "No signin word found for" << date;
    }
}

class MainFrame : public QMainWindow
{
public:
    MainFrame(QWidget* parent = nullptr) : QMainWindow(parent)
    {
        this->resize(500, 300);
        this->setWindowFlags(Qt::FramelessWindowHint);
        this->setWindowTitle("今日气象");
        this->setWindowIcon(QIcon("icon.png"));
        QWidget* centralWidget = new QWidget(this);
        QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

		QFile sign_in_words("data/sign_in_word.json");
		QFile char_birth("data/char_birthday.json");
        if (!sign_in_words.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open sign_in_word.json for reading.";
            return;
		}
        if (!char_birth.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open char_birthday.json for reading.";
            return;
        }
        QJsonDocument sign_in_word_json = QJsonDocument::fromJson(sign_in_words.readAll());
        QJsonObject sign_in_word_obj = sign_in_word_json.object();
        QJsonDocument char_birth_json = QJsonDocument::fromJson(char_birth.readAll());
        QJsonObject char_birth_obj = char_birth_json.object();

        QDate currentDate = QDate::currentDate();
		QString dateStr = currentDate.toString("yyyy-MM-dd");
		DateJsonParser parser;
        QStringList characters = parser.parseDoc1(sign_in_word_obj, dateStr);
        QString signinWord = parser.parseDoc2(char_birth_obj, dateStr);

        QLabel* label1 = new QLabel(signinWord, centralWidget);
        label1->setAlignment(Qt::AlignCenter);
        QLabel* label2 = new QLabel(dateStr, centralWidget);
        label2->setAlignment(Qt::AlignCenter);
        
        mainLayout->addWidget(label1);
        mainLayout->addWidget(label2);
        mainLayout->setAlignment(Qt::AlignCenter);
        this->setCentralWidget(centralWidget);
    }

};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QString _path = QApplication::applicationDirPath();
    qDebug() << "running on" << PLATFORM << "at" << _path << "\n";
    MainFrame window;

    window.show();
    return app.exec();
}
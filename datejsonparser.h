#ifndef DATE_JSON_PARSER_H
#define DATE_JSON_PARSER_H
#pragma once
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QStringList>
#include <QDate>
#include <QDebug>

class DateJsonParser : public QObject
{
    Q_OBJECT
public:
    explicit DateJsonParser(QObject* parent = nullptr);

    // 解析文档1
    QStringList parseDoc1(const QJsonObject& doc1, const QString& date);

    // 解析文档2
    QString parseDoc2(const QJsonObject& doc2, const QString& date);

    // 组合解析两个文档
    void parseBothDocuments(const QJsonObject& doc1, const QJsonObject& doc2, const QString& date);

private:
    // 从日期字符串中提取月份和日期 (MM/DD格式)
    QString extractMonthDay(const QString& date);
};

#endif // DATE_JSON_PARSER_H
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

    // �����ĵ�1
    QStringList parseDoc1(const QJsonObject& doc1, const QString& date);

    // �����ĵ�2
    QString parseDoc2(const QJsonObject& doc2, const QString& date);

    // ��Ͻ��������ĵ�
    void parseBothDocuments(const QJsonObject& doc1, const QJsonObject& doc2, const QString& date);

private:
    // �������ַ�������ȡ�·ݺ����� (MM/DD��ʽ)
    QString extractMonthDay(const QString& date);
};

#endif // DATE_JSON_PARSER_H
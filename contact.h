#ifndef CONTACT_H
#define CONTACT_H
#include <QString>
#include <QDateEdit>
#include <QRegularExpression>
#include <QFile>
#include <QDataStream>
#include <QTableWidget>
#include <QObject>
#include <QtDebug>

class Contact
{

public:
    Contact(QWidget *parentWidget = nullptr);

    QDate getBirthDate() const {
        return birthDateEdit->date();
    }

    void setBirthDate(const QDate &date) {
        birthDateEdit->setDate(date);
    }

    QString firstName;
    QString lastName;
    QString middleName;
    QString address;
    QDateEdit *birthDateEdit;
    QString email;
    QList<QString> phoneNumbers;

    friend QDataStream &operator<<(QDataStream &out, const Contact &contact);
    friend QDataStream &operator>>(QDataStream &in, Contact &contact);

};



bool isNameValid(const QString &name);
bool isEmailValid(const QString &email);
bool isNumberValid(const QString &number);

QList<Contact> loadContactsFromFile(const QString &filename);
bool saveContactsToFile(const QString &filename, const QList<Contact> &contacts);

#endif // CONTACT_H

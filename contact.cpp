#include "contact.h"

Contact::Contact(QWidget *parentWidget) {
    birthDateEdit = new QDateEdit(parentWidget);
    birthDateEdit->setCalendarPopup(true);
    birthDateEdit->setReadOnly(true); // Отключить возможность редактирования
}

bool isNameValid(const QString &name) {
    QRegularExpression regex("^[A-Za-zА-Яа-яЁё][A-Za-zА-Яа-яЁё0-9\\s\\-]*[A-Za-zА-Яа-яЁё0-9]$");
    return regex.match(name).hasMatch();
}

bool isEmailValid(const QString &email) {
    QRegularExpression regex("^[A-Za-z0-9]+@[A-Za-z0-9]+\\.[A-Za-z0-9]+$");
    return regex.match(email).hasMatch();
}

bool isNumberValid(const QString &number) {
    QRegularExpression regex("^\\+?[\\s]?[0-9]+[\\s\\-]?\\(?[0-9]+\\)?[\\s\\-]?[0-9]+[\\s\\-]?[0-9]+[\\s\\-]?[0-9]+$");
    return regex.match(number).hasMatch();
}

QList<Contact> loadContactsFromFile(const QString &filename) {
    QList<Contact> loadedContacts;

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in >> loadedContacts;
        file.close();
    }

    return loadedContacts;
}

bool saveContactsToFile(const QString &filename, const QList<Contact> &contacts) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << contacts;
        file.close();
        return true;
    }
    return false;
}

QDataStream &operator<<(QDataStream &out, const Contact &contact) {
    out << contact.firstName << contact.lastName << contact.middleName << contact.address;

    // Извлекаем дату из QDateEdit и записываем как QDate
    QDate date = contact.birthDateEdit->date();
    out << date;

    out << contact.email << contact.phoneNumbers;
    return out;
}

QDataStream &operator>>(QDataStream &in, Contact &contact) {
    in >> contact.firstName >> contact.lastName >> contact.middleName >> contact.address;

    // Считываем QDate и устанавливаем его в QDateEdit
    QDate date;
    in >> date;
    contact.birthDateEdit->setDate(date);

    in >> contact.email >> contact.phoneNumbers;
    return in;
}



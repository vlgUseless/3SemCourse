#ifndef PHONEBOOKAPPLICATION_H
#define PHONEBOOKAPPLICATION_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDateEdit>
#include <contact.h>
#include <QCheckBox>

class PhoneBookApplication : public QMainWindow {
    Q_OBJECT

public:
    PhoneBookApplication(QWidget *parent = nullptr);

private slots:
    void handleOKButtonClick();
    void openFile();
    void saveFile();
    void handleComboBoxIndexChanged(int index);
    void handleTableSorting(int logicalIndex);

// Поля
private:
    QWidget *mainWindow;
    QTableWidget *tableWidget;  // Виджет таблицы для отображения контактов

    QLineEdit *firstNameEdit;    // Поле для ввода имени
    QLineEdit *lastNameEdit;     // Поле для ввода фамилии
    QLineEdit *middleNameEdit;  // Поле для ввода отчества
    QLineEdit *addressEdit;     // Поле для ввода адреса
    QLineEdit *emailEdit;       // Поле для ввода адреса электронной почты
    QLineEdit *phoneEdit;       // Поле для ввода номеров телефона
    QDateEdit *dateEdit;        // Виджет для выбора даты рождения
    QCheckBox *checkBox;

    QComboBox *actionComboBox; // Выпадающий список для выбора действия (Добавление, Поиск и т. д.)
    QPushButton *okButton;      // Кнопка "ОК" для подтверждения действия

    // Поля для хранения значений текущего ввода
    QString lastName;
    QString firstName;
    QString middleName;
    QString address;
    QString email;
    QList<QString> phoneNumberList;



    QList<Contact> phoneBook;  // Список контактов
    QList<Contact> searchedContacts; // Результаты поиска

// Методы
private:
    void addContact(const Contact& contact, bool addToPhoneBook);
    //void addSearchResult(const Contact& contact);
    void searchContacts();
    void editContact(int row, Contact &selectedContact);
    void deleteContacts(QList<int> indicesToRemove);
    void clearInput();
    void updateInput();
    //void clearTable();
    void MakeTableItemsUneditable();
    void updatePhoneBookFromTable();

    bool AddDataValidation();
    bool EditDataValidation();
};

#endif // PHONEBOOKAPPLICATION_H

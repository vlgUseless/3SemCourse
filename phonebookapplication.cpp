#include "phonebookapplication.h"
#include <QAction>
#include <QMenuBar>
#include <QHeaderView>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QResizeEvent>

PhoneBookApplication::PhoneBookApplication(QWidget *parent) : QMainWindow(parent) {
    QFont placeholderFont("Montserrat");

    // Главное окно
    mainWindow = new QWidget;
    mainWindow->setWindowTitle("Телефонный справочник");
    mainWindow->setMinimumSize(1000, 500);
    mainWindow->resize(1000, 700);


    QMenuBar *menuBar = new QMenuBar(mainWindow);
    QMenu *fileMenu = menuBar->addMenu("Файл");


    QAction *openAction = new QAction("Открыть", mainWindow);
    QAction *saveAction = new QAction("Сохранить", mainWindow);

    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);

    // QTableWidget
    tableWidget = new QTableWidget(mainWindow);
    tableWidget->setColumnCount(7);
    tableWidget->setSortingEnabled(true);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);


        // Заголовки столбцов
    QStringList headerLabels;
    headerLabels << "Фамилия" << "Имя" << "Отчество" << "Адрес" << "Дата рождения" << "E-mail" << "Телефоны";
    tableWidget->setHorizontalHeaderLabels(headerLabels);

        // Стили

    QFont font = tableWidget->horizontalHeader()->font();
    font.setBold(true);
    tableWidget->horizontalHeader()->setFont(font);

    QString style = QString(        "QTableWidget {"
                                    "    background-image: url(:/cat/background.jpg);"
                                    "    background-position: center center;"
                                    "    background-color: white;"
                                    "    background-size: cover;"
                                    "    background-attachment: fixed;"
                                    "    background-repeat: repeat-x;"
                                    "    font-family: Montserrat;"
                                    "    color: black;"
                                    "}"
                                    "QHeaderView::section {"
                                    "    background-color: gray;"
                                    "    color: white;"
                                    "}");

    tableWidget->setStyleSheet(style);

    // Текстовые поля ввода
    firstNameEdit = new QLineEdit(mainWindow);
    lastNameEdit = new QLineEdit(mainWindow);
    middleNameEdit = new QLineEdit(mainWindow);
    addressEdit = new QLineEdit(mainWindow);
    emailEdit = new QLineEdit(mainWindow);

    dateEdit = new QDateEdit(mainWindow);
    dateEdit->setFixedWidth(100);
    dateEdit->setCalendarPopup(true);
    dateEdit->setStyleSheet("QDateEdit { background-color: lightgray; color: black; }");
    dateEdit->setStyleSheet("QDateEdit { font-family: Montserrat; }");

    phoneEdit = new QLineEdit(mainWindow);
    checkBox = new QCheckBox(mainWindow);

    // Подсказки в текстовых полях
        // Устанавливаем стиль шрифта для подсказок
    lastNameEdit->setFont(placeholderFont);
    firstNameEdit->setFont(placeholderFont);
    middleNameEdit->setFont(placeholderFont);
    addressEdit->setFont(placeholderFont);
    emailEdit->setFont(placeholderFont);
    phoneEdit->setFont(placeholderFont);
        // Устанавливаем подсказки
    lastNameEdit->setPlaceholderText("Фамилия");
    firstNameEdit->setPlaceholderText("Имя");
    middleNameEdit->setPlaceholderText("Отчество");
    addressEdit->setPlaceholderText("Адрес");
    emailEdit->setPlaceholderText("E-mail");
    phoneEdit->setPlaceholderText("Номера телефонов");

    // Выбор действия
    actionComboBox = new QComboBox(mainWindow);
    actionComboBox->addItem("Добавление");
    actionComboBox->addItem("Поиск");
    actionComboBox->addItem("Редактировать");
    actionComboBox->addItem("Удаление");
        // Стиль и шрифт для QComboBox с выравниванием текста по центру
    actionComboBox->setStyleSheet("font-family: Montserrat; background-color: #EDEDED; text-align: center;");

    // Кнопка подтверждения действия
    okButton = new QPushButton("ОК", mainWindow);
        // Стиль и шрифт для кнопки "ОК"
    okButton->setStyleSheet("font-family: Montserrat; background-color: #424242; color: white;");
    okButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    // Подключение сигналов к слотам
    connect(okButton, &QPushButton::clicked, this, &PhoneBookApplication::handleOKButtonClick);
    connect(openAction, &QAction::triggered, this, &PhoneBookApplication::openFile);
    connect(saveAction, &QAction::triggered, this, &PhoneBookApplication::saveFile);
    connect(actionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PhoneBookApplication::handleComboBoxIndexChanged);
    connect(tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(handleTableSorting(int)));

    // Создайте горизонтальный виджет для элементов управления вводом
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(lastNameEdit);
    inputLayout->addWidget(firstNameEdit);
    inputLayout->addWidget(middleNameEdit);
    inputLayout->addWidget(addressEdit);
    inputLayout->addWidget(dateEdit);
    inputLayout->addWidget(checkBox);
    inputLayout->addWidget(emailEdit);
    inputLayout->addWidget(phoneEdit);
    inputLayout->addWidget(actionComboBox);
    inputLayout->addWidget(okButton);

    // Разместите виджет QTableWidget и элементы ввода в главном окне
    QVBoxLayout *layout = new QVBoxLayout(mainWindow);
    layout->addLayout(inputLayout);
    layout->addWidget(tableWidget);
    mainWindow->setLayout(layout);
    mainWindow->layout()->setMenuBar(menuBar);

    // Установите политику изменения размера для каждого столбца
    for (int i = 0; i < tableWidget->columnCount(); ++i) {
        tableWidget->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignHCenter);
        tableWidget->horizontalHeaderItem(i)->setSizeHint(QSize(100, 30)); // Установите начальную ширину столбцов
    }

    setCentralWidget(mainWindow);
}

void PhoneBookApplication::openFile() {
    QString filePath = QFileDialog::getOpenFileName(mainWindow, "Выберите файл для загрузки", QDir::homePath(), "Файл с контактами (*.contacts)");
    if (!filePath.isEmpty()) {
        // Загрузить контакты из файла
        QList<Contact> loadedContacts = loadContactsFromFile(filePath);

        // Полностью очищаем старый список и удаляем старые даты рождения
        for (const auto& contact: phoneBook) {
            delete contact.birthDateEdit;
        }
        phoneBook.clear();
        tableWidget->setRowCount(0);

        tableWidget->setSortingEnabled(false);
        // Заполнить таблицу данными из загруженных контактов
        // Вызов
        for (Contact &contact : loadedContacts) {
            addContact(contact, true);
        }
        MakeTableItemsUneditable();
        tableWidget->setSortingEnabled(true);
    }
}

void PhoneBookApplication::saveFile() {
    QString filePath = QFileDialog::getSaveFileName(mainWindow, "Выберите файл для сохранения", QDir::homePath(), "Файл с контактами (*.contacts)");
    if (!filePath.isEmpty()) {
        bool saved = saveContactsToFile(filePath, phoneBook);
        if (saved) {
            QMessageBox::information(mainWindow, "Успешное сохранение", "Контакты успешно сохранены в файл.");
        } else {
            QMessageBox::critical(mainWindow, "Ошибка сохранения", "Не удалось сохранить контакты в файл.");
        }
    }
}

void PhoneBookApplication::handleOKButtonClick() {
    QString selectedAction = actionComboBox->currentText();

    // Считываем текст со всех полей и избавляемся от лишних пробелов
    updateInput();

    if (selectedAction == "Добавление") {
        if (AddDataValidation()) {
            Contact contact;
            contact.lastName = lastName;
            contact.firstName = firstName;
            contact.middleName = middleName;
            contact.address = address;
            contact.email = email;
            contact.phoneNumbers = phoneNumberList;

            contact.birthDateEdit->setDate(dateEdit->date());

            //Вызов
            addContact(contact,true);
            MakeTableItemsUneditable();

            // Очисщаем текстовые поля
            clearInput();
        }

    }
    else if (selectedAction == "Поиск") {
        searchContacts();
        clearInput();
    }
    else if (selectedAction == "Редактировать") {
        if (tableWidget->currentRow() >= 0) {
            int selectedRow = tableWidget->currentRow();
            if (selectedRow < phoneBook.size()) {

                if (EditDataValidation()) {
                    editContact(selectedRow, phoneBook[selectedRow]);
                    // Очисщаем текстовые поля
                    clearInput();
                }
            }
        }
        MakeTableItemsUneditable();
    }

    else if (selectedAction == "Удаление") {
        QList<int> indicesToRemove;
        updateInput();
        // Проходимся по всем контактам и отмечаем индексы тех, которые нужно удалить
        for (int i = 0; i < phoneBook.size(); i++) {
            Contact contact = phoneBook[i];
            bool matchesAllCriteria = true;

            if (!lastName.isEmpty() && !contact.lastName.contains(lastName, Qt::CaseInsensitive)) {
                matchesAllCriteria = false;
            }
            if (!firstName.isEmpty() && !contact.firstName.contains(firstName, Qt::CaseInsensitive)) {
                matchesAllCriteria = false;
            }
            if (!middleName.isEmpty() && !contact.middleName.contains(middleName, Qt::CaseInsensitive)) {
                matchesAllCriteria = false;
            }
            if (!address.isEmpty() && !contact.address.contains(address, Qt::CaseInsensitive)) {
                matchesAllCriteria = false;
            }
            if (!email.isEmpty() && !contact.email.contains(email, Qt::CaseInsensitive)) {
                matchesAllCriteria = false;
            }
            if (checkBox->isChecked() && !dateEdit->date().isNull() && contact.getBirthDate() != dateEdit->date()) {
                matchesAllCriteria = false;
            }
            if (!phoneNumberList.isEmpty()) {
                bool hasMatchingPhoneNumber = false;
                for (auto& searchedPhoneNumber : phoneNumberList) {
                    if (contact.phoneNumbers.contains(searchedPhoneNumber)) {
                        hasMatchingPhoneNumber = true;
                        break;
                    }
                }
                if (!hasMatchingPhoneNumber) {
                    matchesAllCriteria = false;
                }
            }
            if (matchesAllCriteria) {
                indicesToRemove.append(i);
            }

        }
        if (!indicesToRemove.isEmpty()) {
            deleteContacts(indicesToRemove);

            // Выведите диалоговое окно с сообщением об успешном удалении
            QMessageBox messageBox;
            messageBox.setText("Контакты успешно удалены!");
            messageBox.setIcon(QMessageBox::Information);
            messageBox.exec();
        }
        else {
            QMessageBox messageBox;
            messageBox.setText("Нет контактов для удаления.");
            messageBox.setIcon(QMessageBox::Information);
            messageBox.exec();
        }

        // Очисщаем текстовые поля
        clearInput();
    }

}

void PhoneBookApplication::handleComboBoxIndexChanged(int index) {
    // Получите текст выбранного действия
    QString selectedAction = actionComboBox->itemText(index);
    tableWidget->setSortingEnabled(false);

    // Очистите результаты поиска и освободите ресурсы
    searchedContacts.clear();

    if (selectedAction != "Поиск") {
        // Если выбрано другое действие, отобразите все контакты из phoneBook
        tableWidget->setRowCount(0);
        // Вызов
        for (const Contact &contact : phoneBook) {
            addContact(contact, false);
        }
    }

    // Обновите данные из таблицы в phoneBook
    updatePhoneBookFromTable();
    MakeTableItemsUneditable();

    tableWidget->setSortingEnabled(true);
}

void PhoneBookApplication::addContact(const Contact& contact, bool addToPhoneBook) {

    // Создайте QTableWidgetItem для остальных полей
    QTableWidgetItem *firstNameItem = new QTableWidgetItem(contact.firstName);
    QTableWidgetItem *lastNameItem = new QTableWidgetItem(contact.lastName);
    QTableWidgetItem *middleNameItem = new QTableWidgetItem(contact.middleName);
    QTableWidgetItem *addressItem = new QTableWidgetItem(contact.address);
    QTableWidgetItem *birthDateItem = new QTableWidgetItem(contact.birthDateEdit->date().toString("dd.MM.yyyy"));
    QTableWidgetItem *emailItem = new QTableWidgetItem(contact.email);
    QTableWidgetItem *phoneNumbersItem = new QTableWidgetItem(contact.phoneNumbers.join(", "));

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);

    // Установите QTableWidgetItem в соответствующие столбцы
    tableWidget->setItem(row, 0, lastNameItem);
    tableWidget->setItem(row, 1, firstNameItem);
    tableWidget->setItem(row, 2, middleNameItem);
    tableWidget->setItem(row, 3, addressItem);
    tableWidget->setItem(row, 4, birthDateItem);
    tableWidget->setItem(row, 5, emailItem);
    tableWidget->setItem(row, 6, phoneNumbersItem);

    if (addToPhoneBook) {
        phoneBook.append(contact);
    }
}

/*void PhoneBookApplication::addSearchResult(const Contact& contact) {

    // Создайте QTableWidgetItem для остальных полей
    QTableWidgetItem *firstNameItem = new QTableWidgetItem(contact.firstName);
    QTableWidgetItem *lastNameItem = new QTableWidgetItem(contact.lastName);
    QTableWidgetItem *middleNameItem = new QTableWidgetItem(contact.middleName);
    QTableWidgetItem *addressItem = new QTableWidgetItem(contact.address);
    QTableWidgetItem *birthDateItem = new QTableWidgetItem(contact.birthDateEdit->date().toString("dd.MM.yyyy"));
    QTableWidgetItem *emailItem = new QTableWidgetItem(contact.email);
    QTableWidgetItem *phoneNumbersItem = new QTableWidgetItem(contact.phoneNumbers.join(", "));

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);

    // Установите QTableWidgetItem в соответствующие столбцы
    tableWidget->setItem(row, 0, lastNameItem);
    tableWidget->setItem(row, 1, firstNameItem);
    tableWidget->setItem(row, 2, middleNameItem);
    tableWidget->setItem(row, 3, addressItem);
    tableWidget->setItem(row, 4, birthDateItem);
    tableWidget->setItem(row, 5, emailItem);
    tableWidget->setItem(row, 6, phoneNumbersItem);
}
*/
void PhoneBookApplication::searchContacts() {
    // Очищаем предыдущий поиск
    searchedContacts.clear();

    updateInput();
    for (const Contact &contact : phoneBook) {
        bool matchesAllCriteria = true;

        if (!lastName.isEmpty() && !contact.lastName.contains(lastName, Qt::CaseInsensitive)) {
            matchesAllCriteria = false;
        }
        if (!firstName.isEmpty() && !contact.firstName.contains(firstName, Qt::CaseInsensitive)) {
            matchesAllCriteria = false;
        }
        if (!middleName.isEmpty() && !contact.middleName.contains(middleName, Qt::CaseInsensitive)) {
            matchesAllCriteria = false;
        }
        if (!address.isEmpty() && !contact.address.contains(address, Qt::CaseInsensitive)) {
            matchesAllCriteria = false;
        }
        if (!email.isEmpty() && !contact.email.contains(email, Qt::CaseInsensitive)) {
            matchesAllCriteria = false;
        }
        if (checkBox->isChecked() and !dateEdit->date().isNull() && contact.getBirthDate() != dateEdit->date()) {
            matchesAllCriteria = false;
        }
        if (!phoneNumberList.isEmpty() and phoneNumberList[0] != "") {
            bool hasMatchingPhoneNumber = false;
            for (auto& searchedPhoneNumber : phoneNumberList) {
                if (contact.phoneNumbers.contains(searchedPhoneNumber)) {
                    hasMatchingPhoneNumber = true;
                    break;
                }
            }
            if (!hasMatchingPhoneNumber) {
                matchesAllCriteria = false;
            }
        }
        if (matchesAllCriteria) {
            searchedContacts.append(contact);
        }

    }
    // Очищаем таблицу
    tableWidget->setRowCount(0);
    for (int i = 0; i < searchedContacts.size(); i++) {
        addContact(searchedContacts[i], false);
    }
}

void PhoneBookApplication::editContact(int row, Contact &selectedContact) {
    // Считываем значения полей ввода
    updateInput();
    //currentItem = tableWidget->currentItem();

    // Обновляем данные выделенного контакта, только если поле не пустое
    if (!lastName.isEmpty()) {
        selectedContact.lastName = lastName;
    }
    if (!firstName.isEmpty()) {
        selectedContact.firstName = firstName;
    }
    if (!middleName.isEmpty()) {
        selectedContact.middleName = middleName;
    }
    if (!address.isEmpty()) {
        selectedContact.address = address;
    }
    if (checkBox->isChecked() and dateEdit->date() != QDate()) {
        selectedContact.birthDateEdit->setDate(dateEdit->date());
    }
    if (!email.isEmpty()) {
        selectedContact.email = email;
    }
    if (!phoneNumberList.isEmpty()) {
        selectedContact.phoneNumbers = phoneNumberList;
    }

    tableWidget->setSortingEnabled(false);

    // Обновляем данные в таблице только для непустых полей
    if (!lastName.isEmpty()) {
        QTableWidgetItem *lastNameItem = new QTableWidgetItem(selectedContact.lastName);
        tableWidget->setItem(row, 0, lastNameItem);
    }
    if (!firstName.isEmpty()) {
        QTableWidgetItem *firstNameItem = new QTableWidgetItem(selectedContact.firstName);
        tableWidget->setItem(row, 1, firstNameItem);
    }
    if (!middleName.isEmpty()) {
        QTableWidgetItem *middleNameItem = new QTableWidgetItem(selectedContact.middleName);
        tableWidget->setItem(row, 2, middleNameItem);
    }
    if (!address.isEmpty()) {
        QTableWidgetItem *addressItem = new QTableWidgetItem(selectedContact.address);
        tableWidget->setItem(row, 3, addressItem);
    }
    if (dateEdit->date() != QDate()) {
        QTableWidgetItem *birthDateItem = new QTableWidgetItem(selectedContact.birthDateEdit->date().toString("dd.MM.yyyy"));
        tableWidget->setItem(row, 4, birthDateItem);
    }
    if (!email.isEmpty()) {
        QTableWidgetItem *emailItem = new QTableWidgetItem(selectedContact.email);
        tableWidget->setItem(row, 5, emailItem);
    }
    if (!phoneNumberList.isEmpty()) {
        QTableWidgetItem *phoneNumbersItem = new QTableWidgetItem(selectedContact.phoneNumbers.join(", "));
        tableWidget->setItem(row, 6, phoneNumbersItem);
    }

    tableWidget->setSortingEnabled(true);
}

void PhoneBookApplication::deleteContacts(QList<int> indicesToRemove) {
    // Удалите элементы из phoneBook в обратном порядке, чтобы избежать смещения индексов
    for (int i = indicesToRemove.size() - 1; i >= 0; i--) {
        int index = indicesToRemove[i];
        delete phoneBook[index].birthDateEdit;
        phoneBook.removeAt(index);
    }

    // Удалите строки из tableWidget
    for (int i = indicesToRemove.size() - 1; i >= 0; i--) {
        int index = indicesToRemove[i];
        tableWidget->removeRow(index);
    }
}

void PhoneBookApplication::clearInput() {
    // Очисщаем текстовые поля
    firstNameEdit->clear();
    lastNameEdit->clear();
    middleNameEdit->clear();
    addressEdit->clear();
    dateEdit->setDate(QDate(2000,1,1));
    emailEdit->clear();
    phoneEdit->clear();
}

void PhoneBookApplication::updateInput() {
    lastName = lastNameEdit->text().trimmed();
    firstName = firstNameEdit->text().trimmed();
    middleName = middleNameEdit->text().trimmed();
    address = addressEdit->text().trimmed();
    email = emailEdit->text().trimmed();
    if (!phoneEdit->text().isEmpty()) {
        phoneNumberList = phoneEdit->text().trimmed().split(QRegExp("[,;]+"));
    }
    else {
        phoneNumberList.clear();
    }
}

/*void PhoneBookApplication::clearTable() {
    tableWidget->setRowCount(0);
}
*/

void PhoneBookApplication::MakeTableItemsUneditable() {
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = tableWidget->item(row, col);
            if (item) {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }
        }
    }
}

bool PhoneBookApplication::AddDataValidation() {
    int incorrectPhoneNumber = -1; // Своеобразный флажок для проверки корректности номеров телефона
    for (int i = 0; i < phoneNumberList.size(); i++) {
        if (!isNumberValid(phoneNumberList[i])) {
            incorrectPhoneNumber = i;
        }
    }

    // Проверка корректности введённых данных
    if(!isNameValid(lastName)) {
        QMessageBox::critical(mainWindow, "Ошибка", "Пожалуйста, введите корректую фамилию.");
        lastNameEdit->setFocus();
        return false;
    }
    else if (!isNameValid(firstName)) {
        QMessageBox::critical(mainWindow, "Ошибка", "Пожалуйста, введите корректное имя.");
        firstNameEdit->setFocus();
        return false;
    }
    else if (!isNameValid(middleName)) {
        QMessageBox::critical(mainWindow, "Ошибка", "Пожалуйста, введите корректное отчество.");
        middleNameEdit->setFocus();
        return false;
    }
    else if (dateEdit->date() > QDate::currentDate()){
        QMessageBox::critical(mainWindow, "Ошибка", "Пожалуйста, введите корректную дату рождения.");
        dateEdit->setFocus();
        return false;
    }
    else if (!isEmailValid(email)){
        QMessageBox::critical(mainWindow, "Ошибка", "Пожалуйста, введите корректный e-mail.");
        emailEdit->setFocus();
        return false;
    }
    else if (incorrectPhoneNumber != -1){
        QMessageBox::critical(mainWindow, "Ошибка", "Пожалуйста, введите корректный(-е) номер(-а) телефона(-ов).");
        phoneEdit->setFocus();
        return false;
    }
    return true;
}

bool PhoneBookApplication::EditDataValidation() {
    int incorrectPhoneNumber = -1; // Своеобразный флажок для проверки корректности номеров телефона
    for (int i = 0; i < phoneNumberList.size(); i++) {
        if (!isNumberValid(phoneNumberList[i])) {
            incorrectPhoneNumber = i;
        }
    }

    // Проверка корректности введённых данных
    if(!lastName.isEmpty() and !isNameValid(lastName)) {
        QMessageBox::critical(mainWindow, "Ошибка", "Пожалуйста, введите корректую фамилию.");
        lastNameEdit->setFocus();
        return false;
    }
    else if (!firstName.isEmpty() and !isNameValid(firstName)) {
        QMessageBox::critical(mainWindow, "Ошибка", "Пожалуйста, введите корректное имя.");
        firstNameEdit->setFocus();
        return false;
    }
    else if (!middleName.isEmpty() and !isNameValid(middleName)) {
        QMessageBox::critical(mainWindow, "Ошибка", "Пожалуйста, введите корректное отчество.");
        middleNameEdit->setFocus();
        return false;
    }
    else if (dateEdit->date() > QDate::currentDate()){
        QMessageBox::critical(mainWindow, "Ошибка", "Пожалуйста, введите корректную дату рождения.");
        dateEdit->setFocus();
        return false;
    }
    else if (!email.isEmpty() and !isEmailValid(email)){
        QMessageBox::critical(mainWindow, "Ошибка", "Пожалуйста, введите корректный e-mail.");
        emailEdit->setFocus();
        return false;
    }
    else if (phoneNumberList.isEmpty() and incorrectPhoneNumber != -1){
        QMessageBox::critical(mainWindow, "Ошибка", "Пожалуйста, введите корректный(-е) номер(-а) телефона(-ов).");
        phoneEdit->setFocus();
        return false;
    }
    return true;
}

void PhoneBookApplication::updatePhoneBookFromTable() {
    qDebug() << "Updating phone book from table...";

    int rowCount = tableWidget->rowCount();
    qDebug() << "Row count in table:" << rowCount;

    if (rowCount == 0) {
        qDebug() << "Table is empty. Skipping update.";
        return;
    }
    phoneBook.clear(); // Очищаем phoneBook

    for (int row = 0; row < rowCount; ++row) {
        Contact contact;
        contact.lastName = tableWidget->item(row, 0)->text();
        contact.firstName = tableWidget->item(row, 1)->text();
        contact.middleName = tableWidget->item(row, 2)->text();
        contact.address = tableWidget->item(row, 3)->text();


        // Преобразуем строку в объект QDate
        QDate birthDate = QDate::fromString(tableWidget->item(row,4)->text(), "dd.MM.yyyy");
        // Устанавливаем дату в объекте контакта
        contact.birthDateEdit->setDate(birthDate);


        contact.email = tableWidget->item(row, 5)->text();
        QTableWidgetItem *phoneNumbersItem = tableWidget->item(row, 6);
        contact.phoneNumbers = phoneNumbersItem->text().split(", ");

        phoneBook.append(contact);
    }
    qDebug() << "Updated phone book size:" << phoneBook.size();
}

void PhoneBookApplication::handleTableSorting(int logicalIndex) {
    // Обновляем данные в phoneBook
    updatePhoneBookFromTable();
}

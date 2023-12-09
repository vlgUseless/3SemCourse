#include <contact.h>
#include "phonebookapplication.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PhoneBookApplication phoneBookApp;
    phoneBookApp.setWindowTitle("Телефонный справочник");
    phoneBookApp.show();
    return app.exec();
}

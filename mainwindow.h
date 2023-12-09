#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent)
    {
        // Создаем виджет для центральной области
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        // Создаем менеджер размещения для центрального виджета
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);

        // Создайте остальные элементы интерфейса и добавьте их в layout

        // Пример: добавление кнопки "ОК"
        QPushButton *okButton = new QPushButton("ОК", centralWidget);
        layout->addWidget(okButton);

        // Установите layout для центрального виджета
        centralWidget->setLayout(layout);
    }
};

#endif // MAINWINDOW_H

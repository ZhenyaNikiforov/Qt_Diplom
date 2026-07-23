#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->wordInput->setPlaceholderText("Пишем искомые слова");

    ui->host->setPlaceholderText("Хост");
    ui->port->setPlaceholderText("Порт");
    ui->user->setPlaceholderText("Пользователь");
    ui->password->setPlaceholderText("Пароль");
    ui->dataBase->setPlaceholderText("База данных");

    ui->wordSearch->setEnabled(false);
    ui->allWordsSearch->setEnabled(false);

    connect(&(this->dataBase), &DataBaseManager::yesConnect, this, [this]() {
        ui->connectDB->setText("Отключимся?");

        ui->wordSearch->setEnabled(true);
        ui->allWordsSearch->setEnabled(true);

        ui->host->setEnabled(false);
        ui->port->setEnabled(false);
        ui->user->setEnabled(false);
        ui->password->setEnabled(false);
        ui->dataBase->setEnabled(false);
    });

    connect(&(this->dataBase), &DataBaseManager::yesDisconnect, this, [this]() {
        ui->connectDB->setText("Подключимся?");

        ui->wordSearch->setEnabled(false);
        ui->allWordsSearch->setEnabled(false);

        ui->host->setEnabled(true);
        ui->port->setEnabled(true);
        ui->user->setEnabled(true);
        ui->password->setEnabled(true);
        ui->dataBase->setEnabled(true);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectDB_clicked()
{
    QString host = ui->host->text();
    QString port = ui->port->text();
    QString user = ui->user->text();
    QString password = ui->password->text();
    QString dataBase = ui->dataBase->text();

    this->dataBase.connectDB(host, port, user, password, dataBase);
}

void MainWindow::on_allWordsSearch_clicked()
{
    this->dataBase.allWordsSearch(ui->allWordsView);
}

void MainWindow::on_wordSearch_clicked()
{
    QString wordInput = ui->wordInput->text();
    this->dataBase.wordSearch(ui->privateWordsView, wordInput);
}

void MainWindow::on_allWordsClean_clicked()
{
    this->dataBase.allWordsClean(ui->allWordsView);
}

void MainWindow::on_wordClean_clicked()
{
    this->dataBase.wordClean(ui->privateWordsView);
}
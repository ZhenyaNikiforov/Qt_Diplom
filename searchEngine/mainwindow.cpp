#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->wordInput->setPlaceholderText("Пишем искомые слова");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_clicked()
{
    QString host = ui->host->text();
    QString port = ui->port->text();
    QString user = ui->user->text();
    QString password = ui->password->text();
    QString dataBase = ui->dataBase->text();

    this->dataBase.connect(host, port, user, password, dataBase);
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
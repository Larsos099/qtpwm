#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <QListWidget>
#include<QDebug>
#include <array>
#include <QInputDialog>
#include <QMessageBox>
#include <QRandomGenerator>
#include <random>
#include <QCryptographicHash>
#include <string>
#include <iostream>
#include <QString>
#include <openssl/ssl.h>
#define KEYLENGTH 24
QString homedir = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->services->setSelectionBehavior( QAbstractItemView::SelectItems );
    ui->services->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->passwords->setSelectionBehavior( QAbstractItemView::SelectItems );
    ui->passwords->setSelectionMode(QAbstractItemView::SingleSelection);
#ifdef Q_OS_WIN
#include <windows.h>
    char userProfile[MAX_PATH];
    if (GetEnvironmentVariable("USERPROFILE", userProfile, sizeof(userProfile))) {
        homedir = QString::fromUtf8(userProfile);
    }
#endif

#ifdef Q_OS_MACX
    homedir = QString::fromUtf8(qgetenv("HOME"));
#endif

#ifdef Q_OS_LINUX
    homedir = QString::fromUtf8(qgetenv("HOME"));
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_open_file_triggered()
{
    QListWidget* services = ui->services;
    QListWidget* passwords = ui->passwords;
    QString decryptedData;
    services->clear();
    passwords->clear();
    QStringList contentList;
    QFileDialog explorer;
    QString fln = explorer.getOpenFileName(this, "Wähle eine Speicherdatei aus", homedir);
                  qInfo() << fln;
    QFile fl(fln);
                  fl.open(QIODevice::ReadWrite | QIODevice::Text);
                  QTextStream s(&fl);
                  decryptedData = s.readAll();
    qInfo() << decryptedData;
    contentList = decryptedData.split("PW;");
    qInfo() << contentList;
    decryptedData = contentList.join("");
    qInfo() << contentList;
    contentList = decryptedData.split(";SV;");
    qInfo() << contentList;
    contentList = decryptedData.split(";");
    qInfo() << contentList;
    contentList.removeAll("SV");





    for(int i = 0; i < contentList.count()/2; ++i){

        passwords->addItem(contentList[i]);


    }
    for(int i = 0; i < passwords->count(); ++i){
        contentList.removeAll(passwords->item(i)->text());
        qInfo() << contentList;
    }
    for(int i = 0; i< contentList.count(); ++i){
        services->addItem(contentList[i]);
    }


}


void MainWindow::on_close_triggered()
{
    QApplication::quit();
}



void MainWindow::on_save_file_triggered()
{
    QListWidget* services = ui->services;
    std::string Sunpadded_password;
    QString Qunpadded_password;
    QString unencrypted;
    std::string encData;
    QListWidget* passwords = ui->passwords;
    QString serviceItems[services->count()];
    QString passwordItems[passwords->count()];
    for(int i = 0; i < services->count(); ++i){
        serviceItems[i] = services->item(i)->text();
    }
    for(int i = 0; i < passwords->count(); ++i){
        passwordItems[i] = passwords->item(i)->text();
    }
    QFileDialog explorer;
    QFile savefile = explorer.getSaveFileName(this, "Datei Speichern", homedir, "Password-Manager Files (*.pwm)");
    savefile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QTextStream streamout(&savefile);
    streamout << "PW;";
    for(int i = 0; i < passwords->count(); ++i){
        if( i < passwords->count()-1){
            streamout << passwordItems[i] << ";";
        }
        else{
            streamout << passwordItems[i];
        }

    }
    streamout << ";" << "SV;";

    for(int i = 0; i < services->count(); ++i){
        if( i < services->count()-1){
            streamout << serviceItems[i] << ";";
        }
        else{
            streamout << serviceItems[i];
    }
}


}
/*
    QListWidget* services = ui->services;
    QListWidget* passwords = ui->passwords;
    QString serviceItems[services->count()];
    QString passwordItems[passwords->count()];
    for(int i = 0; i < services->count(); ++i){
        serviceItems[i] = services->item(i)->text();
    }
    for(int i = 0; i < passwords->count(); ++i){
        passwordItems[i] = passwords->item(i)->text();
    }

    return passwordItems, serviceItems;
*/


QString GetRandomPassword(int length = 10){
    srand(QTime::currentTime().msec());
    QString capitalLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString smallLetters = "abcdefghijklmnopqrstuvwxyz";
    QString numbers = "1234567890";
    QString specials = "!@#$&_";
    QString possibleCharacters = capitalLetters + smallLetters + numbers + specials;
    QString randomString;
    for(int i=0; i<length; ++i)
    {
        int index = arc4random() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}


void MainWindow::on_newentry_clicked()
{
    int pwlength;
    QString password;
    QListWidget* services = ui->services;
    QListWidget* passwords = ui->passwords;
    QInputDialog inputdialog;
    QMessageBox msgbox;
    QString service = inputdialog.getText(this, "Service", "Von wo?");
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Passwort Generieren", "Passwort Generieren?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes){
        pwlength = inputdialog.getInt(this, "Wie lang soll es sein?", "Länge des Passworts");
        password = GetRandomPassword(pwlength);


    }
    if (reply == QMessageBox::No){
        password = inputdialog.getText(this, "Passwort", "Welches Passwort hast / willst du bei " + service);
    }
    qInfo() << password;
    services->addItem(service);
    passwords->addItem(password);
}


void MainWindow::on_changeenty_clicked()
{
    QListWidget* services = ui->services;
    QListWidget* passwords = ui->passwords;
    QListWidgetItem* selected;
    QString content;
    QString newc = "";
    int row;
    if(services->selectedItems().size() != 0){
        selected = services->currentItem();
        content = selected->text();
        qInfo() << content;
        QInputDialog inputdiag;
        newc = inputdiag.getText(this, "SERVICE ÄNDERN", "Bitte den neuen SERVICE eingeben. (ORIGINAL=" + content + ")");
        selected->setText(newc);
        selected = NULL;
        delete selected;
    }
    else if(passwords->selectedItems().size() != 0){
        selected = passwords->currentItem();
        content = selected->text();
        row = selected->listWidget()->currentRow();
        qInfo() << row;
        QInputDialog inputdiag;
        newc = inputdiag.getText(this, "PASSWORT ÄNDERN", "Bitte das neue Passwort für " + services->item(row)->text());
        selected->setText(newc);
        selected = nullptr;
        delete selected;

    }

}


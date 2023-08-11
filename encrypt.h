#ifndef ENCRYPT_H
#define ENCRYPT_H
#include "QtWidgets/qmessagebox.h"
#define klen 24
#include <mainwindow.h>
#include <iostream>
#include <string.h>
#include <CryptoPP/rijndael.h>
#include <CryptoPP/cryptlib.h>
#include <CryptoPP/files.h>
#include <CryptoPP/modes.h>
#include <CryptoPP/osrng.h>
#include <CryptoPP/hex.h>
using namespace CryptoPP;
QString encrypt192(QString key, QString data){
    try {
        AutoSeededRandomPool srp;
        SecByteBlock iv(AES::BLOCKSIZE);
        srp.GenerateBlock(iv, iv.size());
        QByteArray keyAsBytes = key.toUtf8();
        SecByteBlock keyAsSecByteBlock(reinterpret_cast<const byte*>(keyAsBytes.data()), keyAsBytes.size());
        CBC_Mode<AES>::Encryption e;
        e.SetKeyWithIV(keyAsSecByteBlock, klen, iv);
        SecByteBlock encrypted(data.size());
        QByteArray dataAsBytes = data.toUtf8();
        e.ProcessData(encrypted, reinterpret_cast<const byte*>(dataAsBytes.data()), dataAsBytes.size());
        QByteArray combined;
        combined.append(iv);
        combined.append(encrypted);
        QString final;
        std::string prefinal;
        HexEncoder encoder(new StringSink(prefinal));
        encoder.Put(reinterpret_cast<const byte*>(combined.data()), combined.size());
        encoder.MessageEnd();
        final = QString::fromStdString(prefinal);
        return final;
    } catch (...) {
        qWarning() << "Something failed.";
        QMessageBox qmsgb;
        qmsgb.setText("Something in the Encryption Process Failed.");
        return "";
    }



}
QString decrypt192(QString data, QString key){
    QString final = "";
    QString ivString = data.left(AES::BLOCKSIZE);
    data.remove(ivString);
    QByteArray dataAsByteArray = data.toUtf8();
    QByteArray ivAsByteArray = ivString.toUtf8();
    QByteArray keyAsByteArray = key.toUtf8();
    SecByteBlock dataAsSecByteBlock(reinterpret_cast<const byte*>(dataAsByteArray.data()), dataAsByteArray.size());
    SecByteBlock keyAsSecByteBlock(reinterpret_cast<const byte*>(keyAsByteArray.data()), keyAsByteArray.size());
    SecByteBlock iv(reinterpret_cast<const byte*>(ivAsByteArray.data()), ivAsByteArray.size());
    CBC_Mode<AES>::Decryption d;
    d.SetKeyWithIV(keyAsSecByteBlock, klen, iv);
    SecByteBlock f(data.size());
    d.ProcessData(f, dataAsSecByteBlock, dataAsByteArray.size());
    QByteArray prefinal(reinterpret_cast<const char*>(f.data()), f.size());
    std::string stage1;

        return final;
}

#endif // ENCRYPT_H

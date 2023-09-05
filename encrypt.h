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
QString encrypt192(QString key, QString data) {
    try {
        AutoSeededRandomPool srp;
        SecByteBlock iv(AES::BLOCKSIZE);
        srp.GenerateBlock(iv, iv.size());

        QByteArray keyAsBytes = key.toUtf8();
        if (keyAsBytes.size() < 24) {
            // Pad the key with zeros to reach the required size
            keyAsBytes.resize(24, 0);
        }

        // Initialize the key using the byte data
        SecByteBlock keyAsSecByteBlock;
        keyAsSecByteBlock.Assign(reinterpret_cast<const byte*>(keyAsBytes.data()), keyAsBytes.size());

        CBC_Mode<AES>::Encryption e;
        e.SetKeyWithIV(keyAsSecByteBlock, keyAsSecByteBlock.size(), iv);
        SecByteBlock encrypted(data.size());
        QByteArray dataAsBytes = data.toUtf8();
        e.ProcessData(encrypted, reinterpret_cast<const byte*>(dataAsBytes.data()), dataAsBytes.size());
        QByteArray combined;
        combined.append(reinterpret_cast<const char*>(iv.data()), AES::BLOCKSIZE);
        combined.append(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
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

QString decrypt192(QString data, QString key) {
    QString final;

    if(key.toUtf8().size() < 24){
        key.toUtf8().resize(24, 0);
    }
    QByteArray keyAsBytes = key.toUtf8();
    SecByteBlock keyAsSecByteBlock(reinterpret_cast<const byte*>(keyAsBytes.constData()), keyAsBytes.size());
    CBC_Mode<AES>::Decryption d;
    std::string unhexed;
    HexDecoder hexd(new StringSink(unhexed));
    hexd.Put(reinterpret_cast<const byte*>(data.toUtf8().data()), data.toUtf8().size());
    hexd.MessageEnd();
    QString IVasString = QString::fromStdString(unhexed).left(AES::BLOCKSIZE);
    SecByteBlock IV(reinterpret_cast<const byte*>(IVasString.toUtf8().data()), IVasString.toUtf8().size());
    d.SetKeyWithIV(keyAsSecByteBlock, klen, IV);
    QString stage1 = QString::fromStdString(unhexed).remove(IVasString);
    SecByteBlock decrypted(stage1.toUtf8().size());
    d.ProcessData(decrypted, reinterpret_cast<const byte*>(stage1.toUtf8().data()), stage1.toUtf8().size());
    QByteArray stage2 = reinterpret_cast<const char*>(decrypted.data());
    QString stage3 = QString::fromUtf8(stage2);
    final = stage3;
    return final;

}







#endif // ENCRYPT_H

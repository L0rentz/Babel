/*
** EPITECH PROJECT, 2021
** B-CPP-500-NCE-5-1-babel-lionel.da-rocha-da-silva
** File description:
** MyUDP
*/

#include "MyUDP.hpp"

MyUDP::MyUDP(QObject *parent) : QObject(parent)
{
    // create a QUDP socket
    // _readSocket = new QUdpSocket(this);
    _writeSocket = new QUdpSocket(this);

    // _readIp = "10.16.253.70";
    _writeIp = "10.16.253.70";

    // _readPort = 1234;
    _writePort = 1234;

    // The most common way to use QUdpSocket class is
    // to bind to an address and port using bind()
    // bool QAbstractSocket::bind(const QHostAddress & address,
    //     quint16 port = 0, BindMode mode = DefaultForPlatform)
    _writeSocket->bind(QHostAddress(_writeIp.c_str()), _writePort);
    // _readSocket->bind(QHostAddress(_readIp.c_str()), _readPort);

    // connect(_readSocket, SIGNAL(messageSent()), _w, SLOT(sendData()));
}

MyUDP::~MyUDP()
{
}

void MyUDP::sendData()
{
    QByteArray writeData;
    writeData.append("Lorentz");
    _writeSocket->writeDatagram(writeData, QHostAddress(_writeIp.c_str()), _writePort);
    writeData.clear();
}

void MyUDP::readData()
{
    // when data comes in
    QByteArray readBuffer;
    readBuffer.resize(_readSocket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    // qint64 QUdpSocket::readDatagram(char * data, qint64 maxSize,
    //                 QHostAddress * address = 0, quint16 * port = 0)
    // Receives a datagram no larger than maxSize bytes and stores it in data.
    // The sender's host address and port is stored in *address and *port
    // (unless the pointers are 0).

    _readSocket->readDatagram(readBuffer.data(), readBuffer.size(), &sender, &senderPort);

    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message: " << readBuffer;
}

QUdpSocket *MyUDP::getSocket() const
{
    return (_writeSocket);
}

int MyUDP::getPort() const
{
    return (_writePort);
}

std::string MyUDP::getIp() const
{
    return (_writeIp);
}
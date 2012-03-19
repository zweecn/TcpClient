#include "dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    createHostInfoGroupBox();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(hostInfoGroupBox);
    mainLayout->addWidget(statusLabel);
    setLayout(mainLayout);
    resize(400, 200);
    setWindowTitle(tr("Client"));

    tcpSocket = new QTcpSocket(this);
    connect(connectButton, SIGNAL(clicked()), this, SLOT(newConnect()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(displayError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(displayDisconnect()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(sendData()));
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(disConnect()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
}

Dialog::~Dialog()
{

}

void Dialog::newConnect()
{
    sendMessageEdit->setEnabled(false);
    statusLabel->setText(tr("Connecting..."));
    tcpSocket->abort();
    tcpSocket->connectToHost(hostEdit->text(), portEdit->text().toInt());
    qDebug() << "Connecting....";
}

void Dialog::connected()
{
    qDebug() << "Connected.";
    statusLabel->setText(tr("Connected."));
    hostEdit->setEnabled(false);
    portEdit->setEnabled(false);
    connectButton->setEnabled(false);
    sendButton->setEnabled(true);
    disconnectButton->setEnabled(true);
    sendMessageEdit->setEnabled(true);
}

void Dialog::disConnect()
{
    statusLabel->setText(tr("Disconnecting..."));
//    tcpSocket->abort();
    tcpSocket->disconnectFromHost();

}

void Dialog::displayDisconnect()
{
    qDebug() << "Disconnect.";
    sendMessageEdit->setEnabled(false);
    connectButton->setEnabled(true);
    sendButton->setEnabled(false);
    disconnectButton->setEnabled(false);
    hostEdit->setEnabled(true);
    portEdit->setEnabled(true);
    statusLabel->setText(tr("Disconnected."));
}

void Dialog::sendData()
{
    statusLabel->setText(tr("Sending data..."));
    sendMessageEdit->setEnabled(false);
    static int i = 0;
    QString sendStr = sendMessageEdit->text();
    tcpSocket->write(sendStr.toAscii());
    sendMessageEdit->setText(sendStr);
    qDebug() << "Send: " << sendStr;
    i++;
    sendMessageEdit->setEnabled(true);
    statusLabel->setText(tr("Send succeed."));
}

void Dialog::readData()
{
    statusLabel->setText(tr("Reading data..."));
    QString recvStr;
    recvStr = tcpSocket->readAll();
    receiveMessageEdit->setText(recvStr);
    qDebug() << "Receive: " << recvStr;
    sendMessageEdit->setEnabled(true);
    statusLabel->setText(tr("Read succeed."));
}

void Dialog::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString();
    statusLabel->setText(tcpSocket->errorString());
    connectButton->setEnabled(true);
    sendButton->setEnabled(false);
    disconnectButton->setEnabled(false);
    hostEdit->setEnabled(true);
    portEdit->setEnabled(true);
    sendMessageEdit->setEnabled(false);
}


void Dialog::createHostInfoGroupBox()
{
    hostInfoGroupBox = new QGroupBox(tr("Host Info"));
    hostLabel = new QLabel(tr("Host"));
    portLabel = new QLabel(tr("Port"));
    hostEdit = new QLineEdit(tr("localhost"));
    portEdit = new QLineEdit(tr("6666"));
    sendMessageLabel = new QLabel(tr("Send Message"));
    receiveMessageLabel = new QLabel(tr("Receive Message"));
    sendMessageEdit = new QLineEdit();
    receiveMessageEdit = new QLineEdit();
    sendMessageEdit->setEnabled(false);
    receiveMessageEdit->setEnabled(false);

    statusLabel = new QLabel(tr("Status."));
    connectButton = new QPushButton(tr("Connect"));
    disconnectButton = new QPushButton(tr("Disconnect"));
    sendButton = new QPushButton(tr("Send"));
    disconnectButton->setEnabled(false);
    sendButton->setEnabled(false);
    hostLabel->setAlignment(Qt::AlignCenter);
    portLabel->setAlignment(Qt::AlignCenter);
    sendMessageLabel->setAlignment(Qt::AlignCenter);
    receiveMessageLabel->setAlignment(Qt::AlignCenter);

    QGridLayout *hostInfoLayout = new QGridLayout;
    hostInfoLayout->addWidget(hostLabel, 0, 0, 1, 1);
    hostInfoLayout->addWidget(portLabel, 1, 0, 1, 1);
    hostInfoLayout->addWidget(hostEdit, 0, 1, 1, 2);
    hostInfoLayout->addWidget(portEdit, 1, 1, 1, 2);

    hostInfoLayout->addWidget(sendMessageLabel, 2, 0, 1, 1);
    hostInfoLayout->addWidget(receiveMessageLabel, 3, 0, 1, 1);
    hostInfoLayout->addWidget(sendMessageEdit, 2, 1, 1, 2);
    hostInfoLayout->addWidget(receiveMessageEdit, 3, 1, 1, 2);

    hostInfoLayout->addWidget(connectButton, 0, 3, 1, 1);
    hostInfoLayout->addWidget(disconnectButton, 1, 3, 1, 1);
    hostInfoLayout->addWidget(sendButton, 3, 3, 1, 1);
    hostInfoGroupBox->setLayout(hostInfoLayout);
}


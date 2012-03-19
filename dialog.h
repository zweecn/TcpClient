#ifndef DIALOG_H
#define DIALOG_H
#include <QtGui>
#include <QtNetwork>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    QTcpSocket *tcpSocket;

    void createHostInfoGroupBox();
    QGroupBox* hostInfoGroupBox;
    QLabel* hostLabel;
    QLabel* portLabel;
    QLabel* sendMessageLabel;
    QLabel* receiveMessageLabel;
    QLineEdit* hostEdit;
    QLineEdit* portEdit;
    QLineEdit* sendMessageEdit;
    QLineEdit* receiveMessageEdit;
    QPushButton* connectButton;
    QPushButton* disconnectButton;
    QPushButton* sendButton;
    QLabel* statusLabel;
private slots:
    void newConnect();
    void connected();
    void sendData();
    void readData();
    void displayError(QAbstractSocket::SocketError);
    void disConnect();
    void displayDisconnect();
};

#endif // DIALOG_H

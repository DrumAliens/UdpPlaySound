#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QtNetwork/QUdpSocket>
//#include <QTime>
//#include <QTimer>

// UDP information
#define UDP_DEFAULT_RX_PORT 33033
#define UDP_DEFAULT_TX_PORT 33034
#define UDP_DEFAULT_IP_ADDR "127.0.0.1"
#define UDP_RX_BUFF_SIZE 3072  //size of UDP Rx buffer

// periodic timestamp timer
//#define TIMER_PERIOD 50  //50ms

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    //void updateTimer();

    void readPendingUdpDatagrams();

    void on_dial_sliderMoved(int position);

    void ReadSndFileNames();

private:
    Ui::MainWindow *ui;

    // Media player
    QMediaPlayer *player;

    // UDP INITIALISATION
    QUdpSocket *udpSocket;
    QHostAddress udpHostAddr;
    quint16 udpRxPortNum;
    quint16 udpTxPortNum;

//    // TIMER INITILISATION
//    QTime myTimer;
//    double timeStamp;
//    QTimer *timer;
//    int countTimer;

    // Flag used to check the loading of the sound files
    bool sndFileChck_B;
    QStringList sndFileName_Ary;

};

#endif // MAINWINDOW_H

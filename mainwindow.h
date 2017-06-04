#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QtNetwork/QUdpSocket>
#include <QShortcut>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QFileDialog>

//#include <QTime>
//#include <QTimer>

// UDP information
#define UDP_RX_BUFF_SIZE 3072  //size of UDP Rx buffer
#define MAX_NUM_SNDFILES 3    // maximum number of sound files
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

    void readPendingUdpDatagrams();

    void on_dial_sliderMoved(int position);

    void on_radioButton_toggled(bool checked);

    void slotShortcutCtrlR();

    void DisGuiButtnPres();

    void EnblGuiButtnPres();

    void ReadJsonFileInfo();

    void on_actionOpen_File_triggered();

    void on_actionClose_triggered();

private:
    Ui::MainWindow *ui;

    // Media player
    QMediaPlayer *player;

    // UDP INITIALISATION
    QUdpSocket *udpSocket;
    QHostAddress udpHostAddr;
    quint16 udpRxPort_Z;
    QString udpIpAddr_Str;
    bool udpRxBind_B;

    QString jsonFileName_Str;

    // Flag used to check the loading of the sound files
    bool sndFileChck_B;
    QStringList sndFileName_Ary;

    // rx udp Ascii gain
    quint16 rxUdpAsciiMsg_Z;

    // Create shortcut key
    QShortcut *keyCtrlR;

};

#endif // MAINWINDOW_H

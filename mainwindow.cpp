#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    player->setVolume(100);

    // Read in the sound files
    sndFileChck_B = true;
    ReadSndFileNames();

    // Only set up Udp if file information is OK
    if (sndFileChck_B) {
        //set up the UDP for the host connection
        udpSocket = new QUdpSocket(this);
        udpHostAddr.setAddress(UDP_DEFAULT_IP_ADDR);
        udpRxPortNum = UDP_DEFAULT_RX_PORT;
        udpTxPortNum = UDP_DEFAULT_TX_PORT;
        udpSocket->bind(udpRxPortNum,QAbstractSocket::DefaultForPlatform);
        connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingUdpDatagrams()));

        // Decode the Gait Cycle file
        ui -> plainTextEdit->insertPlainText(" Listening on " + udpHostAddr.toString() + " Port: " + QString::number(udpRxPortNum) + "\n");
        ui -> plainTextEdit->insertPlainText("======================\n");

    }


//    // Connect up the timer to updateTimer
//    timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
//    timer->start(TIMER_PERIOD);

//    qDebug() << qApp->applicationDirPath();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    player->setMedia(QUrl::fromLocalFile(qApp->applicationDirPath() + "/" + sndFileName_Ary[0]));
    player->play();
}

void MainWindow::on_pushButton_2_clicked()
{
    player->setMedia(QUrl::fromLocalFile(qApp->applicationDirPath() + "/" + sndFileName_Ary[1]));
    player->play();
}

void MainWindow::on_pushButton_3_clicked()
{
    player->setMedia(QUrl::fromLocalFile(qApp->applicationDirPath() + "/"+ sndFileName_Ary[2]));
    player->play();
}

// =========================================================
// Timer loop
// =========================================================

//void MainWindow::updateTimer(){

//}

void MainWindow::readPendingUdpDatagrams()
{
    char rxUdpDataStr[UDP_RX_BUFF_SIZE];
    // QByteArray buffer[UDP_RX_BUFF_SIZE];
    quint16 rxUdpLen = 0;
    int tuneNum_Z = 0;

    do {
        rxUdpLen =  udpSocket->pendingDatagramSize();
        udpSocket->readDatagram(rxUdpDataStr, rxUdpLen, &udpHostAddr, &udpRxPortNum);
    } while (udpSocket->hasPendingDatagrams());

    //qDebug() << QString::number(rxUdpLen) + " " + QString::number(rxUdpDataStr[0]); // + " " + QString::number(temp);

    // Check the length of the data received and then convert it
    if (rxUdpLen > 0) {
        // Only worry about the first number
        tuneNum_Z = (int) rxUdpDataStr[0];
    }
    else {
        tuneNum_Z = 0;
    }

    // If the number is out of the data range then it will be ignored
    switch (tuneNum_Z) {
    case 1:
        on_pushButton_clicked();
        break;
    case 2:
        on_pushButton_2_clicked();
        break;
    case 3:
        on_pushButton_3_clicked();
        break;
    }

    // Display the information received in the Gui
    ui -> plainTextEdit->insertPlainText("Received " + QString::number(tuneNum_Z)+ "\n");

}

void MainWindow::on_dial_sliderMoved(int position)
{
    player->setVolume(position);
}

void MainWindow::ReadSndFileNames() {

    QFile f(qApp->applicationDirPath() + "/SndFileList.txt");

    //file opened successfully
    if (f.open(QIODevice::ReadOnly))
    {
        // Read all of the data in one go
        QString data = f.readAll();

        // Splits the data into rows using the carriage return
        sndFileName_Ary = data.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

        // Find the number of rows in the file
        int numRow_Z = sndFileName_Ary.count();

        // Check that there are the at least 3 files
        // Don't really care if there are more than 3 we won't play them
        if (numRow_Z >= 3) {

            // Check through the file names that they are valid
            for (int i = 0; i < numRow_Z; i++) {
                // Point at the file
                QFile loclFileName(qApp->applicationDirPath() + "/" + sndFileName_Ary[i]);

                // If already failed then no point checking any further
                if (sndFileChck_B) {
                    // Check that the file exists
                    if (loclFileName.exists()==false) {
                        // Set the file check flag to true
                        sndFileChck_B = false;
                    }
                }
            }

        }
        else {
            // Set the file check flag to true
            sndFileChck_B = false;
        }

        // close the file
        f.close();

    }

    // Send out a display to the Gui Text box
    if (sndFileChck_B == true) {
        // Decode the Gait Cycle file
        ui -> plainTextEdit->insertPlainText(" Sound 1  is "  + sndFileName_Ary[0] + "\n");
        ui -> plainTextEdit->insertPlainText(" Sound 2  is "  + sndFileName_Ary[1] + "\n");
        ui -> plainTextEdit->insertPlainText(" Sound 3  is "  + sndFileName_Ary[2] + "\n");
    }
    else {
        ui -> plainTextEdit->insertPlainText(" Problem reading SndFileName.txt  \n");
        ui -> plainTextEdit->insertPlainText("   Either not enough files or \n");
        ui -> plainTextEdit->insertPlainText("   the files don't exist \n");
        ui -> plainTextEdit->insertPlainText("======================\n");
    }
}

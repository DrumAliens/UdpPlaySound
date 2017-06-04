#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    player->setVolume(100);

    // Set the default that we receive a number in the udp message
    rxUdpAsciiMsg_Z = 0;

    // Switch off the Gui Buttons
    DisGuiButtnPres();

    // Only allow to the udp to bind once
    udpRxBind_B = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionClose_triggered()
{
     qApp->exit();
}

void MainWindow::on_actionOpen_File_triggered()
{
    // Open up a file dialog box at current directory location
    jsonFileName_Str = QFileDialog::getOpenFileName (this, "Open file",qApp->applicationDirPath(), "json (*.json)");

    // Read in the udp and the sound file information from the json file
    ReadJsonFileInfo();

    // if have already setup the udp then don't allow it to do it again
    if (udpRxBind_B == false) {

        // Only set up udp if file information is OK
        if (sndFileChck_B) {
            //set up the udp for the host connection
            udpSocket = new QUdpSocket(this);
            udpHostAddr.setAddress(udpIpAddr_Str);
            udpSocket->bind(udpRxPort_Z,QAbstractSocket::DefaultForPlatform);
            connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingUdpDatagrams()));

            // Check that have binded correctly
            if (udpSocket -> state() == QUdpSocket::BoundState) {
                // Decode the Gait Cycle file
                ui -> plainTextEdit->insertPlainText(" Listening on " + udpHostAddr.toString() + " Port: " + QString::number(udpRxPort_Z) + "\n");
                ui -> plainTextEdit->insertPlainText("======================\n");

                // Set up the Shortkey
                keyCtrlR = new QShortcut(this); // Initialize the object
                keyCtrlR->setKey(Qt::CTRL + Qt::Key_R); // Set the key code
                // connect handler to keypress
                connect(keyCtrlR, SIGNAL(activated()), this, SLOT(slotShortcutCtrlR()));

                // Enable the Gui Buttons
                EnblGuiButtnPres();

                // Set udp flag to be true
                udpRxBind_B = true;
            }
            else {
                // Decode the Gait Cycle file
                ui -> plainTextEdit->insertPlainText(" Failed to bind UDP successfully \n");
                ui -> plainTextEdit->insertPlainText("======================\n");

                // Disable the Buttons in the Gui
                DisGuiButtnPres();

            }
        }
        else {
            // Disable the Buttons in the Gui
            DisGuiButtnPres();
        }
    }

}

void MainWindow::ReadJsonFileInfo()
{

    QFile f(jsonFileName_Str);
    QJsonParseError error;
    QJsonDocument mJsonFile;

    // Set the file check flag to be true
    sndFileChck_B = true;

    if (f.exists()){
            f.open(QIODevice::ReadOnly | QIODevice::Text);
            mJsonFile = QJsonDocument::fromJson(f.readAll(), &error);
            f.close();
            }
        else{
            qDebug() << "Error: File not found!";
            sndFileChck_B = false;
        }

        if (mJsonFile.isNull() || mJsonFile.isEmpty()){
            //qDebug() << "Error: " << error.errorString() << error.offset;
            sndFileChck_B = false;
            }
//        else{
//            //qDebug() << "File" << mJsonFile << " has been parsed correctly.";
//            }

     QJsonObject myJsonObject = mJsonFile.object();

     // Read IP information from file
     udpIpAddr_Str = myJsonObject["IP Address"].toString();
     // Check that not an empty string
     if (udpIpAddr_Str.isEmpty()){
         sndFileChck_B = false;
     }

     // Read Port information from file
     udpRxPort_Z = myJsonObject["Port"].toInt();
     // Check that the port chosen is within a valid range
     if (udpRxPort_Z < 32768) {
         sndFileChck_B = false;
     }

     QJsonArray loclJson_Ary = myJsonObject["SndFiles"].toArray();

     // Find the number of sound files in the json file
     int numSndFiles_Z = loclJson_Ary.size();

     // Make sure that the list starts at the begining
     sndFileName_Ary.clear();

     // Loop through to get the sound file information
     if ((numSndFiles_Z >= MAX_NUM_SNDFILES) && (sndFileChck_B)) {
         for (int i=0; i < MAX_NUM_SNDFILES; i++) {
             //qDebug() << test[i].toString();
             QString loclTemp_Str = loclJson_Ary[i].toString();

             // Replaces the Windows slashes for Unix
             loclTemp_Str.replace("\\","/");

             // If the file name contains the full path then use that otherwise
             // add in the current working directory
             if (loclTemp_Str.contains("/")) {
                 sndFileName_Ary.append(loclTemp_Str);
             }
             else {
                 sndFileName_Ary.append(qApp->applicationDirPath() + "/" + loclTemp_Str);
             }
         }
     }
     else {
         sndFileChck_B = false;
     }

     // Check that the file exists
     if (sndFileChck_B) {

         // Check through the file names that they are valid
         for (int i = 0; i < MAX_NUM_SNDFILES; i++) {
             // Point at the file
             QFile loclFileName(sndFileName_Ary[i]);

             // If already failed then no point checking any further
             if (sndFileChck_B) {
                 // Check that the file exists
                 if (loclFileName.exists()==false) {
                     // Set the file check flag
                     sndFileChck_B = false;
                 }
             }
         }

     }
     else {
         // Set the file check flag to true
         sndFileChck_B = false;
     }


     // Send out a display to the Gui Text box
     if (sndFileChck_B) {
         // Decode the Gait Cycle file
         ui -> plainTextEdit->insertPlainText(" Sound 1 is "  + sndFileName_Ary[0] + "\n");
         ui -> plainTextEdit->insertPlainText(" Sound 2 is "  + sndFileName_Ary[1] + "\n");
         ui -> plainTextEdit->insertPlainText(" Sound 3 is "  + sndFileName_Ary[2] + "\n");

         // If have already binded the udp then switch back on the Gui buttons
         if (udpRxBind_B) {
             EnblGuiButtnPres();
         }
     }
     else {
         ui -> plainTextEdit->insertPlainText(" Problem reading " + jsonFileName_Str + "\n");
         ui -> plainTextEdit->insertPlainText("   Either there is not enough sound files or \n");
         ui -> plainTextEdit->insertPlainText("   they don't exist \n");

         // Switch off the Gui buttons
         DisGuiButtnPres();
     }
     ui -> plainTextEdit->insertPlainText("======================\n");

}

void MainWindow::readPendingUdpDatagrams()
{
    char rxUdpDataStr[UDP_RX_BUFF_SIZE];
    quint16 rxUdpLen = 0;

    int tuneNum_Z = 0;

    do {
        rxUdpLen =  udpSocket->pendingDatagramSize();
        udpSocket->readDatagram(rxUdpDataStr, rxUdpLen, &udpHostAddr, &udpRxPort_Z);
    } while (udpSocket->hasPendingDatagrams());

    //qDebug() << QString::number(rxUdpLen) + " " + QString::number(rxUdpDataStr[0]); // + " " + QString::number(temp);

    // Check the length of the data received and then convert it
    if (rxUdpLen > 0) {
        // Only worry about the first number
        tuneNum_Z = ((int) rxUdpDataStr[0]) - rxUdpAsciiMsg_Z*48;
    }
    else {
        tuneNum_Z = 0;
    }

    // If the number is out of the data range then it will be ignored and will stop
    // any current sound being played
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
    default:
        // If a sound is playing then stop it
        if (player -> state() == QMediaPlayer::PlayingState) {
            player->stop();
        }
        break;
    }

    // Display the information received in the Gui
    ui -> plainTextEdit->insertPlainText(" Received " + QString::number(tuneNum_Z) + "\n");

}

// Uses the radio button to switch to a string based version of the number
void MainWindow::on_radioButton_toggled(bool radioButtState_B)
{
    // Change the flag based upon the state of the radio button
    rxUdpAsciiMsg_Z = 0;
    if (radioButtState_B == true) {
        rxUdpAsciiMsg_Z = 1;
    }
}

void MainWindow::on_pushButton_clicked()
{
    player->setMedia(QUrl::fromLocalFile(sndFileName_Ary[0]));
    player->play();
}

void MainWindow::on_pushButton_2_clicked()
{
    player->setMedia(QUrl::fromLocalFile(sndFileName_Ary[1]));
    player->play();
}

void MainWindow::on_pushButton_3_clicked()
{
    player->setMedia(QUrl::fromLocalFile(sndFileName_Ary[2]));
    player->play();
}

//  Read the position of the volume dial
void MainWindow::on_dial_sliderMoved(int position)
{
    // Change the sound volume based upon the slider position
    player->setVolume(position);
}

// If key stroke is CTRL R then re-read in the json file information
void MainWindow::slotShortcutCtrlR() {

    // Read information in from the json file
    ReadJsonFileInfo();
}

// Switch OFF the buttons on the Gui
void MainWindow::DisGuiButtnPres() {
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
}

// Switch ON the buttons on the Gui
void MainWindow::EnblGuiButtnPres() {
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
}



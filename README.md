# UdpPlaySound

This is a programm written in QT which allows a udp messages to control the playing of 3 sound files. The program reads in a text file SndFileList.txt which should contain the 3 sounds that you want to played. The sound files can be directly played from the gui using the buttons provided, see screen shot below. The txt file and the sound files should be in same directory as the exe file.

The programm listens on 127.0.01 and Port 33033 for an integer 1, 2 or 3 to play the sound. If a subsequent udp message is received then that new file will play instead. The text window displays the requested sound file.

The screen shot below shows the layout

![](https://github.com/DrumAliens/UdpPlaySound/blob/master/images/MainWindow.png)

## Windows
To run as an exe, within windows, it will require the following DLL's from your QT installation

* libgcc_s_dw2-1.dll
* libstdc++-6.dll
* libwinpthread-1.dll
* Qt5Core.dll
* Qt5Gui.dll
* Qt5Multimedia.dll
* Qt5Network.dll
* Qt5Widgets.dll


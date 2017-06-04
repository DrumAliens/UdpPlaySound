# UdpPlaySound

This is a program written in QT which allows a udp messages to control the playing of 3 sound files. The program reads in a json format file which needs to contain the names of the 3 sounds files and the IP and Port address of the transmitted udp message. The sound files can be directly played from the gui using the buttons provided, see screen shot below. 

The programm listens on Receiving IP and Port address for an integer 1, 2 or 3, or "1", "2", or "3" (ASCII 48,49,50), if the radio button is selected, to play the sound. If whilst the sound is playing a new valid sound message is received this sound will start playing instead. If the new message is not in the valid sound file then the sound will be switched off.

The screen shot below shows the layout

![](https://github.com/DrumAliens/UdpPlaySound/blob/master/images/MainWindow.png)




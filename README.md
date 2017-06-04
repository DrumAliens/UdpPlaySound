# UdpPlaySound

This is a program written in QT which allows a udp messages to control the playing of 3 sound files. The program reads in a json format file which needs to contain the names of the 3 sounds files and the IP and Port address of the transmitted udp message. The sound files can be directly played from the GUI using the buttons provided, see screen shot below. The dial controls the master volume for all of the sounds.

The program listens on Receiving IP and Port address for an integer 1, 2 or 3, or if the radio button is selected then text "1", "2", or "3" (ASCII 49, 50, or 51) to play sound. If whilst the sound is playing a new valid sound message is received then this will be played. If the new message is not in the valid sound file then the sound will be switched off. This then provides that ability to control the length of time a sound is played.

Once the programm has seen a valid json file and has set up to a vaild upd port then is is possible to change the sound files either using the File drop down menu to select a different/modified json file or the same file can be re-read using CTRL+R. Note: that in either case only the sound files can be changed.

GUI screen shot

![](https://github.com/DrumAliens/UdpPlaySound/blob/master/images/MainWindow.png)

## Json File Format

Below is a copy of the format json format that the program is expecting. If any of the fields are missing then an error will be report in the text box of the GUI. The json file information allows path information to be included so it can refer to files located anywhere on your machine. The program accepts either // or \ separators for the path information. If no directory is included in the path then the sound files is assumed to be in the same directory as the exe file. Each file is check that it is valid, no check undertaken to determine whether it is a valid sound file.

```
{
  "IP Address": "127.0.0.1",
  "Port": 33033,
  "SndFiles": ["C:\\Users\\pking6\\Documents\\QT\\UdpPlaySnd\\UdpPlaySnd\\release\\Marimba.wav", 
               "C:/Users/pking6/Documents/QT/UdpPlaySnd/UdpPlaySnd/release/Asleep.mp3", 
               "TOCTone.wav" ]
} 
```




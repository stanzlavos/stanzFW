* I am running an apache server on my Raspberry Pi. The php script in this folder is used for the update procedure. 
  Please refer some online tutorial to get apache setup.
* After installing apache, the default webpage to be displayed will be "index.html" at "/var/www"

  I have added below folder structure here as of now :
  
  - scripts
  	- update.php
  - bin
  	- control
  	  - test_v1.1.bin  
  - spiffs_bin
  	- control
  	  - test_v1.1.spiffs.bin
  
  Software updates will be in bin and SPIFFS update images will be in spiffs_bin	  
  	  
* There are two arrays in update.php : "version_db" and "type_db"

  "type_db" should have an entry for each board. This gives the type of the board. As in, is it a board running a
  "control" software or "door" detection software. The entry in this array will be the name of the subfolder in
  bin or spiffs_bin where the binary will be present.
  
  "version_db" should have the version of the image for a particular type of board.
  
* To generate spiffs image :
  
  - Add the folder path of mkspiffs to the "PATH" environment variable
  - Open "cmd" and go to the firmware folder (where the folder "data" is present)
  - Run : mkspiffs -c data -b 8192 -s 3125248 test_v1.1.spiffs.bin (this is for 3MB SPIFFS; modify name of bin file as required)	  

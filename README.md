THE PROGRAM WAS CREATED ON LINUX (UBUNTU) IN Qt5

INSTALLATION (UBUNTU):

1. Download "Player.zip".
2. Unpack "Player.zip".
3. Move the "Player" folder to your home folder.

IF YOU WANT TO RUN THE PROGRAM FROM COMMAND LINE:  
*4. In "Player/video" folder put mp4 files.  
5. Open command prompt, go to the "Player" folder and type "./player".  

IF YOU WANT TO START THE PROGRAM FROM MENU:  
4. Open the "player.desktop" file in the "Player" folder.  
5. Change "username" for "Exec" and "username" for "Icon" to the name of the current user and save the file.  
6. Move the file "player.desktop" to "/home/"username"/.local/share/applications/".  
7. Log out and log back in.  
8. Move the "video" folder ("Player/video") to your home folder.  
9. In "video" folder put mp4 files.  
10. Open menu, find "Player" and launch it.  

USE OF SOURCE CODE:  

In "main.cpp" on lines 19 and 20 are the paths to change.  

KEYBOARD SHORTCUTS:  

"->" fast forward 10 seconds.  
"<-" Rewind 10 seconds.  
"↑" increase the volume.  
"↓" decrease the volume.  
"space" pause and resume playback.  
"esc" pauses playback and minimizes all program windows.  

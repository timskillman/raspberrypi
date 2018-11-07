# Raspberry Pi OpenGL ES 2.0 Graphics Engine

A portable, high speed graphics engine in OpenGL ES 2.0 for the Raspberry Pi B/B+ and Zero in C++

Contains the following example projects;

 - **SceneViewer** Load modified OBJ files to view fully collidable scenes with animated textures
 - **ScreenRunner** Animated backgrounds with Gamepad input for changing between screens
    
# Compiling on Raspberry Pi

 - Run 'make' for particular project in Projects folder
 
 **Note:** SDL2.0.5 is installed in Raspbian Stretch although you will need to add other libraries to develop on the Pi with this code;
 
 To add libraries, use the 'Add / Remove Software' utility from the Pi->Preferences menu
 
 Add the following libraries;
 
   - Simple DirectMedia Layer development files (libsdl2-dev-2.0.5+dfsg1-2)
   - Image loading library for SDL2 (libsdl2-image-2.0-0-2.0.1+dfsg-2+deb9u1)
   - Image loading library for SDL2 development files (libsdl2-image-dev-2.0.1+dfsg-2+deb9u1)
   - TrueType Font library for SDL2 (libsdl2-ttf-2.0-0-2.0.14+dfsg1-1)
   - TrueType Font library for SDL2 development files (libsdl2-ttf-dev-2.0.14+dfsg1-1)
   
   (you could also add SDL2 Mixer and Network as well - but not essential)
   
   SDL2 libraries also requires OpenGLES 1.x API (GLES library) not currently included in the Stretch image ...
   
   - libgles1-mesa-13.0.6-1+rpi2
   - libgles1-mesa-dev-13.0.6-1+rpi2

**Configuring the Raspberry Pi for Raspberry Stretch OS (with Desktop)**

These examples require OpenGL hardware acceleration and 256Mb graphics memory. Do the following to configure your Raspberry Pi if these examples run slowly;

    Open terminal and type;

    $ sudo raspi-config

    - Select 7 'Advanced Options'
    - Select A3 'Memory Split'
    - Type 256 and hit enter (to reserve 256Mb graphics memory)
    - Select 7 'Advanced Options'
    - Select A7 'GL Driver'
    - Select G1 GL (Full KMS) OpenGL desktop driver with full KMS
    - Select <Finish> and reboot
    
# Compiling for Windows

 - Load solution in VS2017 from Projects folder and run
 
## SceneViewer Project

![Bridge Screenshot](http://www.skillmanmedia.com/images/bridge7.jpg)

Run SceneViewer with '-scene' command, for example

    ./SceneViewer -scene CargoBay.scene
    ./SceneViewer -scene KendalCastle.scene
    ./SceneViewer -scene Bridge.scene
  
## ScreenRunner Project

![Tunnel Screenshot](http://www.skillmanmedia.com/images/hextunnel.jpg)

Displays a sequence of animated screens

Run ScreenRunner - loads various screen scripts from the screens folder and displays them

    ./ScreenRunner


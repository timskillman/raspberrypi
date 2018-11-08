# Raspberry Pi OpenGL ES 2.0 Graphics Engine

A portable, high speed graphics engine in OpenGL ES 2.0 for the Raspberry Pi B/B+ and Zero in C++

Contains the following example projects;

 - **SceneViewer** Load modified OBJ files to view fully collidable scenes with animated textures
 - **ScreenRunner** Animated backgrounds with Gamepad input for changing between screens
    
# Compiling on Raspberry Pi

 - Run 'make' for particular project in Projects folder
 
 **Note:** SDL2.0.5 is installed in Raspbian Stretch although you will need to use SDL2.0.9 for the shaders in these projects to work.
 
 Please follow the instructions on ChoccyHobNob's website to install SDL2.0.9 (of the latest SDL2) on your Raspberry Pi;
 
 https://choccyhobnob.com/raspberry-pi/sdl2-2-0-8-on-raspberry-pi/
 

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


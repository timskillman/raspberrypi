# Raspberry Pi OpenGL ES 2.0 Graphics Engine

A portable, high speed graphics engine in OpenGL ES 2.0 for the Raspberry Pi B/B+ and Zero in C++

Contains the following example projects;

 - **SceneViewer** Load modified OBJ files to view fully collidable scenes with animated textures
 - **ScreenRunner** Animated backgrounds with Gamepad input for changing between screens
 
# Compiling on Raspberry Pi

 - Run 'make' for particular project in Projects folder
 
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


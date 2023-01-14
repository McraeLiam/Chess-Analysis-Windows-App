# Chess-Analysis-Windows-App
*WIP*
This is a little project that I've been working on for a few weeks to learn C++ and the windows API.
I'm still learning C++ so please don't be too harsh when judging my code. However, please give me any constructive criticism and suggestions that you might have.

Anyways, this project has two main sections: the window handling and the internal process handling.

WINDOW HANDLING : WindowClass.h/AppManager.cpp
The window handling utilizes the <windows.h> header to open and handle the basic functions of windows such as close and painting.

INTERNALS HANDLING : InternalDefs.h
The internal class definitions utilize custom classes that represent different sections of the window that might want to be dedicated to certain GUI elements.
I'm trying to make this as dynamic as possible just in case I want to add another game.

Chess.h
Definitions for chess handling.

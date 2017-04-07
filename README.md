# Phantom-Omni-Plugin
This is a Game Plugin for Unreal Engine (developed and tested with UE 4.11)
It allows you to use the Phantom Omni Devices (have a look at http://www.dentsable.com/haptic-phantom-omni.htm) in Unreal.
This works only if you have installed the Devices and all necessary drivers.

*Caution*
The code for this plugin is still pretty messy and only tested once on a Windows 7 machine. Please let me now if you want to use it and write me an email in case there are any problems: kollasch@uni-bremen.de

## Features
* Automatic intialization of the device
* Position retrieving and rotation
* Haptic and Force calculation

## Planned Addons
* Blueprint Interface so you don't have do déal with C++ anymore ;)

## Install
Go to your project's *Plugins* folder. There is no such folder? Create one! Place the PhantomControllerPlugin-Folder in the
Plugins-Folder. Now open your project's solution and do a clean build. Aaaaand you're done!

## How to use?
Right now this plugin is conceptuated to be used with C++. In the Engine, create a new C++ actor. Go to your new actors header file
and include the *PhantomControllerPlugin.h* and the *PhantomControllerInputDevice.h*
Now you can save the device as a variable in your new Actor. Take a look at the PhantomControllerTest.h and 
PhantomControllerTest.cpp, these are examples how you use the plugin correctly in your own C++ Component.

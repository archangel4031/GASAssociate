# GASAssociate

### Description:

This is the source code for for the Plugin GAS Associate, that is supposed to make your life easier while using Unreal's Gameplay Ability System.

    Current Version Ver 1.3.0
    Updated on 3-Jul-2022

### Information:

The plugin contains all the necessary C++ files required to make Gameplay Ability System work. This plugin allows you to jump quickly into GAS without needing to manually create each and every C++ file. I have already created tutorial series on using Gameplay Ability System with Blueprints. I highly recommend you watch the tutorial series to get familiar with Gameplay Ability System. The tutorial series also cover C++ explanations that are easy to understand even for non-programmers.

 - [UE4 Gameplay Ability System Practical Example](https://www.youtube.com/playlist?list=PLeEXbS_TaXrAbfoPYSNROqe1fDQfQHTfo)
 - [UE4 Gameplay Ability System for Blueprint Programmers](https://www.youtube.com/playlist?list=PLeEXbS_TaXrDlqQv753CpKqDlpNXixFMg)

The repo contains blank C++ based projects for both Unreal 4.27 and Unreal 5.0

### How to Use
###### Use Template Blank Project

 1. Download the Blank Project according to your required Engine Version
 2. Start working in the project directly OR
 3. Import your content in this Blank Project and start using GAS

###### Use Standalone Plugin Folder

 1. Download the Plugins folder from the [***Releases***](https://github.com/archangel4031/GASAssociate/releases) Section according to your required Engine Version
 2. If working with a Blueprint Only Project, first add a new C++ Class of Actor. If not continue to next step
 3. Close the Editor and VS
 4. Paste the Plugins folder in the Project Root Directory
 5. Right Click on your .uproject file and select ***Generate Visual Studio Project Files***
 6. Open the Visual Studio and build the project for first time use.
 7. Once compiled, open the Engine and start using GAS

**NOTES:** 

 - See linked video for further information on the capabilities of the plugin
 - The Plugin does not have *Error Checking* for Attribute names. Be careful while naming your attributes and only use alphabets without spaces
 
 **Known Issues**
 - **FOR Mac Users Only:** You will need to manually add `NetCore` to `PublicDependencyModuleNames` array in ***YourProject.Build.cs*** file. (Refer to [this issue](https://github.com/archangel4031/GASAssociate/issues/1) for more information.)
 - Error handling for Attribute Naming is missing. Engine will crash if DefaultGASAttribute.ini file is missing and GAS Associate Helper is used
 - Error Handling of empty Initial Attribute Data is missing. Engine will crash if Attribute Test Data on Ability System Component is not set correctly or left empty
 
### Video:
Refer to the video for more information. (Ver 1.0.0)
[GAS Associate | A plugin for Unreal Engine 5 and Unreal Engine 4](https://youtu.be/ett9ZTHYN8g)

#### Update Log

 - Ver 1.3.0
	 - Added a proper Target Actor for ground trace. This Target Actor is based of built in Ground Trace Target Actor Class
	 - Removed support for UE4. The UE4 will now exist on *legacy* branch
 - Ver 1.2.2
	 - Added an option to select Replication Mode for Ability System Component in Attribute Helper Wizard. Now you can use Full mode for single player games
	 - Default Replication Mode is set to ***Full***
 - Ver 1.2.1
	 - Added Get Ability Info Function in Character Class as well. Now you can get GA Class info in Character Class to update UI Elements
	 - Fixed spelling mistake in GAS Character Class
	 - Fixed Initial Data Table with values for all example Attributes
 - Ver 1.2.0
	 - Added Get Ability Info Function in GAS Gameplay Ability. Now you can retrieve information about Ability Cost and Cooldown using this function. (Currently only Modifier based Cost is supported)
	 - Added a separate [**Release**](https://github.com/archangel4031/GASAssociate/releases) for Ver 1.2.0
 - Ver 1.1.0
	 - Added an option to automatically create a Max Value Attribute to handle clamping (see Example Project)
	 - Added Server Only functions that only execute as server. Generic versions of functions still exists and may be useful in some cases
	 - More comments in C++ Code

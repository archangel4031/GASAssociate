# GASAssociate

### Description:

This is the source code for for the Plugin GAS Associate, that is supposed to make your life easier while using Unreal's Gameplay Ability System.

### Information:

The plugin contains all the necessary C++ files required to make Gameplay Ability System work. There is already my tutorial series on using Gameplay Ability System, [you can find it here](https://www.youtube.com/playlist?list=PLeEXbS_TaXrAbfoPYSNROqe1fDQfQHTfo). The repo contains blank C++ based projects for both Unreal 4.27 and Unreal 5.0

### How to Use
###### Use Template Blank Project

 1. Download the Blank Project according to your required Engine Version
 2. Start working in the project directly OR
 3. Import your content in this Blank Project and start using GAS

###### Use Standalone Plugin Folder

 - Download the Plugins folder from the [***Releases***](https://github.com/archangel4031/GASAssociate/releases) Section according to your required Engine Version
 - If working with a Blueprint Only Project, first add a new C++ Class of Actor. If not continue to next step
 - Paste the Plugins folder in the Project Root Directory
 - Right Click on your .uproject file and select ***Generate Visual Studio Project Files***
 - Open the Visual Studio and build the project for first time use.
 - Once compiled, open the Engine and start using GAS

**NOTES:** 

 - See linked video for further information on the capabilities of the plugin
 - The Plugin does not have *Error Checking* for Attribute names. Be careful while naming your attributes and only use alphabets without spaces
 
### Video:
Refer to the video for more information. (Ver 1.0.0)
[GAS Associate | A plugin for Unreal Engine 5 and Unreal Engine 4](https://youtu.be/ett9ZTHYN8g)

#### Update Log

 - Ver 1.1.0
	 - Added an option to automatically create a Max Value Attribute to handle clamping (see Example Project)
	 - Added Server Only functions that only execute as server. Generic versions of functions still exists and may be useful in some cases
	 - More comments in C++ Code
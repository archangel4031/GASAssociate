# GASAssociate

[![Discord](https://img.shields.io/discord/820665024137789472?label=discord)](https://discord.gg/nfkTafPJKK)
[![GitHub last commit](https://img.shields.io/github/last-commit/archangel4031/GASAssociate?color=%237d0096)](https://github.com/archangel4031/GASAssociate)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/archangel4031/GASAssociate?color=%23ad0000&label=latest%20release)](https://github.com/archangel4031/GASAssociate/releases)
[![GitHub repo size](https://img.shields.io/github/repo-size/archangel4031/GASAssociate)](https://github.com/archangel4031/GASAssociate)
[![GitHub forks](https://img.shields.io/github/forks/archangel4031/GASAssociate?style=social)](https://github.com/archangel4031/GASAssociate/network/members)
[![GitHub Repo stars](https://img.shields.io/github/stars/archangel4031/GASAssociate?style=social)](https://github.com/archangel4031/GASAssociate/stargazers)

## Beta branch! Use at your own risk!

### Description:

This is the source code for for the Plugin GAS Associate, that is supposed to make your life easier while using Unreal's Gameplay Ability System.

### Information:

The plugin contains all the necessary C++ files required to make Gameplay Ability System work. This plugin allows you to jump quickly into GAS without needing to manually create each and every C++ file. I have already created tutorial series on using Gameplay Ability System with Blueprints. I highly recommend you watch the tutorial series to get familiar with Gameplay Ability System. The tutorial series also cover C++ explanations that are easy to understand even for non-programmers.

 - [UE4 Gameplay Ability System Practical Example](https://www.youtube.com/playlist?list=PLeEXbS_TaXrAbfoPYSNROqe1fDQfQHTfo)
 - [UE4 Gameplay Ability System for Blueprint Programmers](https://www.youtube.com/playlist?list=PLeEXbS_TaXrDlqQv753CpKqDlpNXixFMg)

The repo contains blank C++ based projects for both Unreal 4.27 and Unreal 5.0

#### Update Log

 - Ver 1.5.1-devtesting
	 - Update plugin project to Unreal Engine 5.1 The project name has also changed
	 - Update API to Unreal Engine version 5.1. Code changed in Target Actor Classes
	 - Added prompt for using GAS Associate Helper. Now the plugin will explicitly ask you to press *Yes* before modifying the files
 - Ver 1.5.0-devtesting
	 - Added functions for setting Attribute Values directly from Character Class. These can be used to initialize Attributes from Save File for example.
	 - Updated GAS Associate Helper to create Setter functions for changing Attributes directly. Now GAS Associate Helper will also create these Setter functions for each Attribute
 - Ver 1.4.1-devtesting
	 - Improved error handling for Attribute Names
	 - Added error handling for empty starting data. Not setting Default Starting Data with Attribute Table will no longer crash the Engine
 - Ver 1.3.0-devtesting
	 - Added error handling for Attribute Names. Now GAS Associate Helper will take proper inputs for Attribute Names. It will ignore wrong / invalid syntax names
	 - Added error handling for empty DefaultGASAttributes.ini file. In case of any error reading the file, plugin will auto add a default attribute of Health and Max Health
	 - Added separate branch for in-development version for public testing (beta version of plugin will go in *[devtesting](https://github.com/archangel4031/GASAssociate/branches)* branch)
 - Ver 1.3.0
	 - Added a proper Target Actor for ground trace. This Target Actor is based of built in Ground Trace Target Actor Class
	 - Removed support for UE4. The UE4 will now exist on *[legacy](https://github.com/archangel4031/GASAssociate/tree/legacyUE4UE5)* branch
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

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "UnrealString.h"
//#include "Modules/ModuleManager.h"
#include <sstream>
#include <fstream>
#include <string>
#include <regex>

/**
 * 
 */
class HelperFunctions
{

public:

	//FIXME: These variables should be private

	// Get project Config and Plugins Directory Paths
	FString ConfigDir;
	FString PluginDir;
	std::string BaseConfigPath;
	std::string BasePluginPath;

	// Build Path to be used for reading and writing files
	std::string configFilePath;
	std::string outAttrPathHeader;
	std::string outAttrPathCPP;
	std::string outCharPathHeader;
	std::string outCharPathCPP;

	// Store Attributes in vectors to be filled and used to modify GAS Source Files
	std::vector<std::string> vAttributeNames ;
	std::vector<int> vAttributeMin;
	std::vector<int> vAttributeMax;
	std::vector<bool> vUseMaxValueAttribute;
	std::string vRepMode;
	bool AddSupportForALS;

	// String Constants
	std::string HeaderDef;
	std::string HeaderALS;
	std::string InheritDef;
	std::string InheritALS;
	std::string ConstructorDef;
	std::string ConstructorALS;
	std::string ConstructorCPPDef;
	std::string ConstructorCPPALS;


	// Constructor
	HelperFunctions();

	//GetterFunctions ---- May implement in future
	//FString GetConfigDir();
	//FString GetPluginDir();
	//std::string GetBaseConfigPath();
	//std::string GetBasePluginPath();
	//std::string GetConfigFilePath();
	//std::string GetOutAttrPathHeader();
	//std::string GetOutAttrPathCPP();
	//std::string GetOutCharPathHeader();
	//std::string GetOutCharPathCPP();


	//FN: Create a default DefaultGASAttributes.ini file in case the file does not exist (this will be handled in try-catch)
	void makeDefaultFile();
	
	//FN: open and store Config file in buffer
	std::string openFile();
	
	//FN: Fill Names of Attributes from Config File
	void fillAttributeName(std::string inString);
	
	//FN: Fill Min Values of Attributes from Config File
	void fillAttributeMins(std::string inString);
	
	//FN: Fill Max Values of Attributes from Config File
	void fillAttributeMax(std::string inString);

	//FN: Fill Use Max Value of Attributes from Config File
	void fillAttributeBools(std::string inString);

	//FN: Fill Attributes Rep Mode of Attributes from Config File
	void fillAttributeRepMode(std::string inString);

	//FN: Fill ALS Support Value
	void fillALSSupportValue(std::string inString);

	//FN: Write to Attribute Header File GASAttribute.h
	void writeAttrToHeader();

	//FN: Make function Post GE Exec used in GASAttribute.cpp file
	void writePostGEExec(std::ofstream& fileStream);

	//FN: Make Replication functions used in GASAttribute.cpp file
	void writeLifeRepProp(std::ofstream& fileStream);

	//FN: Make OnRep functions used in GASAttribute.cpp file
	void writeOnReps(std::ofstream& fileStream);

	//FN: Write Attributes to GASAttribute.cpp file
	void writeAttrToCPP();

	//FN: Make Event Delagtes used in GASCharacter.h
	void writeEventTriggers(std::ofstream& fileStream);

	//FN: Make Event Delegate Binds to BP side used in GASCharacter.h
	void writeEventBindsBP(std::ofstream& fileStream);

	//FN: Make attribute Getters used in GASCharacter.h
	void writeGettersFN(std::ofstream& fileStream);

	//FN: Make attribute Setters used in GASCharacter.h
	void writeSetterFN(std::ofstream& fileStream);

	//FN: Write changes GASCharacter.h
	void writeAttrToCharHeader();

	// FN: Make rest of Constructor of GASCharacter.cpp file
	void writeCharRepMode(std::ofstream& fileStream);

	//FN: Make BeginPlay of GASCharacter.cpp file
	void writeCharConstructor(std::ofstream& fileStream);

	//FN: Make Delagete Binds of GASCharacter.cpp file
	void writeCharBindFN(std::ofstream& fileStream);

	//FN: Make Attribute Value Getter of GASCharacter.cpp file
	void writeCharGetterFN(std::ofstream& fileStream);

	//FN: Make Attribute Value Setter of GASCharacter.cpp file
	void writeCharSetterFN(std::ofstream& fileStream);

	//FN: Write changes GASCharacter.cpp
	void writeAttrToCharCPP();

	//FN: Clear Variables Vectors
	void clearVectors();

	//FN: Edit cs and uplugin files for ALS
	void updateCSAndUplugins();

};

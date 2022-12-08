// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASAssociateHelper.h"
#include "GASAssociateHelperStyle.h"
#include "GASAssociateHelperCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

#include "GASHAttributeWizard.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"
#include "Developer/Settings/Public/ISettingsContainer.h"

//**************************************************Custom Modder Code***************************************************
#include <sstream>
#include <fstream>
#include <string>
#include <regex>

//Get project Config and Plugins Directory Paths
FString ConfigDir = FPaths::ProjectConfigDir();
FString PluginDir = FPaths::ProjectPluginsDir();

std::string const BaseConfigPath = TCHAR_TO_UTF8(*ConfigDir);
std::string const BasePluginPath = TCHAR_TO_UTF8(*PluginDir);

//Build Path to be used for reading and writing files
std::string configFilePath = BaseConfigPath + "DefaultGASAttributes.ini";
std::string outAttrPathHeader = BasePluginPath + "GASAssociate/Source/GASAssociate/Public/GASAttributeSet.h";
std::string outAttrPathCPP = BasePluginPath + "GASAssociate/Source/GASAssociate/Private/GASAttributeSet.cpp";
std::string outCharPathHeader = BasePluginPath + "GASAssociate/Source/GASAssociate/Public/GASCharacter.h";
std::string outCharPathCPP = BasePluginPath + "GASAssociate/Source/GASAssociate/Private/GASCharacter.cpp";

//Store Attributes in vectors to be filled and used to modify GAS Source Files
std::vector<std::string> vAttributeNames = {};
std::vector<int> vAttributeMin = {};
std::vector<int> vAttributeMax = {};
std::vector<bool> vUseMaxValueAttribute = {};
std::string vRepMode = "";

//FN: Create a default DefaultGASAttributes.ini file in case the file does not exist (this will be handled in try-catch)
void makeDefaultFile(std::string inFilePath)
{

	//Open file in WRITE mode to output chanegs
	std::ofstream fileStreamO(inFilePath, std::ios_base::out);

	//Create a default Attribute of Health and Max Health
	fileStreamO << "[/Script/GASAssociateHelper.GASHAttributeWizard]" << std::endl;
	fileStreamO << "+ AttributeList = (AttributeName = \"Health\", MinValue = 0, MaxValue = 100, UseMaxValueAttribute = True)" << std::endl;
	fileStreamO << "AttributeRepMode = Full" << std::endl;
}

//FN: open and store Config file in buffer
std::string openFile(std::string inFilePath)
{
	//Store complete content from file in a Buffer.
	//Code copied from StackOverflow. I honestly have no idea how it work, but it does!

	std::ifstream MyReadFile(inFilePath);
	std::string outFileBuffer = "";

	MyReadFile.seekg(0, std::ios::end);
	outFileBuffer.reserve(MyReadFile.tellg());
	MyReadFile.seekg(0, std::ios::beg);
	outFileBuffer.assign((std::istreambuf_iterator<char>(MyReadFile)), std::istreambuf_iterator<char>());
	MyReadFile.close();

	return outFileBuffer;
}

//FN: Fill Names of Attributes from Config File
void fillAttributeName(std::string inString)
{
	UE_LOG(LogTemp, Warning, TEXT("=====>Filling Attributes Array Names"));

	//Declare Variabeles for regex pattern matching
	std::smatch match;
	std::regex regexAttributeName("AttributeName=\"[a-zA-Z][a-zA-Z0-9]+\"");			// The first character must be an aplhabet
	std::string tempAttrName;
	std::string bufferLine = "";
	size_t StartPos = 0;
	size_t EndPos = 0;

	//Read Config Buffer line-by-line
	std::istringstream inStringStream(inString);

	//Iterate through line and search for pattern.
	while (getline(inStringStream, bufferLine))
	{
		//This While Loop can be replaced I think, but all examples on internet use this so...
		while (regex_search(bufferLine, match, regexAttributeName))
		{
			//Store in temp Variable
			for (auto x : match)
			{
				tempAttrName = x;
			}
			//Find " and trim them from final string. Store in vector
			StartPos = tempAttrName.find("\"");
			if (StartPos != std::string::npos)
			{
				++StartPos;
				EndPos = tempAttrName.find("\"", StartPos);
				if (EndPos != std::string::npos)
				{
					tempAttrName = tempAttrName.substr(StartPos, EndPos - StartPos);
					vAttributeNames.push_back(tempAttrName);
				}
			}
			break;
		}
	}
}

//FN: Fill Min Values of Attributes from Config File
void fillAttributeMins(std::string inString)
{
	UE_LOG(LogTemp, Warning, TEXT("=====>Filling Attributes Array Min Values"));

	//Declare Variabeles for regex pattern matching
	std::smatch match;
	std::regex regexAttributeName("MinValue=[0-9]+");
	std::string tempAttrName;
	std::string bufferLine = "";
	size_t StartPos = 9;
	size_t EndPos = 0;

	//Read Config Buffer line-by-line
	std::istringstream inStringStream(inString);

	//Iterate through line and search for pattern.
	while (getline(inStringStream, bufferLine))
	{
		//This While Loop can be replaced I think, but all examples on internet use this so...
		while (regex_search(bufferLine, match, regexAttributeName))
		{
			//Store in temp Variable
			for (auto x : match)
			{
				tempAttrName = x;
			}
			//Get Length of match and only push Numeric Value to vector
			EndPos = tempAttrName.length();
			tempAttrName = tempAttrName.substr(StartPos, EndPos);
			vAttributeMin.push_back(stoi(tempAttrName));
			break;
		}
	}
}

//FN: Fill Max Values of Attributes from Config File
void fillAttributeMax(std::string inString)
{
	UE_LOG(LogTemp, Warning, TEXT("=====>Filling Attributes Array Max Values"));

	//Declare Variabeles for regex pattern matching
	std::smatch match;
	std::regex regexAttributeName("MaxValue=[0-9]+");
	std::string tempAttrName;
	std::string bufferLine = "";
	size_t StartPos = 9;
	size_t EndPos = 0;

	//Read Config Buffer line-by-line
	std::istringstream inStringStream(inString);

	//Iterate through line and search for pattern.
	while (getline(inStringStream, bufferLine))
	{
		//This While Loop can be replaced I think, but all examples on internet use this so...
		while (regex_search(bufferLine, match, regexAttributeName))
		{
			//Store in temp Variable
			for (auto x : match)
			{
				tempAttrName = x;
			}

			//Get Length of match and only push Numeric Value to vector
			EndPos = tempAttrName.length();
			tempAttrName = tempAttrName.substr(StartPos, EndPos);
			vAttributeMax.push_back(stoi(tempAttrName));
			break;
		}
	}
}

//FN: Fill Use Max Value of Attributes from Config File
void fillAttributeBools(std::string inString)
{
	UE_LOG(LogTemp, Warning, TEXT("=====>Filling Attributes Array Use Max Value Attribute"));

	//Declare Variabeles for regex pattern matching
	std::smatch match;
	std::regex regexAttributeName("UseMaxValueAttribute=[a-zA-Z]+");
	std::string tempAttrName;
	std::string bufferLine = "";
	size_t StartPos = 21;
	size_t EndPos = 0;

	//Read Config Buffer line-by-line
	std::istringstream inStringStream(inString);

	//Iterate through line and search for pattern.
	while (getline(inStringStream, bufferLine))
	{
		//This While Loop can be replaced I think, but all examples on internet use this so...
		while (regex_search(bufferLine, match, regexAttributeName))
		{
			//Store in temp Variable
			for (auto x : match)
			{
				tempAttrName = x;
			}

			//Push True if string is true and false if string is false
			EndPos = tempAttrName.length();
			tempAttrName = tempAttrName.substr(StartPos, EndPos);
			if (tempAttrName == "True")
			{
				vUseMaxValueAttribute.push_back(true);
			}
			else
			{
				vUseMaxValueAttribute.push_back(false);
			}
			
			break;
		}
	}
}

//FN: Fill Attributes Rep Mode of Attributes from Config File
void fillAttributeRepMode(std::string inString)
{
	UE_LOG(LogTemp, Warning, TEXT("=====>Filling Attributes Rep Mode"));

	//Declare Variabeles for regex pattern matching
	std::smatch match;
	std::regex regexAttributeName("AttributeRepMode=[a-zA-Z]+");
	std::string tempAttrName;
	std::string bufferLine = "";
	size_t StartPos = 17;
	size_t EndPos = 0;

	//Read Config Buffer line-by-line
	std::istringstream inStringStream(inString);

	//Iterate through line and search for pattern.
	while (getline(inStringStream, bufferLine))
	{
		//This While Loop can be replaced I think, but all examples on internet use this so...
		while (regex_search(bufferLine, match, regexAttributeName))
		{
			//Store in temp Variable
			for (auto x : match)
			{
				tempAttrName = x;
			}

			//Trim the temp String to get Rep Mode only
			EndPos = tempAttrName.length();
			tempAttrName = tempAttrName.substr(StartPos, EndPos);
			vRepMode = tempAttrName;
			break;
		}
	}
}

//FN: Write to Attribute Header File GASAttribute.h
void writeAttrToHeader()
{
	UE_LOG(LogTemp, Warning, TEXT("=====>Modifying GASAttributeSet Header File"));
	
	//Variable to store lines of Original File that are not to be modified (used to reconstruct file later)
	std::vector<std::string> fileBufferPre;

	//read original file to buffer and define variable for regex
	std::ifstream fileStream(outAttrPathHeader);
	std::string lineBuffer = "";
	bool breakFlag = false;
	std::smatch match;
	std::regex regexPattern("//==PATTERN==");

	//Read file line by line and store content in vector until a specific PATTERN is found
	while (getline(fileStream, lineBuffer))
	{
		fileBufferPre.push_back(lineBuffer);

		while (regex_search(lineBuffer, match, regexPattern))
		{
			for (auto x : match)
			{
				breakFlag = true;
			}
			break;
		}

		if (breakFlag)
		{
			break;
		}
	}

	fileStream.close();

	//Open file in WRITE mode to output chanegs
	std::ofstream fileStreamO(outAttrPathHeader, std::ios_base::trunc);

	//Write the unmodified section
	for (const std::string& str : fileBufferPre)
	{
		fileStreamO << str << std::endl;
	}

	int counter = 0;
	//Write Attributes. Make necessary changes if UseMaxValue is True
	for (const std::string& str : vAttributeNames)
	{
		fileStreamO << "\t" << "//Attribute for " << str << std::endl;
		fileStreamO << "\t" << "UPROPERTY(BlueprintReadOnly, Category = \"Attributes\", ReplicatedUsing = OnRep_" << str << ")" << std::endl;
		fileStreamO << "\t" << "FGameplayAttributeData " << str << ";" << std::endl;
		fileStreamO << "\t" << "ATTRIBUTE_ACCESSORS(UGASAttributeSet, " << str << ")" << std::endl << std::endl;

		if (vUseMaxValueAttribute.at(counter))
		{
			fileStreamO << "\t" << "//Attribute for Max" << str << std::endl;
			fileStreamO << "\t" << "UPROPERTY(BlueprintReadOnly, Category = \"Attributes\", ReplicatedUsing = OnRep_Max" << str << ")" << std::endl;
			fileStreamO << "\t" << "FGameplayAttributeData Max" << str << ";" << std::endl;
			fileStreamO << "\t" << "ATTRIBUTE_ACCESSORS(UGASAttributeSet, Max" << str << ")" << std::endl << std::endl;
		}

		counter++;
	}

	//Write Delegates
	fileStreamO << "\t//Delegates for attribute changes\n";
	for (const std::string& str : vAttributeNames)
	{
		fileStreamO << "\t" << "FAttrChangeDelegate " << str << "ChangeDelegate;\n";
	}

	//Write OnRepFuncitons
	fileStreamO << "\t//Replication Functions for Attributes\n";

	int counter2 = 0;
	for (const std::string& str : vAttributeNames)
	{	
		fileStreamO << "\t" << "UFUNCTION()\n"
			<< "\t" << "virtual void OnRep_" << str << "(const FGameplayAttributeData& Old" << str << ");\n";

		if (vUseMaxValueAttribute.at(counter2))
		{
			fileStreamO << "\t" << "UFUNCTION()\n"
				<< "\t" << "virtual void OnRep_Max" << str << "(const FGameplayAttributeData& OldMax" << str << ");\n";
		}

		counter2++;
	}

	fileStreamO << "\n};\n";

	fileStreamO.close();
}

//FN: Make function Post GE Exec used in GASAttribute.cpp file
void writePostGEExec(std::ofstream& fileStream)
{
	int counter = 0;

	fileStream << "void UGASAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)\n"
		<< "{\n\tSuper::PostGameplayEffectExecute(Data);\n\n";

	//Write Post GE Section for each attribute. Make necessary changes if UseMaxValue is True
	for (const std::string& str : vAttributeNames)
	{
		if (vUseMaxValueAttribute.at(counter))
		{
			fileStream << "\t" << "if (Data.EvaluatedData.Attribute == Get" << str << "Attribute())\n"
				<< "\t" << "{\n"
				<< "\t\t" << "Set" << str << "(FMath::Clamp(Get" << str << "(), " << vAttributeMin.at(counter) << ".0f, " << "GetMax" << str << "()));\n"
				<< "\t\t" << str << "ChangeDelegate.Broadcast(Get" << str << "(), Data.EffectSpec.StackCount);\n"
				<< "\t}\n";
		}
		else
		{
			fileStream << "\t" << "if (Data.EvaluatedData.Attribute == Get" << str << "Attribute())\n"
				<< "\t" << "{\n"
				<< "\t\t" << "Set" << str << "(FMath::Clamp(Get" << str << "(), " << vAttributeMin.at(counter) << ".0f, " << vAttributeMax.at(counter) << ".0f));\n"
				<< "\t\t" << str << "ChangeDelegate.Broadcast(Get" << str << "(), Data.EffectSpec.StackCount);\n"
				<< "\t}\n";
		}
		counter++;
	}

	fileStream << "}\n\n";
}

//FN: Make Replication functions used in GASAttribute.cpp file
void writeLifeRepProp(std::ofstream& fileStream)
{
	int counter = 0;

	fileStream << "void UGASAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const\n"
		<< "{\n" << "\tSuper::GetLifetimeReplicatedProps(OutLifetimeProps);\n";

	//Write Rep Conditional functions.  Make necessary changes if UseMaxValue is True
	for (const std::string& str : vAttributeNames)
	{
		fileStream << "\t" << "DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, " << str << ", COND_None, REPNOTIFY_Always);\n";

		if (vUseMaxValueAttribute.at(counter))
		{
			fileStream << "\t" << "DOREPLIFETIME_CONDITION_NOTIFY(UGASAttributeSet, Max" << str << ", COND_None, REPNOTIFY_Always);\n";
		}

		counter++;
	}

	fileStream << "}\n\n";
}

//FN: Make OnRep functions used in GASAttribute.cpp file
void writeOnReps(std::ofstream& fileStream)
{
	int counter = 0;

	//Write On_Rep functions.  Make necessary changes if UseMaxValue is True
	for (const std::string& str : vAttributeNames)
	{
		fileStream << "void UGASAttributeSet::OnRep_" << str << "(const FGameplayAttributeData& Old" << str << ")\n{\n"
			<< "\t" << "GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, " << str << ", Old" << str << ");\n"
			<< "}\n";
		if (vUseMaxValueAttribute.at(counter))
		{
			fileStream << "void UGASAttributeSet::OnRep_Max" << str << "(const FGameplayAttributeData& OldMax" << str << ")\n{\n"
				<< "\t" << "GAMEPLAYATTRIBUTE_REPNOTIFY(UGASAttributeSet, Max" << str << ", OldMax" << str << ");\n"
				<< "}\n";
		}

		counter++;
	}
}

//FN: Write Attributes to GASAttribute.cpp file
void writeAttrToCPP()
{
	UE_LOG(LogTemp, Warning, TEXT("=====>Modifying GASAttributeSet CPP File"));

	//Open file and override content
	std::ofstream fileStream;
	fileStream.open(outAttrPathCPP, std::ios_base::trunc);

	fileStream << "// Fill out your copyright notice in the Description page of Project Settings.\n"
		<< "#include \"GASAttributeSet.h\"\n"
		<< "#include \"Net/UnrealNetwork.h\"\n"
		<< "#include \"GameplayEffect.h\"\n"
		<< "#include \"GameplayEffectExtension.h\"\n"
		<< "\n\nUGASAttributeSet::UGASAttributeSet()\n"
		<< "{\n\n}\n\n";


	writePostGEExec(fileStream);
	writeLifeRepProp(fileStream);
	writeOnReps(fileStream);


	fileStream.close();
}

//FN: Make Event Delagtes used in GASCharacter.h
void writeEventTriggers(std::ofstream& fileStream)
{
	fileStream << "\t//Native functions for Attribute Change Delagates. Using UFUNCTION() to expose to BP Side Link\n\n";

	//Read Vector and construct line
	for (const std::string& str : vAttributeNames)
	{
		fileStream << "\t" << "UFUNCTION()\n"
			<< "\t" << "virtual void On" << str << "ChangedNative(float " << str << ", int32 StackCount);\n";
	}
}

//FN: Make Event Delegate Binds to BP side used in GASCharacter.h
void writeEventBindsBP(std::ofstream& fileStream)
{
	fileStream << "\n\n\t//******Event that bind to native events and are implemented in BPs********\n\n";

	//Read Vector and construct line
	for (const std::string& str : vAttributeNames)
	{
		fileStream << "\t" << "//Event Trigger On " << str << " Change\n"
			<< "\t" << "UFUNCTION(BlueprintImplementableEvent, Category = \"GASGameplayAbility\")\n"
			<< "\t" << "void On" << str << "Change(float " << str << ", int32 StackCount);\n";
	}
}

//FN: Make attribute Getters used in GASCharacter.h
void writeGettersFN(std::ofstream& fileStream)
{
	fileStream << "\n\n\t//*******Ability Values Getter Functions**********\n\n";
	int counter = 0;

	//Read Vector and construct line
	for (const std::string& str : vAttributeNames)
	{
		if (vUseMaxValueAttribute.at(counter))
		{
			fileStream << "\t" << "//Getter for " << str << " Values\n"
				<< "\t" << "UFUNCTION(BlueprintPure, Category = \"GASGameplayAbility\")\n"
				<< "\t" << "void Get" << str << "Values(float& " << str << ", float& Max" << str << ");\n";
		}
		else
		{
			fileStream << "\t" << "//Getter for " << str << " Values\n"
				<< "\t" << "UFUNCTION(BlueprintPure, Category = \"GASGameplayAbility\")\n"
				<< "\t" << "void Get" << str << "Value(float& " << str << ");\n";
		}

		counter++;
	}
}

//FN: Make attribute Setters used in GASCharacter.h
void writeSetterFN(std::ofstream& fileStream)
{
	fileStream << "\n\n\t//*******Ability Values Setter Functions**********\n\n";
	int counter = 0;

	//Read Vector and construct line
	for (const std::string& str : vAttributeNames)
	{
		if (vUseMaxValueAttribute.at(counter))
		{
			fileStream << "\t" << "//Setter for " << str << " Values\n"
				<< "\t" << "UFUNCTION(BlueprintCallable, Category = \"GASGameplayAbility\")\n"
				<< "\t" << "void Set" << str << "Values(float New" << str << ", float NewMax" << str << ");\n";
		}
		else
		{
			fileStream << "\t" << "//Setter for " << str << " Values\n"
				<< "\t" << "UFUNCTION(BlueprintCallable, Category = \"GASGameplayAbility\")\n"
				<< "\t" << "void Set" << str << "Value(float New" << str << ");\n";
		}

		counter++;
	}
}

//FN: Write changes GASCharacter.h
void writeAttrToCharHeader()
{
	UE_LOG(LogTemp, Warning, TEXT("=====>Modifying GASCharacter Header File"));

	std::vector<std::string> fileBufferPre;

	//read original file to buffer and define variables for regex
	std::ifstream fileStream(outCharPathHeader);
	std::string lineBuffer = "";
	bool breakFlag = false;
	std::smatch match;
	std::regex regexPattern("//==PATTERN==");

	//Store non changed lines to buffer until a PATTERN is found
	while (getline(fileStream, lineBuffer))
	{
		fileBufferPre.push_back(lineBuffer);

		while (regex_search(lineBuffer, match, regexPattern))
		{
			for (auto x : match)
			{
				breakFlag = true;
			}
			break;
		}

		if (breakFlag)
		{
			break;
		}
	}

	fileStream.close();

	//Open file for write
	std::ofstream fileStreamO(outCharPathHeader, std::ios_base::trunc);

	for (const std::string& str : fileBufferPre)
	{
		fileStreamO << str << std::endl;
	}

	writeEventTriggers(fileStreamO);
	writeEventBindsBP(fileStreamO);
	writeGettersFN(fileStreamO);
	writeSetterFN(fileStreamO);

	fileStreamO << "\n\n};\n";

	fileStreamO.close();

}

// FN: Make rest of Constructor of GASCharacter.cpp file
void writeCharRepMode(std::ofstream & fileStream)
{
	if (vRepMode == "Full")
	{
		fileStream << "\n\tAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);";
	}
	else if (vRepMode == "Mixed")
	{
		fileStream << "\n\tAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);";
	}
	else if (vRepMode == "Minimal")
	{
		fileStream << "\n\tAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);";
	}
	

	fileStream << "\n\n}\n";
}


//FN: Make BeginPlay of GASCharacter.cpp file
void writeCharConstructor(std::ofstream& fileStream)
{
	fileStream << "\n\n// Called when the game starts or when spawned\n"
		<< "void AGASCharacter::BeginPlay()\n{\n"
		<< "\t" << "Super::BeginPlay();\n\n"
		<< "\t" << "if (AbilitySystemComponent)\n\t{\n"
		<< "\t" << "\t" << "if (AbilitySystemComponent->DefaultStartingData.Num() > 0 && AbilitySystemComponent->DefaultStartingData[0].Attributes != NULL && AbilitySystemComponent->DefaultStartingData[0].DefaultStartingTable != NULL)\n\t\t{\n"
		<< "\t" << "\t" << "\t" << "//Link Attribute Set to Ability System Component\n"
		<< "\t" << "\t" << "\t" << "AttributeSetVar = AbilitySystemComponent->GetSet<UGASAttributeSet>();\n\n"
		<< "\t" << "\t" << "\t" << "//Bindings for Attribute Change Delegates\n";

	for (const std::string& str : vAttributeNames)
	{
		fileStream << "\t" << "\t" << "\t" << "const_cast<UGASAttributeSet*>(AttributeSetVar)->" << str << "ChangeDelegate.AddDynamic(this, &AGASCharacter::On" << str << "ChangedNative);\n";
	}
	fileStream << "\t\t}\n";
	fileStream << "\t}\n" << "}\n\n";
}

//FN: Make Delagete Binds of GASCharacter.cpp file
void writeCharBindFN(std::ofstream& fileStream)
{

	for (const std::string& str : vAttributeNames)
	{
		fileStream << "void AGASCharacter::On" << str << "ChangedNative(float " << str << ", int32 StackCount)\n{\n"
			<< "\t" << "On" << str << "Change(" << str << ", StackCount);\n"
			<< "}\n\n";
	}
}

//FN: Make Attribute Value Getter of GASCharacter.cpp file
void writeCharGetterFN(std::ofstream& fileStream)
{
	int counter = 0;

	for (const std::string& str : vAttributeNames)
	{
		if (vUseMaxValueAttribute.at(counter))
		{
			fileStream << "void AGASCharacter::Get" << str << "Values(float& " << str << ", float& Max" << str << ")\n{\n"
				<< "\t" << "if (AttributeSetVar)\n\t{\n"
				<< "\t\t" << str << " = AttributeSetVar->Get" << str << "();\n"
				<< "\t\t" << "Max" << str << " = AttributeSetVar->GetMax" << str << "();\n"
				<< "\t}\n}\n\n";
		}
		else
		{
			fileStream << "void AGASCharacter::Get" << str << "Value(float& " << str << ")\n{\n"
				<< "\t" << "if (AttributeSetVar)\n\t{\n"
				<< "\t\t" << str << " = AttributeSetVar->Get" << str << "();\n"
				<< "\t}\n}\n\n";
		}
		
		counter++;
	}
}

//FN: Make Attribute Value Setter of GASCharacter.cpp file
void writeCharSetterFN(std::ofstream& fileStream)
{
	int counter = 0;

	for (const std::string& str : vAttributeNames)
	{
		if (vUseMaxValueAttribute.at(counter))
		{
			fileStream << "void AGASCharacter::Set" << str << "Values(float New" << str << ", float NewMax" << str << ")\n{\n"
				<< "\t" << "if (AttributeSetVar)\n\t{\n"
				<< "\t\t" << "AbilitySystemComponent->ApplyModToAttribute(AttributeSetVar->Get" << str << "Attribute(), EGameplayModOp::Override, New" << str << ");\n"
				<< "\t\t" << "AbilitySystemComponent->ApplyModToAttribute(AttributeSetVar->GetMax" << str << "Attribute(), EGameplayModOp::Override, NewMax" << str << "); \n"
				<< "\t}\n}\n\n";
		}
		else
		{
			fileStream << "void AGASCharacter::Set" << str << "Value(float New" << str << ")\n{\n"
				<< "\t" << "if (AttributeSetVar)\n\t{\n"
				<< "\t\t" << "AbilitySystemComponent->ApplyModToAttribute(AttributeSetVar->Get" << str << "Attribute(), EGameplayModOp::Override, New" << str << ");\n"
				<< "\t}\n}\n\n";
		}

		counter++;
	}
}

//FN: Write changes GASCharacter.cpp
void writeAttrToCharCPP()
{
	UE_LOG(LogTemp, Warning, TEXT("=====>Modifying GASCharacter CPP File"));

	std::vector<std::string> fileBufferPre;
	std::vector<std::string> fileBufferPost;

	//read original file to buffer  and declare variable for regex
	std::ifstream fileStream(outCharPathCPP);
	std::string lineBuffer = "";
	bool breakFlag = false;
	std::smatch match;
	std::regex regexPattern("//==PATTERN==");
	std::regex regexPatternStart("//==PATTERNSTART==");
	std::regex regexPatternStop("//==PATTERNSTOP==");

	std::regex Pattern = regexPatternStart;
	bool patternSwitch = false;

	//Read file in two sections. Both sections are separated by a PATTERN. These sections are not modified
	while (getline(fileStream, lineBuffer))
	{
		fileBufferPre.push_back(lineBuffer);

		while (regex_search(lineBuffer, match, regexPattern))
		{
			for (auto x : match)
			{
				breakFlag = true;
			}
			break;
		}

		if (breakFlag)
		{
			break;
		}
	}

	breakFlag = false;

	while (getline(fileStream, lineBuffer))
	{
		if (patternSwitch)
		{
			fileBufferPost.push_back(lineBuffer);
		}

		while (regex_search(lineBuffer, match, Pattern))
		{
			for (auto x : match)
			{
				if (!patternSwitch)
				{
					Pattern = regexPatternStop;
				}
				if (patternSwitch)
				{
					breakFlag = true;
				}
				patternSwitch = true;

			}
			break;
		}

		if (breakFlag)
		{
			break;
		}
	}

	fileStream.close();

	//Open file in WRITE mode and commit changes
	std::ofstream fileStreamO(outCharPathCPP, std::ios_base::trunc);

	for (const std::string& str : fileBufferPre)
	{
		fileStreamO << str << std::endl;
	}

	writeCharRepMode(fileStreamO);
	writeCharConstructor(fileStreamO);

	fileStreamO << "//==PATTERNSTART==\n";

	for (const std::string& str : fileBufferPost)
	{
		fileStreamO << str << std::endl;
	}


	writeCharBindFN(fileStreamO);
	writeCharGetterFN(fileStreamO);
	writeCharSetterFN(fileStreamO);

	fileStreamO.close();
}

//**************************************************Plugin Code***************************************************

static const FName GASAssociateHelperTabName("GASAssociateHelper");

#define LOCTEXT_NAMESPACE "FGASAssociateHelperModule"

void FGASAssociateHelperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGASAssociateHelperStyle::Initialize();
	FGASAssociateHelperStyle::ReloadTextures();

	FGASAssociateHelperCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGASAssociateHelperCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FGASAssociateHelperModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGASAssociateHelperModule::RegisterMenus));

	RegisterSettings();
}

void FGASAssociateHelperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGASAssociateHelperStyle::Shutdown();

	FGASAssociateHelperCommands::Unregister();

	if (UObjectInitialized())
	{
		UnregisterSettings();
	}
}

// ************************************************ This is called on button press ************************************************
void FGASAssociateHelperModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::FText::Format(
		LOCTEXT("PluginButtonDialogText", "Press YES to make changes to Files. Existing code will be lost. Compile to commit changes!\nSee Output Log for Details\nChange File Set: {0}, {1}"),
		FText::FromString(TEXT("GASAttributeSet")),
		FText::FromString(TEXT("GASCharacter"))
	);
	//Custom code goes here
	
	EAppReturnType::Type UserResponse = FMessageDialog::Open(EAppMsgType::YesNo, DialogText);

	UE_LOG(LogTemp, Warning, TEXT("=====>Begin Modifications<====="));
	UE_LOG(LogTemp, Warning, TEXT("Config file path: %s"), UTF8_TO_TCHAR(configFilePath.c_str()));
	UE_LOG(LogTemp, Warning, TEXT("Attribute Set Header file path: %s"), UTF8_TO_TCHAR(outAttrPathHeader.c_str()));
	UE_LOG(LogTemp, Warning, TEXT("Attribute CPP file path: %s"), UTF8_TO_TCHAR(outAttrPathCPP.c_str()));
	UE_LOG(LogTemp, Warning, TEXT("Character Header file path: %s"), UTF8_TO_TCHAR(outCharPathHeader.c_str()));
	UE_LOG(LogTemp, Warning, TEXT("Character CPP file path: %s"), UTF8_TO_TCHAR(outCharPathCPP.c_str()));
	
	if (UserResponse == EAppReturnType::Yes)
	{
		try
		{
			UE_LOG(LogTemp, Warning, TEXT("=====>Opeing .ini File"));
			std::string fileBufferConfig = openFile(configFilePath);

			UE_LOG(LogTemp, Warning, TEXT("=====>Clearing Previous Array Data"));
			vAttributeNames.clear();
			vAttributeMax.clear();
			vAttributeMin.clear();
			vUseMaxValueAttribute.clear();

			//Call FNs and start file modifications
			fillAttributeName(fileBufferConfig);
			// Revert to default Attributes if we did not find any valid syntax attribute in config file
			if (vAttributeNames.size() <= 0)
			{
				throw(vAttributeNames.size() <= 0);
			}
			fillAttributeMins(fileBufferConfig);
			fillAttributeMax(fileBufferConfig);
			fillAttributeBools(fileBufferConfig);
			fillAttributeRepMode(fileBufferConfig);
			writeAttrToHeader();
			writeAttrToCPP();
			writeAttrToCharHeader();
			writeAttrToCharCPP();
			UE_LOG(LogTemp, Warning, TEXT("=====>Files Modified<====="));
		}
		catch (...)
		{
			UE_LOG(LogTemp, Warning, TEXT("=====>Failed to open config file. Add some attributes and try again."));
			makeDefaultFile(configFilePath);
			UE_LOG(LogTemp, Warning, TEXT("=====>Added Default Attributes of Health and Max Health"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("=====>Files NOT Modified!<====="));
	}
}

void FGASAssociateHelperModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FGASAssociateHelperCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FGASAssociateHelperCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

bool FGASAssociateHelperModule::HandleSettingsSaved()
{
	UGASHAttributeWizard* Settings = GetMutableDefault<UGASHAttributeWizard>();
	bool ResaveSettings = false;

	// You can put any validation code in here and resave the settings in case an invalid
	// value has been entered

	if (ResaveSettings)
	{
		Settings->SaveConfig();
	}

	return true;
}

void FGASAssociateHelperModule::RegisterSettings()
{
	// Registering some settings is just a matter of exposijg the default UObject of
	// your desired class, feel free to add here all those settings you want to expose
	// to your LDs or artists.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Create the new category
		ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

		SettingsContainer->DescribeCategory("GAS Associate",
			LOCTEXT("RuntimeWDCategoryName", "GAS Associate"),
			LOCTEXT("RuntimeWDCategoryDescription", "Attribute Wizard for GAS Associate Helper"));

		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "GAS Associate", "Attributes Helper Wizard",
			LOCTEXT("RuntimeGeneralSettingsName", "Attribute Helper Wizard"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Attribute Wizard for GAS Associate"),
			GetMutableDefault<UGASHAttributeWizard>()
		);

		// Register the save handler to your settings, you might want to use it to
		// validate those or just act to settings changes.
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FGASAssociateHelperModule::HandleSettingsSaved);
		}
	}
}

void FGASAssociateHelperModule::UnregisterSettings()
{
	// Ensure to unregister all of your registered settings here, hot-reload would
	// otherwise yield unexpected results.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "GAS Associate", "Attribute Helper Wizard");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGASAssociateHelperModule, GASAssociateHelper)
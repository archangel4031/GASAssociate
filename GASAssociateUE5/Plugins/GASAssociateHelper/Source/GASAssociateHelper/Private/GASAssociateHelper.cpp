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
#include "HelperFunctions.h"



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

	//Init Helper Function Object
	HelperFunctions Helper;

	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::FText::Format(
		LOCTEXT("PluginButtonDialogText", "Press YES to make changes to Files. Existing code will be lost. Compile to commit changes!\nSee Output Log for Details\nChange File Set: {0}, {1}"),
		FText::FromString(TEXT("GASAttributeSet")),
		FText::FromString(TEXT("GASCharacter"))
	);
	//Custom code goes here
	
	EAppReturnType::Type UserResponse = FMessageDialog::Open(EAppMsgType::YesNo, DialogText);

	UE_LOG(LogTemp, Warning, TEXT("=====>[BEGIN] GAS Associate Helper Info [BEGIN] <====="));
	UE_LOG(LogTemp, Warning, TEXT("Config file path: %s"), UTF8_TO_TCHAR(Helper.configFilePath.c_str()));
	UE_LOG(LogTemp, Warning, TEXT("Attribute Set Header file path: %s"), UTF8_TO_TCHAR(Helper.outAttrPathHeader.c_str()));
	UE_LOG(LogTemp, Warning, TEXT("Attribute CPP file path: %s"), UTF8_TO_TCHAR(Helper.outAttrPathCPP.c_str()));
	UE_LOG(LogTemp, Warning, TEXT("Character Header file path: %s"), UTF8_TO_TCHAR(Helper.outCharPathHeader.c_str()));
	UE_LOG(LogTemp, Warning, TEXT("Character CPP file path: %s"), UTF8_TO_TCHAR(Helper.outCharPathCPP.c_str()));
	UE_LOG(LogTemp, Warning, TEXT("=====>[END] GAS Associate Helper Info [END]<====="));

	if (UserResponse == EAppReturnType::Yes)
	{
		try
		{
			UE_LOG(LogTemp, Warning, TEXT("=====>[i] Opeing .ini File<====="));
			std::string fileBufferConfig = Helper.openFile();

			UE_LOG(LogTemp, Warning, TEXT("=====>[i] Clearing Previous Array Data<====="));
			Helper.clearVectors();

			//Call FNs and start file modifications
			Helper.fillAttributeName(fileBufferConfig);
			// Revert to default Attributes if we did not find any valid syntax attribute in config file
			if (Helper.vAttributeNames.size() <= 0)
			{
				throw(Helper.vAttributeNames.size() <= 0);
			}
			Helper.fillAttributeMins(fileBufferConfig);
			Helper.fillAttributeMax(fileBufferConfig);
			Helper.fillAttributeBools(fileBufferConfig);
			Helper.fillAttributeRepMode(fileBufferConfig);
			Helper.fillALSSupportValue(fileBufferConfig);
			Helper.writeAttrToHeader();
			Helper.writeAttrToCPP();
			Helper.writeAttrToCharHeader();
			Helper.writeAttrToCharCPP();
			Helper.updateCSAndUplugins();
			UE_LOG(LogTemp, Warning, TEXT("=====>[i] Files Modified<====="));
		}
		catch (...)
		{
			UE_LOG(LogTemp, Warning, TEXT("=====>[e] Failed to open config file. Add some attributes and try again."));
			Helper.makeDefaultFile();
			UE_LOG(LogTemp, Warning, TEXT("=====>[e] Added Default Attributes of Health and Max Health"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("=====>[i] Files NOT Modified!<====="));
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
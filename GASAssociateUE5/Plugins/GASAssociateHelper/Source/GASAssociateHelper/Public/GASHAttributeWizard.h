// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GASHAttributeWizard.generated.h"

/**
 * 
 */
UENUM()
enum class RepMode : uint8
{
	Full,
	Mixed,
	Minimal
};

USTRUCT()
struct FAttributeSettings
{
	GENERATED_BODY()

	// Attribute Name that will be used through out the code
	UPROPERTY(config, EditAnywhere, Category = Custom)
	FString AttributeName;

	// Minimum Value of Attribute for clamping
	UPROPERTY(config, EditAnywhere, Category = Custom)
	int32 MinValue;

	// Maximum Value of Attribute for clamping
	UPROPERTY(config, EditAnywhere, Category = Custom)
	int32 MaxValue;

	// Ignore Max Value and Create an attribute like Max{AttributeName} for clamping
	UPROPERTY(config, EditAnywhere, Category = Custom)
	bool UseMaxValueAttribute;
};


UCLASS(config = GASAttributes, defaultconfig)
class GASASSOCIATEHELPER_API UGASHAttributeWizard : public UObject
{
	GENERATED_BODY()

public:
	UGASHAttributeWizard(const FObjectInitializer& ObjectInitializer);

	// Replication mode for Ability System
	UPROPERTY(config, EditAnywhere, Category = CustomAttributesSetting)
	RepMode AttributeRepMode;

	// Attributes Array
	UPROPERTY(config, EditAnywhere, Category = CustomAttributesSetting)
	TArray<FAttributeSettings> AttributeList;
	
	// Support ALSV4 for UE5.1+
	UPROPERTY(config, EditAnywhere, Category = AdvancedLocomotionSupport)
	bool AddSupportForALS;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GASHAttributeWizard.generated.h"

/**
 * 
 */

USTRUCT()
struct FAttributeSettings
{
	GENERATED_BODY()

	UPROPERTY(config, EditAnywhere, Category = Custom)
	FString AttributeName;

	UPROPERTY(config, EditAnywhere, Category = Custom)
	int32 MinValue;

	UPROPERTY(config, EditAnywhere, Category = Custom)
	int32 MaxValue;

};


UCLASS(config = GASAttributes, defaultconfig)
class GASASSOCIATEHELPER_API UGASHAttributeWizard : public UObject
{
	GENERATED_BODY()

public:
	UGASHAttributeWizard(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(config, EditAnywhere, Category = CustomAttributes)
	TArray<FAttributeSettings> AttributeList;
	
};

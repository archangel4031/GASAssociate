// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GetAbilityInfo.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FGameplayAbilityInfo
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASGameplayAbility")
	float AbilityCooldownDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASGameplayAbility")
	TArray<float> AbilityCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASGameplayAbility")
	TArray<FString> AbilityCostName;

	FGameplayAbilityInfo();
	FGameplayAbilityInfo(float InAbilityCooldownDuration,
		TArray<float> InAbilityCost,
		TArray<FString> InAbilityCostName);
};
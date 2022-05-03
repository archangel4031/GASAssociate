// Fill out your copyright notice in the Description page of Project Settings.


#include "GetAbilityInfo.h"


FGameplayAbilityInfo::FGameplayAbilityInfo()
{
	AbilityCooldownDuration = 0.0f;
	AbilityCost.Empty();
	AbilityCostName.Empty();
}

FGameplayAbilityInfo::FGameplayAbilityInfo(float InAbilityCooldownDuration, TArray<float> InAbilityCost, TArray<FString> InAbilityCostName)
{
	AbilityCooldownDuration = InAbilityCooldownDuration;
	AbilityCost = InAbilityCost;
	AbilityCostName = InAbilityCostName;
}

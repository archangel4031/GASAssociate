// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GetAbilityInfo.h"
#include "GASGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASASSOCIATE_API UGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
	FGameplayAbilityInfo GetAbilityInfo();
	
};

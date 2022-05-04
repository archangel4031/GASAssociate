// Fill out your copyright notice in the Description page of Project Settings.


#include "GASGameplayAbility.h"


FGameplayAbilityInfo UGASGameplayAbility::GetAbilityInfo()
{
	//Store Cost and Cooldown GE Classes in Variables
	UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
	UGameplayEffect* CostEffect = GetCostGameplayEffect();
	
	//Proceed only if we have valid Cost and Cooldown Classes
	if (CooldownEffect && CostEffect)
	{
		//Create variables to store info
		float CooldownDuration = 0;
		TArray<float> Cost;
		TArray<FString> CostName;
		//Theses variables are needed as temp variables to add to TArray
		float SingleCost = 0.0f;
		FString SingleCostName;

		//Initialize the TArray as Empty Arrays
		Cost.Empty();
		CostName.Empty();

		//Get the Cooldown Duration Magnitude from GE
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(GetAbilityLevel(), CooldownDuration);
		
		//If we have atleast one modifier for Cost GE (I'm only dealing with Modifiers)
		if (CostEffect->Modifiers.Num() > 0)
		{
			//Iterate over Modifier Array and add to TArray
			for (auto EffectInfo : CostEffect->Modifiers)
			{
				EffectInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(GetAbilityLevel(), SingleCost);
				FGameplayAttribute CostAttribute = EffectInfo.Attribute;
				SingleCostName = CostAttribute.AttributeName;
				Cost.Add(SingleCost);
				CostName.Add(SingleCostName);

			}
		}
		return FGameplayAbilityInfo(CooldownDuration, Cost, CostName);
	}
	else
	{
		//Return Empty if Invalid
		return FGameplayAbilityInfo();
	}
}
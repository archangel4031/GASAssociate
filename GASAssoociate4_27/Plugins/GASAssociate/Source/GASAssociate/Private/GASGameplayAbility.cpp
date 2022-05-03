// Fill out your copyright notice in the Description page of Project Settings.


#include "GASGameplayAbility.h"


FGameplayAbilityInfo UGASGameplayAbility::GetAbilityInfo()
{
	UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
	UGameplayEffect* CostEffect = GetCostGameplayEffect();
	if (CooldownEffect && CostEffect)
	{
		float CooldownDuration = 0;
		TArray<float> Cost;
		TArray<FString> CostName;
		float SingleCost = 0.0f;
		FString SingleCostName;

		Cost.Empty();
		CostName.Empty();

		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1, CooldownDuration);
		
		if (CostEffect->Modifiers.Num() > 0)
		{
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
		return FGameplayAbilityInfo();
	}
}
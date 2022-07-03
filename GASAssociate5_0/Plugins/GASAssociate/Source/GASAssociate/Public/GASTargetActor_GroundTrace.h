// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_GroundTrace.h"
#include "GASTargetActor_GroundTrace.generated.h"

/**
 * 
 */
UCLASS()
class GASASSOCIATE_API AGASTargetActor_GroundTrace : public AGameplayAbilityTargetActor_GroundTrace
{
	GENERATED_BODY()

public:
	AGASTargetActor_GroundTrace();

	virtual void Tick(float DeltaSeconds) override;

	AGameplayAbilityWorldReticle* MyReticleActor;

	FHitResult TraceHitResults;

	virtual void ConfirmTargetingAndContinue() override;
	virtual void CancelTargeting() override;

	virtual AGameplayAbilityWorldReticle* SpawnReticleActor(FVector Location, FRotator Rotation);
	virtual void DestroyReticleActors();

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GASTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class GASASSOCIATE_API AGASTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:

	AGASTargetActor();

	virtual void Tick(float DeltaSeconds) override;

	AGameplayAbilityWorldReticle* MyReticleActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "GASGameplayAbility")
	float TraceRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASGameplayAbility")
	FHitResult TraceHitResults;

	bool LineTraceFN(FHitResult& TraceHitResult);

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void CancelTargeting() override;

	virtual AGameplayAbilityWorldReticle* SpawnReticleActor(FVector Location, FRotator Rotation);
	virtual void DestroyReticleActors();

};

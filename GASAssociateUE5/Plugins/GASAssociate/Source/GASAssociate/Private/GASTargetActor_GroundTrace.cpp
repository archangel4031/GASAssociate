// Fill out your copyright notice in the Description page of Project Settings.


#include "GASTargetActor_GroundTrace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

AGASTargetActor_GroundTrace::AGASTargetActor_GroundTrace()
{
	PrimaryActorTick.bCanEverTick = true;
	bDestroyOnConfirmation = true;
	ShouldProduceTargetDataOnServer = true;
	MyReticleActor = nullptr;
}

void AGASTargetActor_GroundTrace::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TraceHitResults = PerformTrace(PrimaryPC->GetPawn());

	if (MyReticleActor)
	{
		MyReticleActor->SetActorLocation(TraceHitResults.TraceEnd, false, nullptr, ETeleportType::None);
	}
}

void AGASTargetActor_GroundTrace::ConfirmTargetingAndContinue()
{
	FHitResult HitResult;

	HitResult = PerformTrace(PrimaryPC->GetPawn());
	FGameplayAbilityTargetDataHandle TargetData;
	TargetData = StartLocation.MakeTargetDataHandleFromHitResult(OwningAbility, HitResult);
	if (TargetData != nullptr)
	{
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}
	DestroyReticleActors();
}

void AGASTargetActor_GroundTrace::CancelTargeting()
{
	Super::CancelTargeting();
	DestroyReticleActors();
}

AGameplayAbilityWorldReticle* AGASTargetActor_GroundTrace::SpawnReticleActor(FVector Location, FRotator Rotation)
{
	//Check if Reticle Class is not empty
	if (ReticleClass)
	{
		//Spawn only of MyReticleActor is empty
		if (!MyReticleActor)
		{
			AGameplayAbilityWorldReticle* SpawnedReticleActor = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(ReticleClass, Location, Rotation);
			//When we successfully spawn the actor
			if (SpawnedReticleActor)
			{
				SpawnedReticleActor->InitializeReticle(this, PrimaryPC, ReticleParams);
				//SpawnedReticleActor->SetReplicates(false);'
				return SpawnedReticleActor;
			}
		}
		else
		{
			return MyReticleActor;
		}
	}
	return nullptr;
}

void AGASTargetActor_GroundTrace::DestroyReticleActors()
{
	if (MyReticleActor)
	{
		MyReticleActor->Destroy();
	}
}


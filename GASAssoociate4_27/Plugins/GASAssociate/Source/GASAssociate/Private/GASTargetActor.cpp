// Fill out your copyright notice in the Description page of Project Settings.


#include "GASTargetActor.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

AGASTargetActor::AGASTargetActor()
{
	PrimaryActorTick.bCanEverTick = true;
	TraceRange = 1500.0f;
	bDestroyOnConfirmation = true;
	ShouldProduceTargetDataOnServer = true;
	MyReticleActor = nullptr;
}

void AGASTargetActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	LineTraceFN(TraceHitResults);

	if (MyReticleActor)
	{
		if (TraceHitResults.bBlockingHit)
		{
			MyReticleActor->SetActorLocation(TraceHitResults.ImpactPoint, false, nullptr, ETeleportType::None);
		}
		else
		{
			MyReticleActor->SetActorLocation(TraceHitResults.TraceEnd, false, nullptr, ETeleportType::None);
		}
	}
}

bool AGASTargetActor::LineTraceFN(FHitResult& TraceHitResult)
{
	FVector ViewPoint;
	FRotator ViewRotation;
	MasterPC->GetPlayerViewPoint(ViewPoint, ViewRotation);
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	APawn* MasterPawn = MasterPC->GetPawn();
	if (MasterPawn)
	{
		QueryParams.AddIgnoredActor(MasterPawn->GetUniqueID());
	}

	bool TryTrace = GetWorld()->LineTraceSingleByChannel(TraceHitResult, ViewPoint, ViewPoint + ViewRotation.Vector() * TraceRange, ECC_Visibility, QueryParams);

	return TryTrace;
}

void AGASTargetActor::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
	MyReticleActor = SpawnReticleActor(GetActorLocation(), GetActorRotation());
}

void AGASTargetActor::ConfirmTargetingAndContinue()
{
	FHitResult HitResult;
	bool TryTrace = LineTraceFN(HitResult);
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

void AGASTargetActor::CancelTargeting()
{
	Super::CancelTargeting();
	DestroyReticleActors();
}

AGameplayAbilityWorldReticle* AGASTargetActor::SpawnReticleActor(FVector Location, FRotator Rotation)
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
				SpawnedReticleActor->InitializeReticle(this, MasterPC, ReticleParams);
				//SpawnedReticleActor->SetReplicates(false);
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

void AGASTargetActor::DestroyReticleActors()
{
	if (MyReticleActor)
	{
		MyReticleActor->Destroy();
	}
}


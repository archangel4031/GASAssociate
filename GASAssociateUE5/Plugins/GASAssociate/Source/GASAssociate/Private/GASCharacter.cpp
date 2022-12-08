// Fill out your copyright notice in the Description page of Project Settings.


#include "GASCharacter.h"
#include "GASAttributeSet.h"
#include "GASAbilitySystemComponent.h"

// Sets default values
AGASCharacter::AGASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComponent->SetIsReplicated(true);
	//Options for SetReplicationMode available are Full, Minimal, Mixed (Mixed is used for easier implementation of Gameplay Cues for replication via Gameplay Effect)
	//For handling Gameplay Cues locally, See GASAbilitySystemComponent.h file's comments

//==PATTERN==

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

}


// Called when the game starts or when spawned
void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
{
		//Link Attribute Set to Ability System Component
		AttributeSetVar = AbilitySystemComponent->GetSet<UGASAttributeSet>();

		//Bindings for Attribute Change Delegates
		const_cast<UGASAttributeSet*>(AttributeSetVar)->HealthChangeDelegate.AddDynamic(this, &AGASCharacter::OnHealthChangedNative);
		const_cast<UGASAttributeSet*>(AttributeSetVar)->ManaChangeDelegate.AddDynamic(this, &AGASCharacter::OnManaChangedNative);
		const_cast<UGASAttributeSet*>(AttributeSetVar)->AttackPowerChangeDelegate.AddDynamic(this, &AGASCharacter::OnAttackPowerChangedNative);
	}
}

//==PATTERNSTART==

// Called every frame
void AGASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGASCharacter::InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToGet, AbilityLevel, 0));
	}
}

void AGASCharacter::InitializeAbilityMulti(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire, int32 AbilityLevel)
{
	for (TSubclassOf<UGameplayAbility> AbilityItem : AbilitiesToAcquire)
	{
		InitializeAbility(AbilityItem, AbilityLevel);
	}
}

void AGASCharacter::Server_InitializeAbility_Implementation(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToGet, AbilityLevel, 0));
	}
}

void AGASCharacter::Server_InitializeAbilityMulti(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire, int32 AbilityLevel)
{
	for (TSubclassOf<UGameplayAbility> AbilityItem : AbilitiesToAcquire)
	{
		Server_InitializeAbility(AbilityItem, AbilityLevel);
	}
}

FGameplayAbilityInfo AGASCharacter::GetAbilityInfoFromAbilityClass(TSubclassOf<UGameplayAbility> AbilityClass, int32 AtAbilityLevel)
{
	//Create a default GA Class Object to retrieve info
	UGameplayAbility* AbilityCDO = AbilityClass.GetDefaultObject();
	
	if (AbilityCDO)
	{
		//Store Cost and Cooldown GE Classes in Variables
		UGameplayEffect* CooldownEffect = AbilityCDO->GetCooldownGameplayEffect();
		UGameplayEffect* CostEffect = AbilityCDO->GetCostGameplayEffect();
		
		//Proceed only if we have valid Cost and Cooldown Classes
		if (CostEffect && CooldownEffect)
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
			CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(AtAbilityLevel, CooldownDuration);

			//If we have atleast one modifier for Cost GE (I'm only dealing with Modifiers, not other types like Exec Calculations etc.)
			if (CostEffect->Modifiers.Num() > 0)
			{
				//Iterate over Modifier Array and add to TArray
				for (auto EffectInfo : CostEffect->Modifiers)
				{
					EffectInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(AtAbilityLevel, SingleCost);
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
	else
	{
		//Return Empty if Invalid
		return FGameplayAbilityInfo();
	}
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAbilityMulti(InitialAbilities, 0);
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AGASCharacter::RemoveAbilityWithTags(FGameplayTagContainer TagContainer)
{
	if (AbilitySystemComponent)
	{
		TArray<FGameplayAbilitySpec*> MatchingAbilities;
		AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilities, true);
		for (FGameplayAbilitySpec* Spec : MatchingAbilities)
		{
			AbilitySystemComponent->ClearAbility(Spec->Handle);
		}
	}
}

void AGASCharacter::ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 NewLevel)
{
	if (AbilitySystemComponent)
	{
		TArray<FGameplayAbilitySpec*> MatchingAbility;
		AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbility, true);
		for (FGameplayAbilitySpec* Spec : MatchingAbility)
		{
			Spec->Level = NewLevel;
		}
	}
}

void AGASCharacter::CancelAbilityWithTags(FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAbilities(&WithTags, &WithoutTags, nullptr);
	}
}

void AGASCharacter::AddLooseGameplayTag(FGameplayTag TagToAdd)
{
	if (AbilitySystemComponent)
	{
		GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
		GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
	}
}

void AGASCharacter::RemoveLooseGameplayTags(FGameplayTag TagsToRemove)
{
	if (AbilitySystemComponent)
	{
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagsToRemove);
	}
}

void AGASCharacter::Server_RemoveAbilityWithTags_Implementation(FGameplayTagContainer TagContainer)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		TArray<FGameplayAbilitySpec*> MatchingAbilities;
		AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbilities, true);
		for (FGameplayAbilitySpec* Spec : MatchingAbilities)
		{
			AbilitySystemComponent->ClearAbility(Spec->Handle);
		}
	}
}

void AGASCharacter::Server_ChangeAbilityLevelWithTags_Implementation(FGameplayTagContainer TagContainer, int32 NewLevel)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		TArray<FGameplayAbilitySpec*> MatchingAbility;
		AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContainer, MatchingAbility, true);
		for (FGameplayAbilitySpec* Spec : MatchingAbility)
		{
			Spec->Level = NewLevel;
		}
	}
}

void AGASCharacter::Server_CancelAbilityWithTags_Implementation(FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAbilities(&WithTags, &WithoutTags, nullptr);
	}
}

void AGASCharacter::Server_AddLooseGameplayTag_Implementation(FGameplayTag TagToAdd)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
		GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
	}
}

void AGASCharacter::Server_RemoveLooseGameplayTags_Implementation(FGameplayTag TagsToRemove)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagsToRemove);
	}
}

//==PATTERNSTOP==
void AGASCharacter::OnHealthChangedNative(float Health, int32 StackCount)
{
	OnHealthChange(Health, StackCount);
}

void AGASCharacter::OnManaChangedNative(float Mana, int32 StackCount)
{
	OnManaChange(Mana, StackCount);
}

void AGASCharacter::OnAttackPowerChangedNative(float AttackPower, int32 StackCount)
{
	OnAttackPowerChange(AttackPower, StackCount);
}

void AGASCharacter::GetHealthValues(float& Health, float& MaxHealth)
{
	if (AttributeSetVar)
	{
		Health = AttributeSetVar->GetHealth();
		MaxHealth = AttributeSetVar->GetMaxHealth();
	}
}

void AGASCharacter::GetManaValues(float& Mana, float& MaxMana)
{
	if (AttributeSetVar)
	{
		Mana = AttributeSetVar->GetMana();
		MaxMana = AttributeSetVar->GetMaxMana();
	}
}

void AGASCharacter::GetAttackPowerValue(float& AttackPower)
{
	if (AttributeSetVar)
	{
		AttackPower = AttributeSetVar->GetAttackPower();
	}
}

void AGASCharacter::SetHealthValues(float NewHealth, float NewMaxHealth)
{
	if (AttributeSetVar)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSetVar->GetHealthAttribute(), EGameplayModOp::Override, NewHealth);
		AbilitySystemComponent->ApplyModToAttribute(AttributeSetVar->GetMaxHealthAttribute(), EGameplayModOp::Override, NewMaxHealth); 
	}
}

void AGASCharacter::SetManaValues(float NewMana, float NewMaxMana)
{
	if (AttributeSetVar)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSetVar->GetManaAttribute(), EGameplayModOp::Override, NewMana);
		AbilitySystemComponent->ApplyModToAttribute(AttributeSetVar->GetMaxManaAttribute(), EGameplayModOp::Override, NewMaxMana); 
	}
}

void AGASCharacter::SetAttackPowerValue(float NewAttackPower)
{
	if (AttributeSetVar)
	{
		AbilitySystemComponent->ApplyModToAttribute(AttributeSetVar->GetAttackPowerAttribute(), EGameplayModOp::Override, NewAttackPower);
	}
}


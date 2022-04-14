// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GASCharacter.generated.h"

UCLASS()
class GASASSOCIATE_API AGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Add Ability System Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGameplayAbility")
		class UGASAbilitySystemComponent* AbilitySystemComponent;

	//Add Attribute Set
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGameplayAbility")
		const class UGASAttributeSet* AttributeSetVar;

	//Add Variable for Initial Abilities (do not leave blank!)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGameplayAbility")
		TArray<TSubclassOf<class UGameplayAbility>> InitialAbilities;

	//Interface Function for Ability System Component Getter
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//Initialize Ability Single
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
		void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);

	//Initialize Ability Multi
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
		void InitializeAbilityMulti(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire, int32 AbilityLevel);

	//Server/Client Functions
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//********Ability Modifier Functions********

	//Remove Abilities with Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
		void RemoveAbilityWithTags(FGameplayTagContainer TagContainer);

	//Change Ability Level with Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
		void ChangeAbilityLevelWithTags(FGameplayTagContainer TagContainer, int32 NewLevel);

	//Cancel Ability With Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
		void CancelAbilityWithTags(FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags);

	//Add Loose Gameplay Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
		void AddLooseGameplayTag(FGameplayTag TagToAdd);

	//Remove Loose Gameplay Tag
	UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility")
		void RemoveLooseGameplayTags(FGameplayTag TagsToRemove);

	//==PATTERN==
	//Native functions for Attribute Change Delagates. Using UFUNCTION() to expose to BP Side Link

	UFUNCTION()
	virtual void OnHealthChangedNative(float Health, int32 StackCount);
	UFUNCTION()
	virtual void OnManaChangedNative(float Mana, int32 StackCount);


	//******Event that bind to native events and are implemented in BPs********

	//Event Trigger On Health Change
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	void OnHealthChange(float Health, int32 StackCount);
	//Event Trigger On Mana Change
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGameplayAbility")
	void OnManaChange(float Mana, int32 StackCount);


	//*******Ability Values Getter Functions**********

	//Getter for Health Values
	UFUNCTION(BlueprintPure, Category = "GASGameplayAbility")
	void GetHealthValue(float& Health);
	//Getter for Mana Values
	UFUNCTION(BlueprintPure, Category = "GASGameplayAbility")
	void GetManaValue(float& Mana);


};

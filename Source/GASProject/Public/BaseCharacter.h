// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "BaseCharacter.generated.h"

class UAbilitySystemComponent;
class UGAS_AttributeSet;
class UGameplayAbility;

UCLASS()
class GASPROJECT_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	// ABILITY SYSYTEM IMPLEMENTATION
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystemComponent;

	

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ABILITY SYSTEM IMPLEMENTATION
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Attributes")
	UGAS_AttributeSet* Attributes;
	
	// Effect that initializes our default attributes
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	// Default abilities to give to the player
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGAS_GameplayAbility>> DefaultAbilities;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	virtual void InitializeAttributes();
	virtual void GiveAbilities();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintCallable)
	void AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire);

	UFUNCTION()
	void OnHealthChanged(float Health, float MaxHealth);
	UFUNCTION(BlueprintImplementableEvent, Category = "BaseCharacter", meta = (DisplayName = "OnHealthChanged"))
	void BP_OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION()
    void OnManaChanged(float Mana, float MaxMana);
	UFUNCTION(BlueprintImplementableEvent, Category = "BaseCharacter", meta = (DisplayName = "OnManaChanged"))
    void BP_OnManaChanged(float Mana, float MaxMana);

	UFUNCTION()
    void OnStaminaChanged(float Stamina, float MaxStamina);
	UFUNCTION(BlueprintImplementableEvent, Category = "BaseCharacter", meta = (DisplayName = "OnStaminaChanged"))
    void BP_OnStaminaChanged(float Stamina, float MaxStamina);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "BaseCharacter", meta = (DisplayName = "Die"))
    void BP_Die();

	UFUNCTION(BlueprintCallable, Category = "BaseCharacter", meta = (DisplayName = "IsHostile"))
	bool IsHostile(ABaseCharacter* Character);
	
	UFUNCTION(BlueprintCallable)
	uint8 GetTeamID() const;

	// GAMEPLAY TAGS
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter", meta = (DisplayName = "AddGameplayTag"))
	void AddGameplayTag(FGameplayTag& TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "BaseCharacter", meta = (DisplayName = "RemoveGameplayTag"))
    void RemoveGameplayTag(FGameplayTag& TagToRemove);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseCharacter")
	FGameplayTag FullHealthTag;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HitStun"))
	void HitStun(float StunDuration = 4.f);

private:
	bool bIsDead;
	uint8 TeamID;

	void DetermineTeamID();
	void Dead();
	void DisableInputControl();
	void EnableInputControl();
	FTimerHandle StunTimerHandle;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GAS_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnParamChanged, float, Value, float, MaxValue);

UCLASS()
class GASPROJECT_API UGAS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UGAS_AttributeSet();

	// all of the attributes are replicated, when my health changes, I want everybody knows about that
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Health);

	UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, MaxHealth);

	UFUNCTION()
    virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Mana);

	UFUNCTION()
    virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, MaxMana);

	UFUNCTION()
    virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);	

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, Stamina);

	UFUNCTION()
    virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, MaxStamina);

	UFUNCTION()
    virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, AttackPower);

	UFUNCTION()
    virtual void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxAttackPower)
	FGameplayAttributeData MaxAttackPower;
	ATTRIBUTE_ACCESSORS(UGAS_AttributeSet, MaxAttackPower);

	UFUNCTION()
    virtual void OnRep_MaxAttackPower(const FGameplayAttributeData& OldAttackPower);

	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	FOnParamChanged OnHealthChanged;
	FOnParamChanged OnManaChanged;
	FOnParamChanged OnStaminaChanged;
	FOnParamChanged OnStrengthChanged;
	
};

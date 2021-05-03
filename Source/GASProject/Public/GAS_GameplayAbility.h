// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GASProject/GASProject.h"
#include "Abilities/GameplayAbility.h"
#include "GAS_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASPROJECT_API UGAS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
	UGAS_GameplayAbility();

	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;
};

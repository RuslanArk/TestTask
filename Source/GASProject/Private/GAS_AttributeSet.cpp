// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_AttributeSet.h"

#include "BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GeneratedCodeHelpers.h"
#include "GameplayEffectExtension.h"

UGAS_AttributeSet::UGAS_AttributeSet()	:
	  Health(200.f)
	, MaxHealth(200.f)
	, Mana(150.f)
	, MaxMana(150.f)
	, Stamina(200.f)
	, MaxStamina(200.f)
	, AttackPower(20.f)
	, MaxAttackPower(50.f)
{
	
}

void UGAS_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
}

void UGAS_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Health, OldHealth);
}

void UGAS_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxHealth, OldMaxHealth);
}

void UGAS_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Mana, OldMana);
}

void UGAS_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxMana, OldMaxMana);
}

void UGAS_AttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Stamina, OldStamina);
}

void UGAS_AttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxStamina, OldMaxStamina);
}

void UGAS_AttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, AttackPower, OldAttackPower);
}

void UGAS_AttributeSet::OnRep_MaxAttackPower(const FGameplayAttributeData& OldMaxAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxAttackPower, OldMaxAttackPower);
}

void UGAS_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UGAS_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGAS_AttributeSet, Health)))
	{
		OnHealthChanged.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());

		ABaseCharacter* OwningCharacter = Cast<ABaseCharacter>(GetOwningActor());
		if (OwningCharacter)
		{
			if (OwningCharacter->Attributes->GetHealth() >= OwningCharacter->Attributes->GetMaxHealth())
			{
				OwningCharacter->AddGameplayTag(OwningCharacter->FullHealthTag);
			}
			else
			{
				OwningCharacter->RemoveGameplayTag(OwningCharacter->FullHealthTag);
			}
		}
	}

	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UGAS_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGAS_AttributeSet, Mana)))
	{
		OnManaChanged.Broadcast(Mana.GetCurrentValue(), MaxMana.GetCurrentValue());
	}

	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UGAS_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGAS_AttributeSet, Stamina)))
	{
		OnHealthChanged.Broadcast(Stamina.GetCurrentValue(), MaxStamina.GetCurrentValue());
	}

	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UGAS_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UGAS_AttributeSet, AttackPower)))
	{
		OnHealthChanged.Broadcast(AttackPower.GetCurrentValue(), MaxAttackPower.GetCurrentValue());
	}
}

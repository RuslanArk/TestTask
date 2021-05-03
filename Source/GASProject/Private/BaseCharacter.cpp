// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "GAS_AbilitySystemComponent.h"
#include "GAS_AttributeSet.h"
#include "GASProject/GASProject.h"
#include "GAS_GameplayAbility.h"
#include <GameplayEffectTypes.h>


#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Internationalization/Text.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponent>(TEXT("AbilitySystemComp"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UGAS_AttributeSet>(TEXT("Attributes"));

	bIsDead = false;
	TeamID = 255;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	Attributes->OnHealthChanged.AddDynamic(this, &ABaseCharacter::OnHealthChanged);
	Attributes->OnManaChanged.AddDynamic(this, &ABaseCharacter::OnManaChanged);
	Attributes->OnStaminaChanged.AddDynamic(this, &ABaseCharacter::OnStaminaChanged);
	
	DetermineTeamID();
	AddGameplayTag(FullHealthTag);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}

}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		// ContextHandle is just some information about the context which we`re applying the effect 
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			// Apply the gameplay effect
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ABaseCharacter::GiveAbilities()
{
	// The server should always give abilities
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGAS_GameplayAbility>& StartupAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(
                FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Server GAS Init
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	InitializeAttributes();
	// Only server can Give Abilities
	GiveAbilities();
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	InitializeAttributes();
	// Client needs to bind input to the abilities
	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void ABaseCharacter::AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire)
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		if (AbilityToAquire)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToAquire, 1));
		}

		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void ABaseCharacter::OnHealthChanged(float Health, float MaxHealth)
{
	if (Health <= 0.f)
	{
		bIsDead = true;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Dead();
		BP_Die();
	}
	BP_OnHealthChanged(Health, MaxHealth);
}

void ABaseCharacter::OnManaChanged(float Mana, float MaxMana)
{
	BP_OnManaChanged(Mana, MaxMana);
}

void ABaseCharacter::OnStaminaChanged(float Stamina, float MaxStamina)
{
	BP_OnStaminaChanged(Stamina, MaxStamina);
}

bool ABaseCharacter::IsHostile(ABaseCharacter* Character)
{
	return TeamID != Character->GetTeamID();
}

uint8 ABaseCharacter::GetTeamID() const
{
	return TeamID;
}

void ABaseCharacter::AddGameplayTag(FGameplayTag& TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd, 1);
}

void ABaseCharacter::RemoveGameplayTag(FGameplayTag& TagToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);
}

void ABaseCharacter::HitStun(float StunDuration)
{
	DisableInputControl();
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &ABaseCharacter::EnableInputControl, StunDuration);
}

void ABaseCharacter::DetermineTeamID()
{
	if (GetController() && GetController()->IsPlayerController())
	{
		TeamID = 0;
	}
}

void ABaseCharacter::Dead()
{
	DisableInputControl();
	bIsDead = true;
}

void ABaseCharacter::DisableInputControl()
{
	AAIController* AIController = Cast<AAIController>(GetController()); 
	if (AIController)
	{
		AIController->GetBrainComponent()->StopLogic("IsDead");
		return;
	}
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->DisableInput(PlayerController);
	}	
}

void ABaseCharacter::EnableInputControl()
{
	if (!bIsDead)
	{
		AAIController* AIController = Cast<AAIController>(GetController()); 
		if (AIController)
		{
			AIController->GetBrainComponent()->RestartLogic();
			return;
		}
	
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			PlayerController->EnableInput(PlayerController);
		}
	}	
}




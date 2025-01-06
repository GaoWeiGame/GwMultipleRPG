// Fill out your copyright notice in the Description page of Project Settings.


#include "GwPlayerState.h"

#include "AbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GwMultipleRPG/GwMultipleRPG.h"
#include "GwMultipleRPG/AbilitySystem/GwAbilitySet.h"
#include "GwMultipleRPG/AbilitySystem/GwAbilitySystemComponent.h"
#include "GwMultipleRPG/AbilitySystem/Attributes/GwCombatSet.h"
#include "GwMultipleRPG/AbilitySystem/Attributes/GwHealthSet.h"
#include "GwMultipleRPG/Gameplay/Character/GwPawnExtensionComponent.h"
#include "GwMultipleRPG/Gameplay/GameModes/GwExperienceManagerComponent.h"
#include "GwMultipleRPG/Gameplay/GameModes/GwGameMode.h"
#include "Net/UnrealNetwork.h"


const FName AGwPlayerState::NAME_GwAbilityReady("GwAbilitiesReady");


AGwPlayerState::AGwPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UGwAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthSet = CreateDefaultSubobject<UGwHealthSet>("HealthSet");
	CombatSet = CreateDefaultSubobject<UGwCombatSet>("CombatSet");
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AGwPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGwPlayerState::SetPawnData(const UGwPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogGw, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	for (const UGwAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_GwAbilityReady);

	ForceNetUpdate();
}

void AGwPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		UGwExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UGwExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnGwExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void AGwPlayerState::Reset()
{
	Super::Reset();
}

void AGwPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
	if (UGwPawnExtensionComponent* PawnExtComp = UGwPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		PawnExtComp->CheckDefaultInitialization();
	}
}

void AGwPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void AGwPlayerState::OnDeactivated()
{
	Super::OnDeactivated();
}

void AGwPlayerState::OnReactivated()
{
	Super::OnReactivated();
}

void AGwPlayerState::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void AGwPlayerState::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (HasAuthority())
	{
		const FGenericTeamId OldTeamID = MyTeamID;

		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamID, this);
		MyTeamID = NewTeamID;
		ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
	}
	else
	{
		UE_LOG(LogGwTeams, Error, TEXT("Cannot set team for %s on non-authority"), *GetPathName(this));
	}
}

FGenericTeamId AGwPlayerState::GetGenericTeamId() const
{
	return MyTeamID;
}

FOnGwTeamIndexChangedDelegate* AGwPlayerState::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void AGwPlayerState::OnRep_PawnData()
{
}

void AGwPlayerState::OnExperienceLoaded(const UGwExperienceDefinition* CurrentExperience)
{
	if (AGwGameMode* GwGameMode = GetWorld()->GetAuthGameMode<AGwGameMode>())
	{
		if (const UGwPawnData* NewPawnData = GwGameMode->GetPawnDataForController(GetOwningController()))
		{
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogGw, Error, TEXT("AGwPlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
		}
	}
}

void AGwPlayerState::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void AGwPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyTeamID, SharedParams);

}

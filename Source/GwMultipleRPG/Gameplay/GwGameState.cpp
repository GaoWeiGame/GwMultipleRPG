// Fill out your copyright notice in the Description page of Project Settings.


#include "GwGameState.h"

#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameModes/GwExperienceManagerComponent.h"
#include "GwMultipleRPG/AbilitySystem/GwAbilitySystemComponent.h"
#include "GwMultipleRPG/Messages/GwVerbMessage.h"

AGwGameState::AGwGameState(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGwAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ExperienceManagerComponent = CreateDefaultSubobject<UGwExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}

UAbilitySystemComponent* AGwGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGwGameState::MulticastMessageToClients_Implementation(const FGwVerbMessage Message)
{
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

void AGwGameState::MulticastReliableMessageToClients_Implementation(const FGwVerbMessage Message)
{
	MulticastMessageToClients_Implementation(Message);
}

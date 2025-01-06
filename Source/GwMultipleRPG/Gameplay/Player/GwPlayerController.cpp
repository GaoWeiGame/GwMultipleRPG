// Fill out your copyright notice in the Description page of Project Settings.


#include "GwPlayerController.h"

#include "AbilitySystemGlobals.h"
#include "EnhancedInputSubsystems.h"
#include "GenericTeamAgentInterface.h"
#include "GwPlayerState.h"
#include "GwMultipleRPG/AbilitySystem/GwAbilitySystemComponent.h"
#include "GwMultipleRPG/Gameplay/Tags/GwGameplayTags.h"
#include "GwMultipleRPG/Input/GwInputComponent.h"


AGwPlayerController::AGwPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AGwPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AGwPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AGwPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AGwPlayerController::OnUnPossess()
{
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}
	Super::OnUnPossess();
}

void AGwPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AGwPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AGwPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BroadcastOnPlayerStateChanged();
}

void AGwPlayerController::SetIsAutoRunning(const bool bEnabled)
{
	const bool bIsAutoRunning = GetIsAutoRunning();
	if (bEnabled != bIsAutoRunning)
	{
		if (!bEnabled)
		{
			OnEndAutoRun();
		}
		else
		{
			OnStartAutoRun();
		}
	}
}

bool AGwPlayerController::GetIsAutoRunning() const
{
	bool bIsAutoRunning = false;
	if (const UGwAbilitySystemComponent* GwASC = GetGwAbilitySystemComponent())
	{
		bIsAutoRunning = GwASC->GetTagCount(GwGameplayTags::Status_AutoRunning) > 0;
	}
	return bIsAutoRunning;
}

UGwAbilitySystemComponent* AGwPlayerController::GetGwAbilitySystemComponent() const
{
	const AGwPlayerState* GwPS = GetGwPlayerState();
	return (GwPS ? GwPS->GetGwAbilitySystemComponent() : nullptr);
}

AGwPlayerState* AGwPlayerController::GetGwPlayerState() const
{
	return CastChecked<AGwPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

void AGwPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UGwAbilitySystemComponent* GwASC = GetGwAbilitySystemComponent())
	{
		GwASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AGwPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
}

FGenericTeamId AGwPlayerController::GetGenericTeamId() const
{
	if (const IGwTeamAgentInterface* PSWithTeamInterface = Cast<IGwTeamAgentInterface>(PlayerState))
	{
		return PSWithTeamInterface->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

FOnGwTeamIndexChangedDelegate* AGwPlayerController::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}


void AGwPlayerController::OnStartAutoRun()
{
	if (UGwAbilitySystemComponent* GwASC = GetGwAbilitySystemComponent())
	{
		GwASC->SetLooseGameplayTagCount(GwGameplayTags::Status_AutoRunning, 1);
		K2_OnStartAutoRun();
	}
}

void AGwPlayerController::OnEndAutoRun()
{
	if (UGwAbilitySystemComponent* GwASC = GetGwAbilitySystemComponent())
	{
		GwASC->SetLooseGameplayTagCount(GwGameplayTags::Status_AutoRunning, 0);
		K2_OnEndAutoRun();
	}
}

void AGwPlayerController::OnPlayerStateChanged()
{
}

void AGwPlayerController::BroadcastOnPlayerStateChanged()
{
	OnPlayerStateChanged();

	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (LastSeenPlayerState != nullptr)
	{
		if (IGwTeamAgentInterface* PlayerStateTeamInterface = Cast<IGwTeamAgentInterface>(LastSeenPlayerState))
		{
			OldTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().RemoveAll(this);
		}
	}

	// Bind to the new player state, if any
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (PlayerState != nullptr)
	{
		if (IGwTeamAgentInterface* PlayerStateTeamInterface = Cast<IGwTeamAgentInterface>(PlayerState))
		{
			NewTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnPlayerStateChangedTeam);
		}
	}

	// Broadcast the team change (if it really has)
	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
	
	LastSeenPlayerState = PlayerState;
}

void AGwPlayerController::OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));

}

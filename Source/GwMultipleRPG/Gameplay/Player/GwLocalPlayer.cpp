// Fill out your copyright notice in the Description page of Project Settings.


#include "GwLocalPlayer.h"

#include "GwMultipleRPG/Settings/GwSettingsLocal.h"
#include "GwMultipleRPG/Settings/GwSettingsShared.h"

UE_DISABLE_OPTIMIZATION

UGwLocalPlayer::UGwLocalPlayer()
{
}

UGwSettingsShared* UGwLocalPlayer::GetSharedSettings() const
{
	if (!SharedSettings)
	{
		bool bCanLoadBeforeLogin = PLATFORM_DESKTOP;

		if (bCanLoadBeforeLogin)
		{
			SharedSettings = UGwSettingsShared::LoadOrCreateSettings(this);
		}
		else
		{
			// We need to wait for user login to get the real settings so return temp ones
			SharedSettings = UGwSettingsShared::CreateTemporarySettings(this);
		}
	}

	return SharedSettings;
}

void UGwLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();
}

void UGwLocalPlayer::SwitchController(APlayerController* PC)
{
	Super::SwitchController(PC);

	OnPlayerControllerChanged(PlayerController);
}

bool UGwLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);

	OnPlayerControllerChanged(PlayerController);

	return bResult;
}

void UGwLocalPlayer::InitOnlineSession()
{
	OnPlayerControllerChanged(PlayerController);

	Super::InitOnlineSession();
}

void UGwLocalPlayer::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	IGwTeamAgentInterface::SetGenericTeamId(NewTeamID);
}

FGenericTeamId UGwLocalPlayer::GetGenericTeamId() const
{
	if (IGwTeamAgentInterface* ControllerAsTeamProvider = Cast<IGwTeamAgentInterface>(PlayerController))
	{
		return ControllerAsTeamProvider->GetGenericTeamId();
	}
	else
	{
		return FGenericTeamId::NoTeam;
	}
}

FOnGwTeamIndexChangedDelegate* UGwLocalPlayer::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

UGwSettingsLocal* UGwLocalPlayer::GetLocalSettings() const
{
	return UGwSettingsLocal::Get();
}

void UGwLocalPlayer::OnPlayerControllerChanged(APlayerController* NewController)
{
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (IGwTeamAgentInterface* ControllerAsTeamProvider = Cast<IGwTeamAgentInterface>(LastBoundPC.Get()))
	{
		OldTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().RemoveAll(this);
	}

	// Grab the current team ID and listen for future changes
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (IGwTeamAgentInterface* ControllerAsTeamProvider = Cast<IGwTeamAgentInterface>(NewController))
	{
		NewTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnControllerChangedTeam);
		LastBoundPC = NewController;
	}

	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
}

void UGwLocalPlayer::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
}

UE_ENABLE_OPTIMIZATION

// Copyright Epic Games, Inc. All Rights Reserved.

#include "GwSettingScreen.h"

#include "GwMultipleRPG/Gameplay/Player/GwLocalPlayer.h"
#include "GwMultipleRPG/Settings/GwGameSettingRegistry.h"
#include "Input/CommonUIInputTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GwSettingScreen)

class UGameSettingRegistry;

void UGwSettingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleApplyAction)));
	CancelChangesHandle = RegisterUIActionBinding(FBindUIActionArgs(CancelChangesInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleCancelChangesAction)));
}

UGameSettingRegistry* UGwSettingScreen::CreateRegistry()
{
	UGwGameSettingRegistry* NewRegistry = NewObject<UGwGameSettingRegistry>();

	if (UGwLocalPlayer* LocalPlayer = CastChecked<UGwLocalPlayer>(GetOwningLocalPlayer()))
	{
		NewRegistry->Initialize(LocalPlayer);
	}

	return NewRegistry;
}

void UGwSettingScreen::HandleBackAction()
{
	if (AttemptToPopNavigation())
	{
		return;
	}

	ApplyChanges();

	DeactivateWidget();
}

void UGwSettingScreen::HandleApplyAction()
{
	ApplyChanges();
}

void UGwSettingScreen::HandleCancelChangesAction()
{
	CancelChanges();
}

void UGwSettingScreen::OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
{
	if (bSettingsDirty)
	{
		if (!GetActionBindings().Contains(ApplyHandle))
		{
			AddActionBinding(ApplyHandle);
		}
		if (!GetActionBindings().Contains(CancelChangesHandle))
		{
			AddActionBinding(CancelChangesHandle);
		}
	}
	else
	{
		RemoveActionBinding(ApplyHandle);
		RemoveActionBinding(CancelChangesHandle);
	}
}

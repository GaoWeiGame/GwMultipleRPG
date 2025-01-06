// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GwSettingsShared.generated.h"


UENUM(BlueprintType)
enum class EGwGamepadSensitivity : uint8
{
	Invalid = 0 UMETA(Hidden),

	Slow UMETA(DisplayName = "01 - Slow"),
	SlowPlus UMETA(DisplayName = "02 - Slow+"),
	SlowPlusPlus UMETA(DisplayName = "03 - Slow++"),
	Normal UMETA(DisplayName = "04 - Normal"),
	NormalPlus UMETA(DisplayName = "05 - Normal+"),
	NormalPlusPlus UMETA(DisplayName = "06 - Normal++"),
	Fast UMETA(DisplayName = "07 - Fast"),
	FastPlus UMETA(DisplayName = "08 - Fast+"),
	FastPlusPlus UMETA(DisplayName = "09 - Fast++"),
	Insane UMETA(DisplayName = "10 - Insane"),

	MAX UMETA(Hidden),
};

class UGwLocalPlayer;

UCLASS()
class UGwSettingsShared : public ULocalPlayerSaveGame
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(UGwSettingsShared, FOnSettingChangedEvent, UGwSettingsShared* Settings);
	FOnSettingChangedEvent OnSettingChanged;
	
	UGwSettingsShared();

	static UGwSettingsShared* LoadOrCreateSettings(const UGwLocalPlayer* LocalPlayer);

	static UGwSettingsShared* CreateTemporarySettings(const UGwLocalPlayer* LocalPlayer);

	void ApplySettings();

	void SaveSettings();

	UFUNCTION()
	float GetGamepadMoveStickDeadZone() const { return GamepadMoveStickDeadZone; }

	UFUNCTION()
	float GetGamepadLookStickDeadZone() const { return GamepadLookStickDeadZone; }

	UFUNCTION()
	EGwGamepadSensitivity GetGamepadLookSensitivityPreset() const { return GamepadLookSensitivityPreset; }

	UFUNCTION()
	EGwGamepadSensitivity GetGamepadTargetingSensitivityPreset() const { return GamepadTargetingSensitivityPreset; }

	UFUNCTION()
	bool GetInvertVerticalAxis() const { return bInvertVerticalAxis; }

	UFUNCTION()
	void SetInvertVerticalAxis(bool NewValue)
	{
		ChangeValueAndDirty(bInvertVerticalAxis, NewValue);
		ApplyInputSensitivity();
	}

	UFUNCTION()
	bool GetInvertHorizontalAxis() const { return bInvertHorizontalAxis; }

	UFUNCTION()
	void SetInvertHorizontalAxis(bool NewValue)
	{
		ChangeValueAndDirty(bInvertHorizontalAxis, NewValue);
		ApplyInputSensitivity();
	}

	void ApplyInputSensitivity();

	void ClearPendingCulture();

	void ResetToDefaultCulture();

	void SetPendingCulture(const FString& NewCulture);

	bool ShouldResetToDefaultCulture() const { return bResetToDefaultCulture; }

	const FString& GetPendingCulture() const;

	bool IsUsingDefaultCulture() const;

	void ApplyCultureSettings();

private:
	UPROPERTY()
	float GamepadMoveStickDeadZone;

	UPROPERTY()
	float GamepadLookStickDeadZone;

	UPROPERTY()
	EGwGamepadSensitivity GamepadLookSensitivityPreset = EGwGamepadSensitivity::Normal;

	UPROPERTY()
	EGwGamepadSensitivity GamepadTargetingSensitivityPreset = EGwGamepadSensitivity::Normal;

	UPROPERTY()
	bool bInvertVerticalAxis = false;

	UPROPERTY()
	bool bInvertHorizontalAxis = false;

	template <typename T>
	bool ChangeValueAndDirty(T& CurrentValue, const T& NewValue)
	{
		if (CurrentValue != NewValue)
		{
			CurrentValue = NewValue;
			bIsDirty = true;
			OnSettingChanged.Broadcast(this);

			return true;
		}

		return false;
	}

	bool bIsDirty = false;

	UPROPERTY(Transient)
	FString PendingCulture;

	bool bResetToDefaultCulture = false;

};

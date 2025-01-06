// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "GwSettingsLocal.generated.h"

class USoundControlBus;
/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UGwSettingsLocal : public UGameUserSettings
{
	GENERATED_BODY()
public:
	static UGwSettingsLocal* Get();

public:
	UFUNCTION()
	bool IsSafeZoneSet() const { return SafeZoneScale != -1; }
	UFUNCTION()
	float GetSafeZone() const { return SafeZoneScale >= 0 ? SafeZoneScale : 0; }
	UFUNCTION()
	void SetSafeZone(float Value) { SafeZoneScale = Value; ApplySafeZoneScale(); }

	void ApplySafeZoneScale();
private:
	UPROPERTY(Config)
	float SafeZoneScale = -1;

};

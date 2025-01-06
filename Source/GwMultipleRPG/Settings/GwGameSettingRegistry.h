// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingRegistry.h"
#include "GwGameSettingRegistry.generated.h"

class UGwLocalPlayer;

USTRUCT(BlueprintType)
struct FGwQuickBarSlotsChangedMessageKeyboard
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=QuickBar)
	TObjectPtr<ULocalPlayer> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = QuickBar)
	FName QuickBarName;
	
	UPROPERTY(BlueprintReadOnly, Category = QuickBar)
	FName KeyboardName;
};

UCLASS()
class GWMULTIPLERPG_API UGwGameSettingRegistry : public UGameSettingRegistry
{
	GENERATED_BODY()

public:
	UGwGameSettingRegistry();

	static UGwGameSettingRegistry* Get(UGwLocalPlayer* InLocalPlayer);

	virtual void SaveChanges() override;

	virtual void OnSettingApplied(UGameSetting* Setting) override;

protected:
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) override;
	virtual bool IsFinishedInitializing() const override;

	UGameSettingCollection* InitializeGameplaySettings(UGwLocalPlayer* InLocalPlayer);
	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GameplaySettings;
	
	UGameSettingCollection* InitializeMouseAndKeyboardSettings(UGwLocalPlayer* InLocalPlayer);
	UPROPERTY()
	TObjectPtr<UGameSettingCollection> MouseAndKeyboardSettings;

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "GwUIManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GWMULTIPLERPG_API UGwUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()
public:
	UGwUIManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();
	
	FTSTicker::FDelegateHandle TickHandle;
};

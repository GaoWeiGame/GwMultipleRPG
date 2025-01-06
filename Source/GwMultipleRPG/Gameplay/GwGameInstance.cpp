// Fill out your copyright notice in the Description page of Project Settings.


#include "GwGameInstance.h"

#include "CommonSessionSubsystem.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Tags/GwGameplayTags.h"

UGwGameInstance::UGwGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGwGameInstance::Init()
{
	Super::Init();

	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	if (ensure(ComponentManager))
	{
		ComponentManager->RegisterInitState(GwGameplayTags::InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GwGameplayTags::InitState_DataAvailable, false, GwGameplayTags::InitState_Spawned);
		ComponentManager->RegisterInitState(GwGameplayTags::InitState_DataInitialized, false, GwGameplayTags::InitState_DataAvailable);
		ComponentManager->RegisterInitState(GwGameplayTags::InitState_GameplayReady, false, GwGameplayTags::InitState_DataInitialized);
	}
	if (UCommonSessionSubsystem* SessionSubsystem = GetSubsystem<UCommonSessionSubsystem>())
	{
		SessionSubsystem->OnPreClientTravelEvent.AddUObject(this, &UGwGameInstance::OnPreClientTravelToSession);
	}
}

void UGwGameInstance::Shutdown()
{
	if (UCommonSessionSubsystem* SessionSubsystem = GetSubsystem<UCommonSessionSubsystem>())
	{
		SessionSubsystem->OnPreClientTravelEvent.RemoveAll(this);
	}
	Super::Shutdown();
}

void UGwGameInstance::OnPreClientTravelToSession(FString& URL)
{
}

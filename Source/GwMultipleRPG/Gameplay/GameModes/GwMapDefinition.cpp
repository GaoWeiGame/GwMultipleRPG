// Fill out your copyright notice in the Description page of Project Settings.


#include "GwMapDefinition.h"

#include "CommonSessionSubsystem.h"

UCommonSession_HostSessionRequest* UGwMapDefinition::CreateHostingRequest(const UObject* WorldContextObject) const
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
	UCommonSession_HostSessionRequest* Result = nullptr;
	
	if (UCommonSessionSubsystem* Subsystem = GameInstance ? GameInstance->GetSubsystem<UCommonSessionSubsystem>() : nullptr)
	{
		Result = Subsystem->CreateOnlineHostSessionRequest();
	}

	if (!Result)
	{
		Result = NewObject<UCommonSession_HostSessionRequest>();
		Result->OnlineMode = ECommonSessionOnlineMode::Online;
		Result->bUseLobbies = true;
	}
	Result->MapID = MapID;
	Result->MaxPlayerCount = 4;
	return Result;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GwTeamInfoBase.h"

#include "GwTeamSubsystem.h"
#include "Net/UnrealNetwork.h"


AGwTeamInfoBase::AGwTeamInfoBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	  , TeamId(INDEX_NONE)
{
	bReplicates = true;
	bAlwaysRelevant = true;
	NetPriority = 3.0f;
	SetReplicatingMovement(false);
}

void AGwTeamInfoBase::RegisterWithTeamSubsystem(UGwTeamSubsystem* Subsystem)
{
	Subsystem->RegisterTeamInfo(this);
}

void AGwTeamInfoBase::TryRegisterWithTeamSubsystem()
{
	if (TeamId != INDEX_NONE)
	{
		UGwTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UGwTeamSubsystem>();
		if (ensure(TeamSubsystem))
		{
			RegisterWithTeamSubsystem(TeamSubsystem);
		}
	}
}

void AGwTeamInfoBase::SetTeamId(int32 NewTeamId)
{
	check(HasAuthority());
	check(TeamId == INDEX_NONE);
	check(NewTeamId != INDEX_NONE);

	TeamId = NewTeamId;

	TryRegisterWithTeamSubsystem();
}

void AGwTeamInfoBase::OnRep_TeamId()
{
	TryRegisterWithTeamSubsystem();
}


void AGwTeamInfoBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TeamTags);
	DOREPLIFETIME_CONDITION(ThisClass, TeamId, COND_InitialOnly);
}

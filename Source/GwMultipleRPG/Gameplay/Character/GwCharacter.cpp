// Fill out your copyright notice in the Description page of Project Settings.


#include "GwCharacter.h"

#include "GenericTeamAgentInterface.h"
#include "GwCharacterMovementComponent.h"
#include "GwHealthComponent.h"
#include "GwHeroComponent.h"
#include "GwPawnExtensionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/GameStateComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GwMultipleRPG/GwMultipleRPG.h"
#include "GwMultipleRPG/AbilitySystem/GwAbilitySystemComponent.h"
#include "GwMultipleRPG/Equipment/GwEquipmentManagerComponent.h"
#include "GwMultipleRPG/Gameplay/Player/GwPlayerState.h"
#include "GwMultipleRPG/Gameplay/Tags/GwGameplayTags.h"
#include "GwMultipleRPG/MinMap/GwMinMapComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AGwCharacter::AGwCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGwCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))

{
	PrimaryActorTick.bCanEverTick = false;

	USkeletalMeshComponent* MeshComponent = GetMesh();
	check(MeshComponent);
	MeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 350.f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 250.f);
	SpringArm->ProbeSize = 24;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 5.f;

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(SpringArm);
	ThirdPersonCamera->SetRelativeRotation(FRotator(-19.f, 0.f, 0.f));

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->SetWalkableFloorAngle(50.f);
	CharacterMovementComponent->SetWalkableFloorZ(0.64f);
	CharacterMovementComponent->JumpZVelocity = 600.f;
	CharacterMovementComponent->RotationRate = FRotator(0.f, 0.f, 640.f);
	CharacterMovementComponent->bUseControllerDesiredRotation = true;
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->bConstrainToPlane = true;
	CharacterMovementComponent->bSnapToPlaneAtStart = true;

	PawnExtComponent = CreateDefaultSubobject<UGwPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));

	EquipmentManagerComponent = CreateDefaultSubobject<UGwEquipmentManagerComponent>(TEXT("EquipmentManagerComponent"));

	HealthComponent = CreateDefaultSubobject<UGwHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);

	HeroComponent = CreateDefaultSubobject<UGwHeroComponent>(TEXT("HeroComponent"));
}

void AGwCharacter::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent* AGwCharacter::GetAbilitySystemComponent() const
{
	if (PawnExtComponent == nullptr)
	{
		return nullptr;
	}

	return PawnExtComponent->GetGwAbilitySystemComponent();
}

UGwAbilitySystemComponent* AGwCharacter::GetGwAbilitySystemComponent() const
{
	return Cast<UGwAbilitySystemComponent>(GetAbilitySystemComponent());
}

void AGwCharacter::InitAbilityActorInfo()
{
	AGwPlayerState* GwPlayerState = GetPlayerState<AGwPlayerState>();
	check(GwPlayerState);
	GwPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(GwPlayerState, this);
	AbilitySystemComponent = GwPlayerState->GetGwAbilitySystemComponent();


	for (const UGwAbilitySet* AbilitySet : HeroComponent->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}

void AGwCharacter::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void AGwCharacter::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void AGwCharacter::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UGwCharacterMovementComponent* GwMoveComp = CastChecked<UGwCharacterMovementComponent>(GetCharacterMovement());
	GwMoveComp->StopMovementImmediately();
	GwMoveComp->DisableMovement();
}

void AGwCharacter::DestroyDueToDeath()
{
	K2_OnDeathFinished();


	{
		//从小地图上删除玩家
		UGwMinMapComponent* MinMapComponent = UGameplayStatics::GetGameState(GetWorld())->GetComponentByClass<UGwMinMapComponent>();
		if (MinMapComponent)
		{
			MinMapComponent->OnGwRemoveMinMapPlayer.Broadcast(this);
		}
	}

	UninitAndDestroy();
}

void AGwCharacter::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	if (UGwAbilitySystemComponent* GwASC = GetGwAbilitySystemComponent())
	{
		if (GwASC->GetAvatarActor() == this)
		{
			PawnExtComponent->UninitializeAbilitySystem();
		}
	}

	SetActorHiddenInGame(true);
}

void AGwCharacter::OnRep_MyTeamID(FGenericTeamId OldTeamID)
{
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void AGwCharacter::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	const FGenericTeamId MyOldTeamID = MyTeamID;
	MyTeamID = IntegerToGenericTeamId(NewTeam);
	ConditionalBroadcastTeamChanged(this, MyOldTeamID, MyTeamID);
}

void AGwCharacter::ToggleCrouch()
{
	const UGwCharacterMovementComponent* GwMoveComp = CastChecked<UGwCharacterMovementComponent>(GetCharacterMovement());

	if (bIsCrouched || GwMoveComp->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (GwMoveComp->IsMovingOnGround())
	{
		Crouch();
	}
}

void AGwCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (GetController() == nullptr)
	{
		if (HasAuthority())
		{
			const FGenericTeamId OldTeamID = MyTeamID;
			MyTeamID = NewTeamID;
			ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
		}
		else
		{
			UE_LOG(LogGwTeams, Error, TEXT("You can't set the team ID on a character (%s) except on the authority"), *GetPathNameSafe(this));
		}
	}
	else
	{
		UE_LOG(LogGwTeams, Error, TEXT("You can't set the team ID on a possessed character (%s); it's driven by the associated controller"), *GetPathNameSafe(this));
	}
}

FGenericTeamId AGwCharacter::GetGenericTeamId() const
{
	return MyTeamID;
}

FOnGwTeamIndexChangedDelegate* AGwCharacter::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void AGwCharacter::NotifyControllerChanged()
{
	const FGenericTeamId OldTeamId = GetGenericTeamId();

	Super::NotifyControllerChanged();

	// Update our team ID based on the controller
	if (HasAuthority() && (Controller != nullptr))
	{
		if (IGwTeamAgentInterface* ControllerWithTeam = Cast<IGwTeamAgentInterface>(Controller))
		{
			MyTeamID = ControllerWithTeam->GetGenericTeamId();
			ConditionalBroadcastTeamChanged(this, OldTeamId, MyTeamID);
		}
	}
}

void AGwCharacter::PossessedBy(AController* NewController)
{
	const FGenericTeamId OldTeamID = MyTeamID;

	Super::PossessedBy(NewController);

	PawnExtComponent->HandleControllerChanged();

	InitAbilityActorInfo();

	if (IGwTeamAgentInterface* ControllerAsTeamProvider = Cast<IGwTeamAgentInterface>(NewController))
	{
		MyTeamID = ControllerAsTeamProvider->GetGenericTeamId();
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnControllerChangedTeam);
	}
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void AGwCharacter::UnPossessed()
{
	AController* const OldController = Controller;

	const FGenericTeamId OldTeamID = MyTeamID;
	if (IGwTeamAgentInterface* ControllerAsTeamProvider = Cast<IGwTeamAgentInterface>(OldController))
	{
		ControllerAsTeamProvider->GetTeamChangedDelegateChecked().RemoveAll(this);
	}

	Super::UnPossessed();
	PawnExtComponent->HandleControllerChanged();

	MyTeamID = DetermineNewTeamAfterPossessionEnds(OldTeamID);
	ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void AGwCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	PawnExtComponent->HandleControllerChanged();
}

void AGwCharacter::InitializeGameplayTags()
{
	if (UGwAbilitySystemComponent* GwASC = GetGwAbilitySystemComponent())
	{
		for (const TPair<uint8, FGameplayTag>& TagMapping : GwGameplayTags::MovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				GwASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		for (const TPair<uint8, FGameplayTag>& TagMapping : GwGameplayTags::CustomMovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				GwASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		UGwCharacterMovementComponent* GwMoveComp = CastChecked<UGwCharacterMovementComponent>(GetCharacterMovement());
		SetMovementModeTag(GwMoveComp->MovementMode, GwMoveComp->CustomMovementMode, true);
	}
}

void AGwCharacter::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled)
{
	if (UGwAbilitySystemComponent* GwASC = GetGwAbilitySystemComponent())
	{
		const FGameplayTag* MovementModeTag = nullptr;
		if (MovementMode == MOVE_Custom)
		{
			MovementModeTag = GwGameplayTags::CustomMovementModeTagMap.Find(CustomMovementMode);
		}
		else
		{
			MovementModeTag = GwGameplayTags::MovementModeTagMap.Find(MovementMode);
		}

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			GwASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnabled ? 1 : 0));
		}
	}
}

void AGwCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	PawnExtComponent->HandlePlayerStateReplicated();
}

void AGwCharacter::OnAbilitySystemInitialized()
{
	UGwAbilitySystemComponent* GwASC = GetGwAbilitySystemComponent();
	check(GwASC);

	HealthComponent->InitializeWithAbilitySystem(GwASC);

	InitializeGameplayTags();
}

void AGwCharacter::OnAbilitySystemUninitialized()
{
	HealthComponent->UninitializeFromAbilitySystem();
}

void AGwCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PawnExtComponent->SetupPlayerInputComponent();
}


void AGwCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, MyTeamID)
}

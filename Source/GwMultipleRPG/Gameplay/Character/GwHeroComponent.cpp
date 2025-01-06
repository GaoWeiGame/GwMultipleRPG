// Fill out your copyright notice in the Description page of Project Settings.


#include "GwHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "GwCharacter.h"
#include "GwPawnExtensionComponent.h"
#include "GwMultipleRPG/AbilitySystem/GwAbilitySystemComponent.h"
#include "GwMultipleRPG/Gameplay/Player/GwPlayerController.h"
#include "GwMultipleRPG/Gameplay/Tags/GwGameplayTags.h"
#include "GwMultipleRPG/Input/GwInputComponent.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "GwMultipleRPG/GwMultipleRPG.h"
#include "GwMultipleRPG/GameFeatures/GameFeatureAction_AddInputContextMapping.h"
#include "GwMultipleRPG/Gameplay/Player/GwLocalPlayer.h"
#include "GwMultipleRPG/Gameplay/Player/GwPlayerState.h"
#include "GwMultipleRPG/Messages/GwVerbMessage.h"
#include "Misc/UObjectToken.h"


namespace GwHero
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};

const FName UGwHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UGwHeroComponent::NAME_ActorFeatureName("Hero");

UE_DEFINE_GAMEPLAY_TAG(TAG_Input_Mouse_Move, "Input.Mouse.Move");


UE_DISABLE_OPTIMIZATION

UGwHeroComponent::UGwHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReadyToBindInputs = false;
}

void UGwHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const UGwLocalPlayer* LP = Cast<UGwLocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const UGwPawnExtensionComponent* PawnExtComp = UGwPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UGwPawnData* PawnData = PawnExtComp->GetPawnData<UGwPawnData>())
		{
			if (const UGwInputConfig* InputConfig = PawnData->InputConfig)
			{
				for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
				{
					if (UInputMappingContext* IMC = Mapping.InputMapping.Get())
					{
						if (Mapping.bRegisterWithSettings)
						{
							if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
							{
								Settings->RegisterInputMappingContext(IMC);
							}

							FModifyContextOptions Options = {};
							Options.bIgnoreAllPressedKeysUntilRelease = false;
							// Actually add the config to the local player							
							Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
						}
					}
				}

				// The Gw Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the UGwInputComponent or modify this component accordingly.
				UGwInputComponent* GwIC = Cast<UGwInputComponent>(PlayerInputComponent);
				if (ensureMsgf(GwIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UGwInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
					GwIC->AddInputMappings(InputConfig, Subsystem);

					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					TArray<uint32> BindHandles;
					GwIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

					GwIC->BindNativeAction(InputConfig, GwGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
					GwIC->BindNativeAction(InputConfig, GwGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
					GwIC->BindNativeAction(InputConfig, GwGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
					GwIC->BindNativeAction(InputConfig, GwGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
					GwIC->BindNativeAction(InputConfig, GwGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
				}
			}
		}
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

bool UGwHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == GwGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == GwGameplayTags::InitState_Spawned && DesiredState == GwGameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<AGwPlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) &&
				(Controller->PlayerState != nullptr) &&
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			AGwPlayerController* GwPC = GetController<AGwPlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !GwPC || !GwPC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == GwGameplayTags::InitState_DataAvailable && DesiredState == GwGameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		AGwPlayerState* GwPS = GetPlayerState<AGwPlayerState>();

		return GwPS && Manager->HasFeatureReachedInitState(Pawn, UGwPawnExtensionComponent::NAME_ActorFeatureName, GwGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == GwGameplayTags::InitState_DataInitialized && DesiredState == GwGameplayTags::InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void UGwHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == GwGameplayTags::InitState_DataAvailable && DesiredState == GwGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		AGwPlayerState* GwPS = GetPlayerState<AGwPlayerState>();
		if (!ensure(Pawn && GwPS))
		{
			return;
		}

		const UGwPawnData* PawnData = nullptr;

		if (UGwPawnExtensionComponent* PawnExtComp = UGwPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UGwPawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			PawnExtComp->InitializeAbilitySystem(GwPS->GetGwAbilitySystemComponent(), GwPS);
		}

		if (AGwPlayerController* GwPC = GetController<AGwPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

void UGwHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UGwPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == GwGameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void UGwHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = {GwGameplayTags::InitState_Spawned, GwGameplayTags::InitState_DataAvailable, GwGameplayTags::InitState_DataInitialized, GwGameplayTags::InitState_GameplayReady};

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

bool UGwHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UGwHeroComponent::AddAdditionalInputConfig(const UGwInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const UGwPawnExtensionComponent* PawnExtComp = UGwPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		UGwInputComponent* GwIC = Pawn->FindComponentByClass<UGwInputComponent>();
		if (ensureMsgf(GwIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UGwInputComponent or a subclass of it.")))
		{
			GwIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
		}
	}
}

void UGwHeroComponent::RemoveAdditionalInputConfig(const UGwInputConfig* InputConfig)
{
	return;
}

void UGwHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogGw, Error, TEXT("[UGwHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("GwHeroComponent", "NotOnPawnError",
			                                       "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("GwHeroComponent");

			FMessageLog(HeroMessageLogName).Error()
			                               ->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
			                               ->AddToken(FTextToken::Create(Message));

			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

void UGwHeroComponent::BeginPlay()
{
	Super::BeginPlay();
	BindOnActorInitStateChanged(UGwPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(GwGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UGwHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UGwHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		const AGwCharacter* Character = Cast<AGwCharacter>(Pawn);
		UGwAbilitySystemComponent* GwAbilitySystemComponent = Character->GetGwAbilitySystemComponent();
		GwAbilitySystemComponent->AbilityInputTagPressed(InputTag);
	}
}

void UGwHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		const AGwCharacter* Character = Cast<AGwCharacter>(Pawn);
		UGwAbilitySystemComponent* GwAbilitySystemComponent = Character->GetGwAbilitySystemComponent();
		GwAbilitySystemComponent->AbilityInputTagReleased(InputTag);
	}
}

void UGwHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	// If the player has attempted to move again then cancel auto running
	if (AGwPlayerController* GwController = Cast<AGwPlayerController>(Controller))
	{
		GwController->SetIsAutoRunning(false);
	}

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}

		OnInputMove.Broadcast();
		// FGwVerbMessage Message;
		// Message.Instigator = GetController<AController>();
		// UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
		// MessageSystem.BroadcastMessage(TAG_Input_Mouse_Move, Message);
	}
}

void UGwHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(-Value.Y);
	}
}

void UGwHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X * GwHero::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * GwHero::LookPitchRate * World->GetDeltaSeconds());
	}
}

void UGwHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (AGwCharacter* Character = GetPawn<AGwCharacter>())
	{
		Character->ToggleCrouch();
	}
}

void UGwHeroComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		if (AGwPlayerController* Controller = Cast<AGwPlayerController>(Pawn->GetController()))
		{
			// Toggle auto running
			Controller->SetIsAutoRunning(!Controller->GetIsAutoRunning());
		}
	}
}

UE_ENABLE_OPTIMIZATION

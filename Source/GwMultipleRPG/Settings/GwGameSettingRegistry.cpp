// Fill out your copyright notice in the Description page of Project Settings.


#include "GwGameSettingRegistry.h"
#include "Engine/PlatformSettingsManager.h"
#include "CommonInputBaseTypes.h"
#include "EnhancedInputSubsystems.h"
#include "GameSettingCollection.h"
#include "GwSettingsLocal.h"
#include "GwSettingsShared.h"
#include "CustomSettings/GwSettingKeyboardInput.h"
#include "EditCondition/WhenCondition.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "GwMultipleRPG/Gameplay/Player/GwLocalPlayer.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "NativeGameplayTags.h"

#define LOCTEXT_NAMESPACE "Gw"
#include "CustomSettings/GwSettingValueDiscrete_Language.h"
#include "EditCondition/WhenPlayingAsPrimaryPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GwGameSettingRegistry)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Gw_QuickBar_Message_SlotsChangedKeyboard, "Gw.QuickBar.Message.SlotsChangedKeyboard");

UGwGameSettingRegistry::UGwGameSettingRegistry()
{
}

UGwGameSettingRegistry* UGwGameSettingRegistry::Get(UGwLocalPlayer* InLocalPlayer)
{
	UGwGameSettingRegistry* Registry = FindObject<UGwGameSettingRegistry>(InLocalPlayer, TEXT("GwGameSettingRegistry"), true);
	if (Registry == nullptr)
	{
		Registry = NewObject<UGwGameSettingRegistry>(InLocalPlayer, TEXT("GwGameSettingRegistry"));
		Registry->Initialize(InLocalPlayer);
	}

	return Registry;
}

void UGwGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();

	if (UGwLocalPlayer* LocalPlayer = Cast<UGwLocalPlayer>(OwningLocalPlayer))
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		LocalPlayer->GetLocalSettings()->ApplySettings(false);

		LocalPlayer->GetSharedSettings()->ApplySettings();
		LocalPlayer->GetSharedSettings()->SaveSettings();
	}
}

void UGwGameSettingRegistry::OnSettingApplied(UGameSetting* Setting)
{
	Super::OnSettingApplied(Setting);

	if (UGwSettingKeyboardInput* GwSettingKeyboardInput = Cast<UGwSettingKeyboardInput>(Setting))
	{
		FGwQuickBarSlotsChangedMessageKeyboard Message;
		Message.Owner = OwningLocalPlayer;
		Message.QuickBarName = FName(*Setting->GetDisplayName().ToString());
		Message.KeyboardName = FName(*GwSettingKeyboardInput->GetKeyTextFromSlot(EPlayerMappableKeySlot::First).ToString());
		UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwningLocalPlayer);
		MessageSystem.BroadcastMessage(TAG_Gw_QuickBar_Message_SlotsChangedKeyboard, Message);
	}
	return;
}


void UGwGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	UGwLocalPlayer* GwLocalPlayer = Cast<UGwLocalPlayer>(InLocalPlayer);

	GameplaySettings = InitializeGameplaySettings(GwLocalPlayer);
	RegisterSetting(GameplaySettings);
	
	MouseAndKeyboardSettings = InitializeMouseAndKeyboardSettings(GwLocalPlayer);
	RegisterSetting(MouseAndKeyboardSettings);
}

bool UGwGameSettingRegistry::IsFinishedInitializing() const
{
	if (Super::IsFinishedInitializing())
	{
		if (UGwLocalPlayer* LocalPlayer = Cast<UGwLocalPlayer>(OwningLocalPlayer))
		{
			if (LocalPlayer->GetSharedSettings() == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

UGameSettingCollection* UGwGameSettingRegistry::InitializeGameplaySettings(UGwLocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* Screen = NewObject<UGameSettingCollection>();
	Screen->SetDevName(TEXT("GameplayCollection"));
	Screen->SetDisplayName(LOCTEXT("GameplayCollection_Name", "Gameplay"));
	Screen->Initialize(InLocalPlayer);

	{
		UGameSettingCollection* LanguageSubsection = NewObject<UGameSettingCollection>();
		LanguageSubsection->SetDevName(TEXT("LanguageCollection"));
		LanguageSubsection->SetDisplayName(LOCTEXT("LanguageCollection_Name", "Language"));
		Screen->AddSetting(LanguageSubsection);

		//----------------------------------------------------------------------------------
		{
			UGwSettingValueDiscrete_Language* Setting = NewObject<UGwSettingValueDiscrete_Language>();
			Setting->SetDevName(TEXT("Language"));
			Setting->SetDisplayName(LOCTEXT("LanguageSetting_Name", "Language"));
			Setting->SetDescriptionRichText(LOCTEXT("LanguageSetting_Description", "The language of the game."));
			
#if WITH_EDITOR
			if (GIsEditor)
			{
				Setting->SetDescriptionRichText(LOCTEXT("LanguageSetting_WithEditor_Description", "The language of the game.\n\n<text color=\"#ffff00\">WARNING: Language changes will not affect PIE, you'll need to run with -game to test this, or change your PIE language options in the editor preferences.</>"));
			}
#endif
			
			Setting->AddEditCondition(FWhenPlayingAsPrimaryPlayer::Get());

			LanguageSubsection->AddSetting(Setting);
		}
		//----------------------------------------------------------------------------------
	}

	return Screen;
}

UGameSettingCollection* UGwGameSettingRegistry::InitializeMouseAndKeyboardSettings(UGwLocalPlayer* InLocalPlayer)
{
	UGameSettingCollection* Screen = NewObject<UGameSettingCollection>();
	Screen->SetDevName(TEXT("MouseAndKeyboardCollection"));
	Screen->SetDisplayName(LOCTEXT("MouseAndKeyboardCollection_Name", "Mouse & Keyboard"));
	Screen->Initialize(InLocalPlayer);

	const TSharedRef<FWhenCondition> WhenPlatformSupportsMouseAndKeyboard = MakeShared<FWhenCondition>(
		[](const ULocalPlayer*, FGameSettingEditableState& InOutEditState)
		{
			const UCommonInputPlatformSettings* PlatformInput = UPlatformSettingsManager::Get().GetSettingsForPlatform<UCommonInputPlatformSettings>();
			if (!PlatformInput->SupportsInputType(ECommonInputType::MouseAndKeyboard))
			{
				InOutEditState.Kill(TEXT("Platform does not support mouse and keyboard"));
			}
		});

	// Bindings for Mouse & Keyboard - Automatically Generated
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingCollection* KeyBinding = NewObject<UGameSettingCollection>();
		KeyBinding->SetDevName(TEXT("KeyBindingCollection"));
		KeyBinding->SetDisplayName(LOCTEXT("KeyBindingCollection_Name", "Keyboard & Mouse"));
		Screen->AddSetting(KeyBinding);

		const UEnhancedInputLocalPlayerSubsystem* EISubsystem = InLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		const UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();

		// If you want to just get one profile pair, then you can do UserSettings->GetCurrentProfile

		// A map of key bindings mapped to their display category
		TMap<FString, UGameSettingCollection*> CategoryToSettingCollection;

		// Returns an existing setting collection for the display category if there is one.
		// If there isn't one, then it will create a new one and initialize it
		auto GetOrCreateSettingCollection = [&CategoryToSettingCollection, &Screen](FText DisplayCategory) -> UGameSettingCollection* {
			static const FString DefaultDevName = TEXT("Default_KBM");
			static const FText DefaultDevDisplayName = NSLOCTEXT("GwInputSettings", "GwInputDefaults", "Default Experiences");

			if (DisplayCategory.IsEmpty())
			{
				DisplayCategory = DefaultDevDisplayName;
			}

			FString DisplayCatString = DisplayCategory.ToString();

			if (UGameSettingCollection** ExistingCategory = CategoryToSettingCollection.Find(DisplayCatString))
			{
				return *ExistingCategory;
			}

			UGameSettingCollection* ConfigSettingCollection = NewObject<UGameSettingCollection>();
			ConfigSettingCollection->SetDevName(FName(DisplayCatString));
			ConfigSettingCollection->SetDisplayName(DisplayCategory);
			Screen->AddSetting(ConfigSettingCollection);
			CategoryToSettingCollection.Add(DisplayCatString, ConfigSettingCollection);

			return ConfigSettingCollection;
		};

		static TSet<FName> CreatedMappingNames;
		CreatedMappingNames.Reset();

		for (const TPair<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : UserSettings->GetAllSavedKeyProfiles())
		{
			const FGameplayTag& ProfileName = ProfilePair.Key;
			const TObjectPtr<UEnhancedPlayerMappableKeyProfile>& Profile = ProfilePair.Value;

			for (const TPair<FName, FKeyMappingRow>& RowPair : Profile->GetPlayerMappingRows())
			{
				// Create a setting row for anything with valid mappings and that we haven't created yet
				if (RowPair.Value.HasAnyMappings() /* && !CreatedMappingNames.Contains(RowPair.Key)*/)
				{
					// We only want keyboard keys on this settings screen, so we will filter down by mappings
					// that are set to keyboard keys
					FPlayerMappableKeyQueryOptions Options = {};
					Options.KeyToMatch = EKeys::W;
					Options.bMatchBasicKeyTypes = true;

					const FText& DesiredDisplayCategory = RowPair.Value.Mappings.begin()->GetDisplayCategory();

					if (UGameSettingCollection* Collection = GetOrCreateSettingCollection(DesiredDisplayCategory))
					{
						// Create the settings widget and initialize it, adding it to this config's section
						UGwSettingKeyboardInput* InputBinding = NewObject<UGwSettingKeyboardInput>();

						InputBinding->InitializeInputData(Profile, RowPair.Value, Options);
						InputBinding->AddEditCondition(WhenPlatformSupportsMouseAndKeyboard);

						Collection->AddSetting(InputBinding);
						CreatedMappingNames.Add(RowPair.Key);
					}
					else
					{
						ensure(false);
					}
				}
			}
		}
	}

	return Screen;
}

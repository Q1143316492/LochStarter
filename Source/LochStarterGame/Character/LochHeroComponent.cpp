// Copyright Epic Games, Inc. All Rights Reserved.

#include "LochHeroComponent.h"
#include "Components/GameFrameworkComponentDelegates.h"
#include "Logging/MessageLog.h"
#include "LochLogChannels.h"
#include "EnhancedInputSubsystems.h"
#include "Player/LochPlayerController.h"
#include "Player/LochPlayerState.h"
#include "Player/LochLocalPlayer.h"
#include "Character/LochPawnExtensionComponent.h"
#include "Character/LochPawnData.h"
#include "Character/LochCharacter.h"
#include "AbilitySystem/LochAbilitySystemComponent.h"
#include "Input/LochInputConfig.h"
#include "Input/LochInputComponent.h"
#include "Camera/LochCameraComponent.h"
#include "LochGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include "PlayerMappableInputConfig.h"
#include "Camera/LochCameraMode.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochHeroComponent)

#if WITH_EDITOR
#include "Misc/UObjectToken.h"
#endif	// WITH_EDITOR

namespace LochHero
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};

const FName ULochHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName ULochHeroComponent::NAME_ActorFeatureName("Hero");

ULochHeroComponent::ULochHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityCameraMode = nullptr;
	bReadyToBindInputs = false;
}

void ULochHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogLoch, Error, TEXT("[ULochHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("LochHeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("LochHeroComponent");
			
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

bool ULochHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == LochGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == LochGameplayTags::InitState_Spawned && DesiredState == LochGameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<ALochPlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
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
			ALochPlayerController* LochPC = GetController<ALochPlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !LochPC || !LochPC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == LochGameplayTags::InitState_DataAvailable && DesiredState == LochGameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		ALochPlayerState* LochPS = GetPlayerState<ALochPlayerState>();

		return LochPS && Manager->HasFeatureReachedInitState(Pawn, ULochPawnExtensionComponent::NAME_ActorFeatureName, LochGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == LochGameplayTags::InitState_DataInitialized && DesiredState == LochGameplayTags::InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void ULochHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == LochGameplayTags::InitState_DataAvailable && DesiredState == LochGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ALochPlayerState* LochPS = GetPlayerState<ALochPlayerState>();
		if (!ensure(Pawn && LochPS))
		{
			return;
		}

		const ULochPawnData* PawnData = nullptr;

		if (ULochPawnExtensionComponent* PawnExtComp = ULochPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<ULochPawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			PawnExtComp->InitializeAbilitySystem(LochPS->GetLochAbilitySystemComponent(), LochPS);
		}

		if (ALochPlayerController* LochPC = GetController<ALochPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		// Hook up the delegate for all pawns, in case we spectate later
		if (PawnData)
		{
			if (ULochCameraComponent* CameraComponent = ULochCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void ULochHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == ULochPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == LochGameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void ULochHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = { LochGameplayTags::InitState_Spawned, LochGameplayTags::InitState_DataAvailable, LochGameplayTags::InitState_DataInitialized, LochGameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void ULochHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(ULochPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(LochGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void ULochHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void ULochHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	// check(LP); // Might be null for bots or net clients not local

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP ? LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() : nullptr;
	if (Subsystem)
    {
	    Subsystem->ClearAllMappings();
    }

	if (const ULochPawnExtensionComponent* PawnExtComp = ULochPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULochPawnData* PawnData = PawnExtComp->GetPawnData<ULochPawnData>())
		{
			if (const ULochInputConfig* InputConfig = PawnData->InputConfig)
			{
                if (Subsystem)
                {
				    for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
				    {
					    if (UInputMappingContext* IMC = Mapping.InputMapping.LoadSynchronous())
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
                }

				// The Loch Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the ULochInputComponent or modify this component accordingly.
				ULochInputComponent* LochIC = Cast<ULochInputComponent>(PlayerInputComponent);
				if (ensureMsgf(LochIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to ULochInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
                    if (Subsystem)
					{
                        LochIC->AddInputMappings(InputConfig, Subsystem);
                    }

					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					TArray<uint32> BindHandles;
					LochIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

					LochIC->BindNativeAction(InputConfig, LochGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
					LochIC->BindNativeAction(InputConfig, LochGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
					LochIC->BindNativeAction(InputConfig, LochGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
					LochIC->BindNativeAction(InputConfig, LochGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
					LochIC->BindNativeAction(InputConfig, LochGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
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

void ULochHeroComponent::AddAdditionalInputConfig(const ULochInputConfig* InputConfig)
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
	// check(LP); // Can be null

	// UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(); // LP can be null

	if (const ULochPawnExtensionComponent* PawnExtComp = ULochPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		ULochInputComponent* LochIC = Pawn->FindComponentByClass<ULochInputComponent>();
		if (ensureMsgf(LochIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to ULochInputComponent or a subclass of it.")))
		{
			LochIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
		}
	}
}

void ULochHeroComponent::RemoveAdditionalInputConfig(const ULochInputConfig* InputConfig)
{
	//@TODO: Implement me!
}

bool ULochHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void ULochHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const ULochPawnExtensionComponent* PawnExtComp = ULochPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (ULochAbilitySystemComponent* LochASC = PawnExtComp->GetLochAbilitySystemComponent())
			{
				LochASC->AbilityInputTagPressed(InputTag);
			}
		}	
	}
}

void ULochHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const ULochPawnExtensionComponent* PawnExtComp = ULochPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (ULochAbilitySystemComponent* LochASC = PawnExtComp->GetLochAbilitySystemComponent())
		{
			LochASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void ULochHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	// If the player has attempted to move again then cancel auto running
	if (ALochPlayerController* LochController = Cast<ALochPlayerController>(Controller))
	{
		LochController->SetIsAutoRunning(false);
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
	}
}

void ULochHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
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
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void ULochHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
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
		Pawn->AddControllerYawInput(Value.X * LochHero::LookYawRate * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y * LochHero::LookPitchRate * World->GetDeltaSeconds());
	}
}

void ULochHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	/*
	if (ALochCharacter* Character = GetPawn<ALochCharacter>())
	{
		Character->ToggleCrouch();
	}
	*/
}

void ULochHeroComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		if (ALochPlayerController* Controller = Cast<ALochPlayerController>(Pawn->GetController()))
		{
			// Toggle auto running
			Controller->SetIsAutoRunning(!Controller->GetIsAutoRunning());
		}	
	}
}

TSubclassOf<ULochCameraMode> ULochHeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (ULochPawnExtensionComponent* PawnExtComp = ULochPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULochPawnData* PawnData = PawnExtComp->GetPawnData<ULochPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

void ULochHeroComponent::SetAbilityCameraMode(TSubclassOf<ULochCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void ULochHeroComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}
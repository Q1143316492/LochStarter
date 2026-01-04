#include "LochCharacter.h"
#include "AbilitySystemComponent.h"
#include "LochCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "LochGameplayTags.h"
#include "LochLogChannels.h"
#include "Net/UnrealNetwork.h"
#include "Player/LochPlayerController.h"
#include "Player/LochPlayerState.h"
#include "TimerManager.h"

static FName NAME_LochCharacterCollisionProfile_Capsule(TEXT("LyraPawnCapsule")); // 预设名字就不改了
static FName NAME_LochCharacterCollisionProfile_Mesh(TEXT("LyraPawnMesh"));

#include UE_INLINE_GENERATED_CPP_BY_NAME(LochCharacter)

ALochCharacter::ALochCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<ULochCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetNetCullDistanceSquared(900000000.0f);

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComp->SetCollisionProfileName(NAME_LochCharacterCollisionProfile_Capsule);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));  // Rotate mesh to be X forward since it is exported as Y forward.
	MeshComp->SetCollisionProfileName(NAME_LochCharacterCollisionProfile_Mesh);

	ULochCharacterMovementComponent* LochMoveComp = CastChecked<ULochCharacterMovementComponent>(GetCharacterMovement());
	LochMoveComp->GravityScale = 1.0f;
	LochMoveComp->MaxAcceleration = 2400.0f;
	LochMoveComp->BrakingFrictionFactor = 1.0f;
	LochMoveComp->BrakingFriction = 6.0f;
	LochMoveComp->GroundFriction = 8.0f;
	LochMoveComp->BrakingDecelerationWalking = 1400.0f;
	LochMoveComp->bUseControllerDesiredRotation = false;
	LochMoveComp->bOrientRotationToMovement = false;
	LochMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	LochMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	LochMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	LochMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	LochMoveComp->SetCrouchedHalfHeight(65.0f);

    // TODO
    // PawnExtComponent HealthComponent CameraComponent

    bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.0f;
	CrouchedEyeHeight = 50.0f;
}

UAbilitySystemComponent* ALochCharacter::GetAbilitySystemComponent() const
{
	if (ALochPlayerState* LochPS = GetPlayerState<ALochPlayerState>())
	{
		return LochPS->GetAbilitySystemComponent();
	}
	return nullptr;
}

void ALochCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->GetOwnedGameplayTags(TagContainer);
	}
}
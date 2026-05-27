// Fill out your copyright notice in the Description page of Project Settings.



#include "Character/PlayerCharacter.h"

#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "Player/MyPlayerState.h"
#include "Player/PlayerCharacterController.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/STH_HUD.h"

class AMyPlayerState;

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	ApplyTopDownMovementTuning();
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateMouseFacing(DeltaSeconds);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, bUseMouseFacing);
	DOREPLIFETIME(APlayerCharacter, bMovementLockedByAbility);
	DOREPLIFETIME(APlayerCharacter, MovementSpeedMultiplier);
}

void APlayerCharacter::SetUseMouseFacing(bool bInUseMouseFacing)
{
	bUseMouseFacing = bInUseMouseFacing;
	OnRep_UseMouseFacing();
}

void APlayerCharacter::SetMovementLockedByAbility(bool bLocked)
{
	bMovementLockedByAbility = bLocked;
	OnRep_MovementLockedByAbility();
}

void APlayerCharacter::SetMovementSpeedMultiplier(float InMultiplier)
{
	MovementSpeedMultiplier = FMath::Clamp(InMultiplier, MinSpeedMultiplier, MaxSpeedMultiplier);
	OnRep_MovementSpeedMultiplier();
}

void APlayerCharacter::SetMouseFacingTarget(const FVector& WorldTargetLocation)
{
	CachedMouseFacingTarget = WorldTargetLocation;
	bHasMouseFacingTarget = true;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// Init  ability actor info for the Server
	InitAbilityActorInfo();	
	AddCharacterAbilities();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init  ability actor info for the Clients
	InitAbilityActorInfo();
}

int32 APlayerCharacter::GetPlayerLevel()
{
	AMyPlayerState* MyPlayerState = GetPlayerState<AMyPlayerState>();
	check(MyPlayerState);
	
	
	return MyPlayerState->GetPlayerLevel();
}

void APlayerCharacter::OnRep_UseMouseFacing()
{
	if (!bUseMouseFacing)
	{
		bHasMouseFacingTarget = false;
	}
}

void APlayerCharacter::OnRep_MovementSpeedMultiplier()
{
	ApplyMovementSpeed();
}

void APlayerCharacter::OnRep_MovementLockedByAbility()
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		if (bMovementLockedByAbility)
		{
			MoveComp->StopMovementImmediately();
			MoveComp->DisableMovement();
		}
		else
		{
			MoveComp->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}
}

void APlayerCharacter::InitAbilityActorInfo()
{
	AMyPlayerState* MyPlayerState = GetPlayerState<AMyPlayerState>();
	check(MyPlayerState);
	MyPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MyPlayerState, this);
	Cast<UMyAbilitySystemComponent>(MyPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = MyPlayerState->GetAbilitySystemComponent();
	AttributeSet = MyPlayerState->GetAttributeSet();
	
	if (APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(GetController()))
	{
		if (ASTH_HUD* STH_HUD = Cast<ASTH_HUD>(PlayerCharacterController->GetHUD()))
		{
			STH_HUD->InitOverlay(PlayerCharacterController, MyPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}

void APlayerCharacter::ApplyTopDownMovementTuning()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (!MoveComp) return;

	MoveComp->bOrientRotationToMovement = true;
	MoveComp->bUseControllerDesiredRotation = false;
	MoveComp->RotationRate = FRotator(0.f, TopDownRotationRateYaw, 0.f);
	ApplyMovementSpeed();
	MoveComp->MaxAcceleration = TopDownMaxAcceleration;
	MoveComp->BrakingDecelerationWalking = TopDownBrakingDeceleration;
	MoveComp->GroundFriction = TopDownGroundFriction;
}

void APlayerCharacter::ApplyMovementSpeed()
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = TopDownMaxWalkSpeed * MovementSpeedMultiplier;
	}
}

void APlayerCharacter::UpdateMouseFacing(float DeltaSeconds)
{
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->bOrientRotationToMovement = !bUseMouseFacing;
		MoveComp->bUseControllerDesiredRotation = false;
	}

	if (!bUseMouseFacing)
	{
		bHasMouseFacingTarget = false;
	}
}

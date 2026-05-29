// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacterController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "Character/PlayerCharacter.h"
#include "Input/MyInputComponent.h"
#include "Interaction/EnemyInterface.h"


APlayerCharacterController::APlayerCharacterController()
{
	bReplicates = true;
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
	
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (CursorHit.bBlockingHit)
	{
		if (APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
		{
			PlayerCharacter->SetMouseFacingTarget(CursorHit.ImpactPoint);
		}
	}
}

void APlayerCharacterController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();
	
	/**
	 *Line trace from cursor. There are several scenarios: 
	 * A. LastActor is null && ThisActor is nul 
	 *  - do nothing.
	 *  B. LastActor is null && ThisActor is valid
	 *		-Highlight ThisActor
	 *		C. LastActor is valid && ThisActor is null
			-Unhighlight this actor. 
	 */
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
		else
		{
			
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				
			}
		}
	
	}
}

void APlayerCharacterController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	
}

void APlayerCharacterController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void APlayerCharacterController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

UMyAbilitySystemComponent* APlayerCharacterController::GetASC()
{
	if (MyAbilitySystemComponent == nullptr)
	{
		MyAbilitySystemComponent = Cast<UMyAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return MyAbilitySystemComponent;
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	check(InputMappingContext);
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
	
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UMyInputComponent* MyInputComponent = CastChecked<UMyInputComponent>(InputComponent);
	MyInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);
	MyInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void APlayerCharacterController::Move(const FInputActionValue& InputActionValue)
{
	if (const APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		if (PlayerCharacter->IsMovementLockedByAbility())
		{
			return;
		}
	}
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	FRotator BasisRotation = GetControlRotation();
	if (const APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		if (PlayerCharacter->IsUsingMouseFacing())
		{
			BasisRotation = PlayerCharacter->GetActorRotation();
		}
	}

	const FRotator YawRotation(0.f, BasisRotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}



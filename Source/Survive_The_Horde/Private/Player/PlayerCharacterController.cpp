// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacterController.h"

#include "Camera/PlayerCameraManager.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/PlayerCharacter.h"
#include "Interaction/EnemyInterface.h"


APlayerCharacterController::APlayerCharacterController()
{
	bReplicates = true;
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
	FaceMouseCursor();
	
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
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);
	if (LookAction)
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Look);
	}
	if (ComboAttackAction)
	{
		EnhancedInputComponent->BindAction(ComboAttackAction, ETriggerEvent::Started, this, &APlayerCharacterController::ComboAttackPressed);
	}
	if (ChargedAttackAction)
	{
		EnhancedInputComponent->BindAction(ChargedAttackAction, ETriggerEvent::Started, this, &APlayerCharacterController::ChargedAttackPressed);
		EnhancedInputComponent->BindAction(ChargedAttackAction, ETriggerEvent::Completed, this, &APlayerCharacterController::ChargedAttackReleased);
		EnhancedInputComponent->BindAction(ChargedAttackAction, ETriggerEvent::Canceled, this, &APlayerCharacterController::ChargedAttackReleased);
	}
}

void APlayerCharacterController::ComboAttackPressed()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->DoComboAttackStart();
	}
}

void APlayerCharacterController::ChargedAttackPressed()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->DoChargedAttackStart();
	}
}

void APlayerCharacterController::ChargedAttackReleased()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		PlayerCharacter->DoChargedAttackEnd();
	}
}

void APlayerCharacterController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator CameraRotation = PlayerCameraManager
		? PlayerCameraManager->GetCameraRotation()
		: GetControlRotation();
	const FRotator CameraYawRotation(0.f, CameraRotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(CameraYawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(CameraYawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void APlayerCharacterController::Look(const FInputActionValue& InputActionValue)
{
	// The mouse cursor location is already available from the player controller,
	// so the action value only needs to signal that look input happened.
	FaceMouseCursor();
}

void APlayerCharacterController::FaceMouseCursor()
{
	APawn* ControlledPawn = GetPawn<APawn>();
	if (!ControlledPawn)
	{
		return;
	}

	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	FVector LookDirection = CursorHit.ImpactPoint - ControlledPawn->GetActorLocation();
	LookDirection.Z = 0.f;
	if (LookDirection.IsNearlyZero())
	{
		return;
	}

	const FRotator LookRotation = LookDirection.Rotation();
	SetControlRotation(LookRotation);
	ControlledPawn->SetActorRotation(LookRotation);
}


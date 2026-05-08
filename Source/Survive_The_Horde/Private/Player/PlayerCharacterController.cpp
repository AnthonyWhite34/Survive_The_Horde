// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacterController.h"

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
	if (ComboAttackAction)
	{
		EnhancedInputComponent->BindAction(ComboAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::ComboAttackPressed);
	}
	if (ChargedAttackAction)
	{
		EnhancedInputComponent->BindAction(ChargedAttackAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::ChargedAttackPressed);	
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
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}



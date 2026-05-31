// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacterController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MyGameplayTags.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/MyInputComponent.h"
#include "Interaction/EnemyInterface.h"


APlayerCharacterController::APlayerCharacterController()
{
	bReplicates = true;
	
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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

void APlayerCharacterController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FMyGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
	
}

void APlayerCharacterController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void APlayerCharacterController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FMyGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		
	} 
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		FHitResult Hit; 
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}
		
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
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



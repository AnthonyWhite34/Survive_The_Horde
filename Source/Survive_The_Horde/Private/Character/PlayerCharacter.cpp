// Fill out your copyright notice in the Description page of Project Settings.



#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Player/MyPlayerState.h"
#include "Player/PlayerCharacterController.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/STH_HUD.h"

class AMyPlayerState;

APlayerCharacter::APlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// Init  ability actor info for the Server
	InitAbilityActorInfo();	
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

void APlayerCharacter::NotifyEnemiesOfIncomingAttack()
{
	// sweep for objects in front of the character to be hit by the attack
	TArray<FHitResult> OutHits;

	// start at the actor location, sweep forward
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * DangerTraceDistance);

	// check for pawn object types only
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	// use a sphere shape for the sweep
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(DangerTraceRadius);

	// ignore self
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->SweepMultiByObjectType(OutHits, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, CollisionShape, QueryParams))
	{
		// iterate over each object hit
		for (const FHitResult& CurrentHit : OutHits)
		{
			// check if we've hit a damageable actor
			if (ICombatDamageable* Damageable = Cast<ICombatDamageable>(CurrentHit.GetActor()))
			{
				// notify the enemy
				Damageable->NotifyDanger(GetActorLocation(), this);
			}
		}
	}
}

void APlayerCharacter::DoAttackTrace(FName DamageSourceBone)
{
	TArray<FHitResult> OutHits;
	TSet<AActor*> DamagedActors;

	const FVector TraceStart = GetMesh()->GetSocketLocation(DamageSourceBone);
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * MeleeTraceDistance);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(MeleeTraceRadius);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->SweepMultiByObjectType(OutHits, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, CollisionShape, QueryParams))
	{
		for (const FHitResult& CurrentHit : OutHits)
		{
			AActor* HitActor = CurrentHit.GetActor();
			if (!HitActor || DamagedActors.Contains(HitActor))
			{
				continue;
			}

			if (ICombatDamageable* Damageable = Cast<ICombatDamageable>(HitActor))
			{
				DamagedActors.Add(HitActor);

				const FVector Impulse = (CurrentHit.ImpactNormal * -MeleeKnockbackImpulse) + (FVector::UpVector * MeleeLaunchImpulse);
				Damageable->ApplyDamage(MeleeDamage, this, CurrentHit.ImpactPoint, Impulse);

				DealtDamage(MeleeDamage, CurrentHit.ImpactPoint);
			}
		}
	}
}

void APlayerCharacter::CheckCombo()
{
	if (bIsAttacking && !bIsChargingAttack)
	{
		const UWorld* World = GetWorld();
		if (World && World->GetTimeSeconds() - CachedAttackInputTime <= ComboInputCacheTimeTolerance)
		{
			CachedAttackInputTime = -1.0f;
			++ComboCount;

			if (ComboCount < ComboSectionNames.Num())
			{
				NotifyEnemiesOfIncomingAttack();

				if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
				{
					AnimInstance->Montage_JumpToSection(ComboSectionNames[ComboCount], ComboAttackMontage);
				}
			}
		}
	}
}

void APlayerCharacter::CheckChargedAttack()
{
	// raise the looped charged attack flag
	bHasLoopedChargedAttack = true;

	// jump to either the loop or the attack section depending on whether we're still holding the charge button
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_JumpToSection(bIsChargingAttack ? ChargeLoopSection : ChargeAttackSection, ChargedAttackMontage);
	}
}

void APlayerCharacter::DoComboAttackStart()
{
	// are we already playing an attack animation?
	if (bIsAttacking)
	{
		// cache the input time so we can check it later
		CachedAttackInputTime = GetWorld()->GetTimeSeconds();

		return;
	}

	// perform a combo attack
	ComboAttack();
}

void APlayerCharacter::DoComboAttackEnd()
{
	// stub
}

void APlayerCharacter::DoChargedAttackStart()
{
	// raise the charging attack flag
	bIsChargingAttack = true;

	if (bIsAttacking)
	{
		// cache the input time so we can check it later
		CachedAttackInputTime = GetWorld()->GetTimeSeconds();

		return;
	}

	ChargedAttack();
}

void APlayerCharacter::DoChargedAttackEnd()
{
	// lower the charging attack flag
	bIsChargingAttack = false;

	// if we've done the charge loop at least once, release the charged attack right away
	if (bHasLoopedChargedAttack)
	{
		CheckChargedAttack();
	}
}

void APlayerCharacter::ComboAttack()
{
	if (!ComboAttackMontage)
	{
		return;
	}
	// raise the attacking flag
	bIsAttacking = true;

	// reset the combo count
	ComboCount = 0;

	// notify enemies they are about to be attacked
	NotifyEnemiesOfIncomingAttack();

	// play the attack montage
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		const float MontageLength = AnimInstance->Montage_Play(ComboAttackMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

		// subscribe to montage completed and interrupted events
		if (MontageLength > 0.0f)
		{
			// set the end delegate for the montage
			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, ComboAttackMontage);
		}
		else
		{
			bIsAttacking = false;
		}
	}
	else
	{
		bIsAttacking = false;
	}

}

void APlayerCharacter::ChargedAttack()
{
	if (!ChargedAttackMontage)
	{
		return;
	}
	
	// raise the attacking flag
	bIsAttacking = true;

	// reset the charge loop flag
	bHasLoopedChargedAttack = false;

	// notify enemies they are about to be attacked
	NotifyEnemiesOfIncomingAttack();

	// play the charged attack montage
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		const float MontageLength = AnimInstance->Montage_Play(ChargedAttackMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

		// subscribe to montage completed and interrupted events
		if (MontageLength > 0.0f)
		{
			// set the end delegate for the montage
			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, ChargedAttackMontage);
		}
		else
		{
			bIsAttacking = false;
		}
	}
	else
	{
		bIsAttacking = false;
	}
}

void APlayerCharacter::AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;

	const UWorld* World = GetWorld();
	if (World && World->GetTimeSeconds() - CachedAttackInputTime <= AttackInputCacheTimeTolerance)
	{
		CachedAttackInputTime = -1.0f;

		if (bIsChargingAttack)
		{
			ChargedAttack();
		}
		else
		{
			ComboAttack();
		}
	}
}

void APlayerCharacter::OnDamageReceived(float Damage, const FVector& DamageLocation, const FVector& DamageDirection)
{
	ReceivedDamage(Damage, DamageLocation, DamageDirection);
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

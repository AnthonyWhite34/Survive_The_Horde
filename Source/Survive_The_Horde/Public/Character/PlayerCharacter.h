// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVE_THE_HORDE_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
public:
	APlayerCharacter();
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	void SetUseMouseFacing(bool bInUseMouseFacing);
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	void SetMovementLockedByAbility(bool bLocked);

	UFUNCTION(BlueprintPure, Category="Movement")
	bool IsMovementLockedByAbility() const { return bMovementLockedByAbility; }
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	void SetMovementSpeedMultiplier(float InMultiplier);
	
	UFUNCTION(BlueprintCallable, Category="Movement") 
	void SetMouseFacingTarget(const FVector& WorldTargetLocation);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Tuning")
	float TopDownMaxWalkSpeed = 600.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Tuning")
	float TopDownMaxAcceleration = 2200.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Tuning")
	float TopDownBrakingDeceleration = 1800.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Tuning")
	float TopDownGroundFriction = 6.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Tuning")
	float TopDownRotationRateYaw = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Tuning")
	float MouseFacingInterpSpeed = 12.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Tuning")
	float MinSpeedMultiplier = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Tuning")
	float MaxSpeedMultiplier = 2.f;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface */
	
private:
	UFUNCTION()
	void OnRep_UseMouseFacing();

	UFUNCTION()
	void OnRep_MovementSpeedMultiplier();

	UFUNCTION()
	void OnRep_MovementLockedByAbility();

	virtual void InitAbilityActorInfo() override;
	
	void ApplyTopDownMovementTuning();
	void ApplyMovementSpeed();
	void UpdateMouseFacing(float DeltaSeconds);

	UPROPERTY(ReplicatedUsing=OnRep_UseMouseFacing, EditAnywhere, Category="Movement")
	bool bUseMouseFacing = true;

	UPROPERTY(ReplicatedUsing=OnRep_MovementLockedByAbility, EditAnywhere, Category="Movement")
	bool bMovementLockedByAbility = false;

	UPROPERTY(ReplicatedUsing=OnRep_MovementSpeedMultiplier, EditAnywhere, Category="Movement")
	float MovementSpeedMultiplier = 1.f;
	FVector CachedMouseFacingTarget = FVector::ZeroVector;
	bool bHasMouseFacingTarget = false;
};

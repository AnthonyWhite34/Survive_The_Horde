// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "CombatAttacker.h"
#include "CombatDamageable.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVE_THE_HORDE_API APlayerCharacter : public ACharacterBase, public ICombatAttacker, public ICombatDamageable
{
	GENERATED_BODY()
public: 
	APlayerCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface */
	
	//delete start here if below doesn't work, also delete ICombatAttacker, and ICombatDamageable dirived interfaces
	
	/** Notifies nearby enemies that an attack is coming so they can react */
	void NotifyEnemiesOfIncomingAttack();
	
	/** Performs an attack collision check from an animation notify */
	virtual void DoAttackTrace(FName DamageSourceBone) override;

	/** Performs the combo string check from an animation notify */
	virtual void CheckCombo() override;
	
	/** Performs the charged attack hold check */
	virtual void CheckChargedAttack() override;
	
	/** Handles combo attack pressed from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoComboAttackStart();

	/** Handles combo attack released from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoComboAttackEnd();

	/** Handles charged attack pressed from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoChargedAttackStart();

	/** Handles charged attack released from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoChargedAttackEnd();

protected:
	
	/** Index of the current stage of the melee attack combo */
	int32 ComboCount = 0;
	
	/** Time at which an attack button was last pressed */
	float CachedAttackInputTime = -1.0f;
	
	/** If true, the character is currently playing an attack animation */
	bool bIsAttacking = false;
	
	UPROPERTY(EditAnywhere, Category="Melee Attack", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float AttackInputCacheTimeTolerance = 1.0f;
	
	/** Flag that determines if the player is currently holding the charged attack input */
	bool bIsChargingAttack = false;
	
	/** If true, the charged attack hold check has been tested at least once */
	bool bHasLoopedChargedAttack = false;
	
	/** Distance ahead of the attack bone that melee attack sphere collision traces will extend */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Trace", meta = (ClampMin = 0, ClampMax = 500, Units="cm"))
	float MeleeTraceDistance = 75.0f;

	/** Radius of the sphere trace for melee attacks */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Trace", meta = (ClampMin = 0, ClampMax = 200, Units = "cm"))
	float MeleeTraceRadius = 75.0f;

	/** Amount of damage a melee attack will deal */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 100))
	float MeleeDamage = 1.0f;

	/** Amount of knockback impulse a melee attack will apply */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm/s"))
	float MeleeKnockbackImpulse = 250.0f;

	/** Amount of upwards impulse a melee attack will apply */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm/s"))
	float MeleeLaunchImpulse = 300.0f;
	
	/** Performs a combo attack */
	void ComboAttack();

	/** Performs a charged attack */
	void ChargedAttack();
	
	/** AnimMontage that will play for combo attacks */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Combo")
	UAnimMontage* ComboAttackMontage;
	
	
	/** Names of the AnimMontage sections that correspond to each stage of the combo attack */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Combo")
	TArray<FName> ComboSectionNames;

	/** Max amount of time that may elapse for a combo attack input to not be considered stale */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Combo", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float ComboInputCacheTimeTolerance = 0.45f;
	
	/** Attack montage ended delegate */
	FOnMontageEnded OnAttackMontageEnded;
	
	/** AnimMontage that will play for charged attacks */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Charged")
	UAnimMontage* ChargedAttackMontage;
	
	/** Distance ahead of the character that enemies will be notified of incoming attacks */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Trace", meta = (ClampMin = 0, ClampMax = 500, Units="cm"))
	float DangerTraceDistance = 300.0f;
	
	/** Distance ahead of the character that enemies will be notified of incoming attacks */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Trace", meta = (ClampMin = 0, ClampMax = 200, Units = "cm"))
	float DangerTraceRadius = 100.0f;
	
	UPROPERTY(EditAnywhere, Category="Melee Attack|Charged")
	FName ChargeLoopSection;
	
	/** Name of the AnimMontage section that corresponds to the attack */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Charged")
	FName ChargeAttackSection;
	
	
	/** Called from a delegate when the attack montage ends */
	void AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/** Blueprint handler to play damage dealt effects */
	UFUNCTION(BlueprintImplementableEvent, Category="Combat")
	void DealtDamage(float Damage, const FVector& ImpactPoint);

	/** Blueprint handler to play damage received effects */
	UFUNCTION(BlueprintImplementableEvent, Category="Combat")
	void ReceivedDamage(float Damage, const FVector& ImpactPoint, const FVector& DamageDirection);

	/** CharacterBase hook for player-specific damage feedback. */
	virtual void OnDamageReceived(float Damage, const FVector& DamageLocation, const FVector& DamageDirection) override;
	
	//delete end here if breaks
	
private:
	virtual void InitAbilityActorInfo() override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "CombatAttacker.h"
#include "CombatDamageable.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVE_THE_HORDE_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
	APlayerCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface */
	
	//delete start here if below doesn't work, also delete ICombatAttacker, and ICombatDamageable dirived interfaces
public:
	
	/** Notifies nearby enemies that an attack is coming so they can react */
	void NotifyEnemiesOfIncomingAttack();
	
	/** Performs the charged attack hold check */
	virtual void CheckChargedAttack();
	
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
	float CachedAttackInputTime = 0.0f;
	
	/** If true, the character is currently playing an attack animation */
	bool bIsAttacking = false;
	
	/** Flag that determines if the player is currently holding the charged attack input */
	bool bIsChargingAttack = false;
	
	/** If true, the charged attack hold check has been tested at least once */
	bool bHasLoopedChargedAttack = false;
	
	/** Performs a combo attack */
	void ComboAttack();

	/** Performs a charged attack */
	void ChargedAttack();
	
	/** AnimMontage that will play for combo attacks */
	UPROPERTY(EditAnywhere, Category="Melee Attack|Combo")
	UAnimMontage* ComboAttackMontage;
	
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
	
	//delete end here if breaks
	
private:
	virtual void InitAbilityActorInfo() override;
	
};

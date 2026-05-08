// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
/**
 * 
 */
UCLASS()
class SURVIVE_THE_HORDE_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerCharacterController();
	virtual void PlayerTick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;
	
	/** Combo Attack Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* ComboAttackAction;

	/** Charged Attack Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* ChargedAttackAction;
	
	bool bIsAttacking = false;
	
	int32 ComboCount = 0;

private:
	
	/** Called for combo attack input */
	void ComboAttackPressed();

	/** Called for combo attack input pressed */
	void ChargedAttackPressed();

	/** Called for combo attack input released */
	void ChargedAttackReleased();
	
	void Move(const FInputActionValue& InputActionValue);
	


	void CursorTrace();
	
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
};

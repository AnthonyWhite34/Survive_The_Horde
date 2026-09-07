// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;

/**
 * 
 */
UCLASS()
class SURVIVE_THE_HORDE_API AEnemyCharacter : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	AEnemyCharacter();
	
public:
	/* Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* End Enemy Interface */
	
	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface */
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;
protected:	
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWRite, Category = "Character Class Defaults")
    int32 Level = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWRite, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
};

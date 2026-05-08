// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVE_THE_HORDE_API AEnemyCharacter : public ACharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	/* Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* End Enemy Interface */
	
	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface */
	
	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;
	
	virtual void BeginPlay() override;
	
	virtual void InitAbilityActorInfo() override;
	

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWRite, Category = "Character Class Defaults")
    	int32 Level = 1;
	
};

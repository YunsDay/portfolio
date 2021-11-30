// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateType.h"
#include "CharacterState.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCharacterState : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		ATPSCharacter* m_pOwnerRef;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		bool m_bIsStaminaUse;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		EStateType m_eStateType;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		TArray<float> m_arPlayerState;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		TArray<float> m_arPlayerStateBase;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		bool UseStaminaIfCan(float fUseAmount);
	UFUNCTION(BlueprintCallable)
		float GetHpPercent();
	UFUNCTION(BlueprintCallable)
		float GetStaminaPercent();
	UFUNCTION(BlueprintCallable)
		void TakeDamage(float fDamage);
	UFUNCTION(BlueprintCallable)
		void Init(ATPSCharacter* cOwenr);
	UFUNCTION(BlueprintCallable)
		void SetIsStaminaUse(bool bIsStaminaUse);
	UFUNCTION(BlueprintCallable)
		float GetState(EStateType eStateType);
	UFUNCTION(BlueprintCallable)
		void SetState(EStateType eStateType, float fValue);
	UFUNCTION(BlueprintCallable)
		void SetStateEquipWeapon(bool EquipUnEquip);
	UFUNCTION(BlueprintCallable)
		void Recovery(EStateType eStateType, float fRecoveryAmount);
private:
	UFUNCTION(BlueprintCallable)
		void SetStamina(float fRecoveryAmount);

		
};

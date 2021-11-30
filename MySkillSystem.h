// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructSkill.h"
#include "MySkillSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventBinding);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UMySkillSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMySkillSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		TArray<float> m_arMaxCooltime;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		TArray<float> m_arCooltime;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		TArray<bool> m_arSkillReady;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		TArray<FS_Skill> m_arSkill;

public:
	UFUNCTION(BlueprintCallable)
		void CoolTimeTick(float fTick, int nIndex);
	UFUNCTION(BlueprintCallable)
		void SetSkill(int nIndex, UPARAM(ref)const FS_Skill &sSkill);
	UFUNCTION(BlueprintCallable)
		void StartCoolTime(int nIndex);
	UFUNCTION(BlueprintCallable)
		bool GetSkillReady(int nIndex);
	UFUNCTION(BlueprintCallable)
		void GetSkillInfo(int nIndex, FS_Skill & sSkillInfo);
	UFUNCTION(BlueprintCallable)
		void UseSkill(int nIndex);

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, VisibleDefaultsOnly)
		FEventBinding SetSkillImageBinding;
};

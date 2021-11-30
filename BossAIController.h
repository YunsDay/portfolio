// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API ABossAIController : public AAIController
{
    GENERATED_BODY()

public:
    ABossAIController();

    UFUNCTION(BlueprintCallable)
        void RunMachanic();
    UPROPERTY()
        UBehaviorTree* m_btBossBehavior;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"

ABossAIController::ABossAIController():
	m_btBossBehavior{}
{
	

}

void ABossAIController::RunMachanic()
{
	FString strBehaviorTreePath = TEXT("BehaviorTree'/Game/Game/Enemy/Spider_Tank/BehaviorTree/BT_BossSpiderTank.BT_BossSpiderTank'");
	m_btBossBehavior = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(), NULL, *strBehaviorTreePath));
	if(m_btBossBehavior)
		RunBehaviorTree(m_btBossBehavior);
}
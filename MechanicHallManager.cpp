// Fill out your copyright notice in the Description page of Project Settings.


#include "MechanicHallManager.h"
#include "GasEngineTrap.h"
#include "BossSpiderTank.h"
#include "TPSCharacter.h"
#include "WarpGate.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionBox.h"
#include "WayPoint.h"
#include "TriggerCollision.h"
#include "Door.h"
#include "WarpGateLevel.h"

// Sets default values
AMechanicHallManager::AMechanicHallManager():
	m_pSpiderTank{},
	m_pPlayer{},
	m_pGasEngine{},
	m_pShotDownInteractionBox{},
	m_pBetteryInteractionBox{},
	m_pWarpGate{},
	m_pWayPoint{},
	m_pTriggerBox{},
	m_pDoor{},
	m_arMissionText{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void AMechanicHallManager::BeginPlay()
{
	Super::BeginPlay();
	m_pPlayer = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (m_cCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, TEXT("RotatorUpdate"));
		TimelineFinishedCallback.BindUFunction(this, TEXT("TimelineFinished"));

		m_TimelineRotator.AddInterpFloat(m_cCurve, TimelineCallback);
		m_TimelineRotator.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}

	m_pShotDownInteractionBox->TakeEndEvent.AddDynamic(this, &AMechanicHallManager::ShotDownEngine);
	m_pBetteryInteractionBox->TakeEndEvent.AddDynamic(this, &AMechanicHallManager::EndQuest);
	m_pSpiderTank->DeathEventBinding.AddDynamic(this, &AMechanicHallManager::BossDeath);
	m_pTriggerBox->TriggerEvent.AddDynamic(this, &AMechanicHallManager::TriggedBox);
}

// Called every frame
void AMechanicHallManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_TimelineRotator.TickTimeline(DeltaTime);
}

void AMechanicHallManager::EndQuest()
{
	InformationOpenInteractionUnbinding(m_arMissionText[2], false);
	m_pWarpGate->SetWarpActivate(true);
}

void AMechanicHallManager::ShotDownEngine()
{
	m_pPlayer = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	m_TimelineRotator.PlayFromStart();
	m_pWayPoint->SetMoveNextWayPoint();
}

void AMechanicHallManager::Interaction()
{
	m_pPlayer->CloseInformation();
	m_pPlayer->InteractionEventBinding.Remove(this,TEXT("Interaction"));
}

void AMechanicHallManager::BossDeath()
{
	DoorActive();
	m_pWayPoint->SetMoveNextWayPoint();
	InformationOpenInteractionUnbinding(m_arMissionText[1], false);
	m_pTriggerBox->SetActive(true);
}

void AMechanicHallManager::DoorActive()
{
	m_pDoor->ActivateDoor();
}

void AMechanicHallManager::TriggedBox()
{
	m_pWayPoint->SetMoveNextWayPoint();
	m_pTriggerBox->SetActive(false);
	m_pTriggerBox->TriggerEvent.Clear();
}

void AMechanicHallManager::RotatorUpdate()
{
	float fRotatorSpeed;
	fRotatorSpeed = m_cCurve->GetFloatValue(m_TimelineRotator.GetPlaybackPosition());
	m_pGasEngine->SetRotatorSpeed(fRotatorSpeed, fRotatorSpeed, fRotatorSpeed, fRotatorSpeed,0.0f);
}

void AMechanicHallManager::TimelineFinished()
{
	m_pGasEngine->ShotDownEngine();
	m_pSpiderTank->SetPlayer(m_pPlayer);

	FTimerHandle sWaitHandle{};
	float fWaitTime{};
	fWaitTime = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(sWaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		m_pSpiderTank->StartMachanic();

		FTimerHandle sTimeHandle{};
		float fWaitTime{};
		fWaitTime = 1.0f;
		GetWorld()->GetTimerManager().SetTimer(sTimeHandle, FTimerDelegate::CreateLambda([&]()
		{
			InformationOpenInteractionUnbinding(m_arMissionText[0], false);

		}), fWaitTime, false);

	}), fWaitTime, false);


}

void AMechanicHallManager::InformationOpenInteractionUnbinding(UPARAM(ref) const FText& tInformation, bool bMoveLimit)
{
	m_pPlayer->OpenInformation(tInformation, bMoveLimit);
	m_pPlayer->InteractionEventBinding.AddDynamic(this, &AMechanicHallManager::Interaction);
}

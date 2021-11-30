// Fill out your copyright notice in the Description page of Project Settings.


#include "WayPoint.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWayPoint::AWayPoint():
	m_RootComponent{},
	m_StaticMesh{},
	m_vLocation{},
	m_pNextWayPointLocation{},
	m_nMaxPointIndex{},
	m_nNextPointIndex{},
	m_bFlipFlop{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = m_RootComponent;

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_StaticMesh->SetupAttachment(reinterpret_cast<USceneComponent*>(m_RootComponent));

}

// Called when the game starts or when spawned
void AWayPoint::BeginPlay()
{
	Super::BeginPlay();

	if (m_cCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, TEXT("UpdateLocation"));
		TimelineFinishedCallback.BindUFunction(this, TEXT("TimelineFinished"));

		m_TimelineLocation.AddInterpFloat(m_cCurve, TimelineCallback);
		m_TimelineLocation.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
	m_vLocation = GetActorLocation();
	m_TimelineLocation.Play();

	m_nNextPointIndex = 0;
	
}

// Called every frame
void AWayPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_TimelineLocation.TickTimeline(DeltaTime);
}

void AWayPoint::SetMoveNextWayPoint()
{
	if (m_nNextPointIndex < m_nMaxPointIndex)
		m_vLocation = m_pNextWayPointLocation[m_nNextPointIndex]->GetActorLocation();
	else
		EndNextPosition();

	m_nNextPointIndex++;
}

void AWayPoint::SetVisibility(bool bVisible)
{
	m_StaticMesh->SetVisibility(bVisible);
}

void AWayPoint::EndNextPosition()
{
	SetVisibility(false);
	m_TimelineLocation.Stop();
}

void AWayPoint::PlayTimeline()
{
	m_TimelineLocation.Play();
}

void AWayPoint::ReverseTimeline()
{
	m_TimelineLocation.Reverse();
}

void AWayPoint::UpdateLocation()
{
	SetActorLocation(FVector(m_vLocation.X, m_vLocation.Y, UKismetMathLibrary::Lerp(m_vLocation.Z,m_vLocation.Z+100.0f,m_cCurve->GetFloatValue(m_TimelineLocation.GetPlaybackPosition()))));
}

void AWayPoint::TimelineFinished()
{
	if (m_bFlipFlop)
	{
		PlayTimeline();
		m_bFlipFlop = false;
	}
	else
	{
		ReverseTimeline();
		m_bFlipFlop = true;
	}
}


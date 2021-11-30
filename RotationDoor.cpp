// Fill out your copyright notice in the Description page of Project Settings.


#include "RotationDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARotationDoor::ARotationDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_StaticDoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	RootComponent = m_StaticDoorFrame;

	m_StaticDoorA = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorA"));
	m_StaticDoorA->SetupAttachment(reinterpret_cast<USceneComponent*>(m_StaticDoorFrame));

	m_StaticDoorB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorB"));
	m_StaticDoorB->SetupAttachment(reinterpret_cast<USceneComponent*>(m_StaticDoorFrame));

	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	m_BoxComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(m_StaticDoorFrame));
	m_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ARotationDoor::OnOverlapBegin);
	m_BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ARotationDoor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ARotationDoor::BeginPlay()
{
	Super::BeginPlay();

	m_BoxComponent->SetGenerateOverlapEvents(m_bDoorActivate);
	if (m_cCurve)
	{
		FOnTimelineFloat TimelineCallback;

		TimelineCallback.BindUFunction(this, TEXT("TimelineUpdate"));

		m_TimelineRotation.AddInterpFloat(m_cCurve, TimelineCallback);
	}
}

// Called every frame
void ARotationDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_TimelineRotation.TickTimeline(DeltaTime);
}

void ARotationDoor::OpenDoor()
{
	m_TimelineRotation.Play();
}

void ARotationDoor::CloseDoor()
{
	m_TimelineRotation.Reverse();
}

void ARotationDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OpenDoor();
}

void ARotationDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CloseDoor();
}

void ARotationDoor::TimelineUpdate()
{
	m_StaticDoorA->SetRelativeRotation(FRotator(0.0f, UKismetMathLibrary::Lerp(0.0f,60.0f,m_cCurve->GetFloatValue(m_TimelineRotation.GetPlaybackPosition()))*-1.0f, 0.0f));
	m_StaticDoorB->SetRelativeRotation(FRotator(0.0f, UKismetMathLibrary::Lerp(0.0f, 60.0f, m_cCurve->GetFloatValue(m_TimelineRotation.GetPlaybackPosition())), 0.0f));
}


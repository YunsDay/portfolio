// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADoor::ADoor():
	m_StaticDoorFrame{},
	m_StaticDoorA{},
	m_StaticDoorB{},
	m_BoxComponent{},
	m_bDoorActivate{}
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
	m_BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&ADoor::OnOverlapBegin);
	m_BoxComponent->OnComponentEndOverlap.AddDynamic(this,&ADoor::OnOverlapEnd);

	

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	m_BoxComponent->SetGenerateOverlapEvents(m_bDoorActivate);
	if (m_cCurve)
	{
		FOnTimelineFloat TimelineCallback;

		TimelineCallback.BindUFunction(this, TEXT("TimelineUpdate"));

		m_TimelineLocation.AddInterpFloat(m_cCurve, TimelineCallback);
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_TimelineLocation.TickTimeline(DeltaTime);

}

void ADoor::OpenDoor()
{
	m_TimelineLocation.Play();
}

void ADoor::CloseDoor()
{
	m_TimelineLocation.Reverse();
}

void ADoor::ActivateDoor()
{
	m_BoxComponent->SetGenerateOverlapEvents(true);
}

void ADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OpenDoor();
}

void ADoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CloseDoor();
}

void ADoor::TimelineUpdate()
{
	m_StaticDoorA->SetRelativeLocation(FVector(0.0f, UKismetMathLibrary::Lerp(0.0f, 90.0f, m_cCurve->GetFloatValue(m_TimelineLocation.GetPlaybackPosition())),0.0f));
	m_StaticDoorB->SetRelativeLocation(FVector(0.0f, UKismetMathLibrary::Lerp(0.0f, 90.0f, m_cCurve->GetFloatValue(m_TimelineLocation.GetPlaybackPosition())) * -1.0f, 0.0f));
}

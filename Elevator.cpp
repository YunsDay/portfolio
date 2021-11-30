// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AElevator::AElevator():
	m_StaticElevator{},
	m_StaticDoorA{},
	m_StaticDoorB{},
	m_BoxComponent{},
	m_MoveBoxComponent{},
	m_TimelineLocation{},
	m_fLiftDistance{},
	m_bUpDown{},
	m_vCurrentLocation{},
	m_cRotationCurve{},
	m_cCurve{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_StaticElevator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	RootComponent = m_StaticElevator;

	m_StaticDoorA = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorA"));
	m_StaticDoorA->SetupAttachment(reinterpret_cast<USceneComponent*>(m_StaticElevator));

	m_StaticDoorB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorB"));
	m_StaticDoorB->SetupAttachment(reinterpret_cast<USceneComponent*>(m_StaticElevator));

	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	m_BoxComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(m_StaticElevator));
	m_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnOverlapBegin);
	m_BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AElevator::OnOverlapEnd);

	m_MoveBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MoveBoxCollision"));
	m_MoveBoxComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(m_StaticElevator));
	m_MoveBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnMoveOverlapBegin);
	m_MoveBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AElevator::OnMoveOverlapEnd);
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();
	if (m_cRotationCurve)
	{
		FOnTimelineFloat TimelineCallbackRotation;
		

		TimelineCallbackRotation.BindUFunction(this, TEXT("TimelineRotationUpdate"));
		m_TimelineRotation.AddInterpFloat(m_cRotationCurve, TimelineCallbackRotation);
	}
	if (m_cCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedCallback;
		
		TimelineCallback.BindUFunction(this, TEXT("TimelineLocationUpdate"));
		TimelineFinishedCallback.BindUFunction(this, TEXT("TimelineFinished"));

		m_TimelineLocation.AddInterpFloat(m_cCurve, TimelineCallback);
		m_TimelineLocation.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}

	m_vCurrentLocation = GetActorLocation();
}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_TimelineLocation.TickTimeline(DeltaTime);
	m_TimelineRotation.TickTimeline(DeltaTime);
}

void AElevator::OpenDoor()
{
	m_TimelineRotation.Play();
}

void AElevator::CloseDoor()
{
	m_TimelineRotation.Reverse();
}

void AElevator::MoveLift()
{
	if (m_bUpDown)
		m_TimelineLocation.PlayFromStart();
	else
		m_TimelineLocation.ReverseFromEnd();
}

void AElevator::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OpenDoor();
}

void AElevator::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CloseDoor();
}

void AElevator::OnMoveOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_BoxComponent->SetGenerateOverlapEvents(false);
	FTimerHandle sWaitHandle{};
	float fWaitTime{};
	fWaitTime = 1.5f;
	GetWorld()->GetTimerManager().SetTimer(sWaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		MoveLift();

	}), fWaitTime, false);
	
}

void AElevator::OnMoveOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (m_bUpDown)
		m_bUpDown = false;
	else
		m_bUpDown = true;
}

void AElevator::TimelineLocationUpdate()
{
	m_StaticElevator->SetRelativeLocation(FVector(m_vCurrentLocation.X, m_vCurrentLocation.Y, UKismetMathLibrary::Lerp(m_vCurrentLocation.Z, m_vCurrentLocation.Z+m_fLiftDistance, m_cCurve->GetFloatValue(m_TimelineLocation.GetPlaybackPosition()))));
}

void AElevator::TimelineFinished()
{
	m_BoxComponent->SetGenerateOverlapEvents(true);
}

void AElevator::TimelineRotationUpdate()
{
	m_StaticDoorA->SetRelativeRotation(FRotator(0.0f, UKismetMathLibrary::Lerp(0.0f, 60.0f, m_cRotationCurve->GetFloatValue(m_TimelineRotation.GetPlaybackPosition())), 0.0f));
	m_StaticDoorB->SetRelativeRotation(FRotator(0.0f, UKismetMathLibrary::Lerp(0.0f, 60.0f, m_cRotationCurve->GetFloatValue(m_TimelineRotation.GetPlaybackPosition())) * -1.0f, 0.0f));
}


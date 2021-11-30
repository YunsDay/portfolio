// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TPSCharacter.h"

// Sets default values
AInteractionBox::AInteractionBox():
	m_StaticMesh{},
	m_BoxComponent{},
	m_bTake{},
	m_fCooltime{},
	m_tCooltimeText{},
	m_eInteractionType{},
	m_pPlayer{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = m_StaticMesh;

	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	m_BoxComponent->SetRelativeLocation(FVector(0.0f,0.0f,0.0f));
	m_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractionBox::OnOverlapBegin);
	m_BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractionBox::OnOverlapEnd);

}

// Called when the game starts or when spawned
void AInteractionBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractionBox::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	TakeEndEvent.Clear();
}

// Called every frame
void AInteractionBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractionBox::TakeEvent()
{
	m_pPlayer->CloseInteractionUI();
	m_pPlayer->EndCoolTimeEventBinding.AddDynamic(this, &AInteractionBox::Gathering);
	m_pPlayer->Cooltime(m_fCooltime,m_tCooltimeText);
}

void AInteractionBox::Gathering()
{
	m_pPlayer->InteractionEventBinding.Remove(this,TEXT("TakeEvent"));
	m_bTake = true;
	if (TakeEndEvent.IsBound())
		TakeEndEvent.Broadcast();
	m_pPlayer->EndCoolTimeEventBinding.Clear();
}

void AInteractionBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_pPlayer = Cast<ATPSCharacter>(OtherActor);
	if (m_pPlayer && !m_bTake)
	{
		m_pPlayer->OpenInteractionUI(m_eInteractionType);
		m_pPlayer->InteractionEventBinding.AddDynamic(this,&AInteractionBox::TakeEvent);
	}
}

void AInteractionBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_pPlayer = Cast<ATPSCharacter>(OtherActor);
	if (m_pPlayer)
	{
		m_pPlayer->CloseInteractionUI();
		m_pPlayer->InteractionEventBinding.Remove(this,TEXT("TakeEvent"));
	}
}


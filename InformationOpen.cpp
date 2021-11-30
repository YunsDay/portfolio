// Fill out your copyright notice in the Description page of Project Settings.


#include "InformationOpen.h"
#include "TPSCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AInformationOpen::AInformationOpen():
	m_BoxComponent{},
	m_tText{},
	m_bMoveLimit{},
	m_pPlayer{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));

	m_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInformationOpen::OnOverlapBegin);
	m_BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AInformationOpen::OnOverlapEnd);
	
	
	
}

// Called when the game starts or when spawned
void AInformationOpen::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInformationOpen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInformationOpen::InteractionEvent()
{
	if (m_pPlayer)
		m_pPlayer->CloseInformation();
	Destroy();
}

void AInformationOpen::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_pPlayer = Cast<ATPSCharacter>(OtherActor);
	if (m_pPlayer)
	{
		m_pPlayer->OpenInformation(m_tText,m_bMoveLimit);
		m_pPlayer->InteractionEventBinding.AddDynamic(this, &AInformationOpen::InteractionEvent);
	}
}

void AInformationOpen::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_pPlayer = Cast<ATPSCharacter>(OtherActor);
	if (m_pPlayer)
	{
		m_pPlayer->CloseInformation();
		m_pPlayer->InteractionEventBinding.Remove(this, TEXT("InteractionEvent"));
	}
	m_pPlayer = nullptr;
}


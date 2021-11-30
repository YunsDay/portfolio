// Fill out your copyright notice in the Description page of Project Settings.


#include "GiveItemBox.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "TPSCharacter.h"
#include "InteractionType.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AGiveItemBox::AGiveItemBox():
	m_StaticMesh{},
	m_SphereComponent{},
	m_Particle{},
	m_bGet{},
	m_fCooltime{},
	m_tCooltimeText{},
	m_arGiveItem{},
	m_nArraySize{},
	m_pPlayer{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = m_StaticMesh;

	m_SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	m_SphereComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	m_SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AGiveItemBox::OnOverlapBegin);
	m_SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AGiveItemBox::OnOverlapEnd);

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
}

void AGiveItemBox::FinishGetItem()
{
	m_pPlayer->CloseInteractionUI();
	m_Particle->SetTemplate(nullptr);
	m_SphereComponent->SetGenerateOverlapEvents(false);
	m_bGet = true;

}

void AGiveItemBox::GiveItem()
{
	m_pPlayer->CloseInteractionUI();
	if (!m_bGet)
	{
		m_pPlayer->EndCoolTimeEventBinding.AddDynamic(this, &AGiveItemBox::EndCooltime);
		m_pPlayer->Cooltime(m_fCooltime, m_tCooltimeText);
	}
}

void AGiveItemBox::EndCooltime()
{
	for (int i = 0; i < m_nArraySize; i++)
	{
		for (int j = 1; j <= m_arGiveItem[i].nAmount; j++)
		{
			m_pPlayer->AddItemFromDataTable(m_arGiveItem[i].tItemDataTableRow);
		}
	}
	FinishGetItem();
	m_pPlayer->EndCoolTimeEventBinding.Clear();
}

void AGiveItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_pPlayer = Cast<ATPSCharacter>(OtherActor);
	if (m_pPlayer)
	{
		m_pPlayer->OpenInteractionUI(EInteractionType::Get);
		m_pPlayer->InteractionEventBinding.AddDynamic(this, &AGiveItemBox::GiveItem);
	}
}

void AGiveItemBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_pPlayer = Cast<ATPSCharacter>(OtherActor);
	if (m_pPlayer)
	{
		m_pPlayer->CloseInteractionUI();
		m_pPlayer->InteractionEventBinding.Remove(this, TEXT("GiveItem"));
		m_pPlayer = nullptr;
	}
}

// Called when the game starts or when spawned
void AGiveItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGiveItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


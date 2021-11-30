// Fill out your copyright notice in the Description page of Project Settings.


#include "ConversationNPC.h"
#include "TPSCharacter.h"
#include "InteractionType.h"
#include "Components/BoxComponent.h"

AConversationNPC::AConversationNPC():
	m_arConversationText{},
	m_nConversationTextMaxIndex{},
	m_pPlayer{},
	m_BoxComponent{}
{

	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	m_BoxComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(GetMesh()));

}

void AConversationNPC::BeginPlay()
{
	Super::BeginPlay();

	m_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AConversationNPC::OnOverlapBegin);
	m_BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AConversationNPC::OnOverlapEnd);
}

void AConversationNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AConversationNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AConversationNPC::ConversationStart()
{
	m_pPlayer->ConversationOpen(m_tNPCName,m_arConversationText[FMath::RandRange(0,m_nConversationTextMaxIndex)]);
	m_pPlayer->InteractionEventBinding.Remove(this,TEXT("ConversationStart"));
	m_pPlayer->InteractionEventBinding.AddDynamic(this, &AConversationNPC::ConversationEnd);
}

void AConversationNPC::ConversationEnd()
{
	m_pPlayer->ConversationClose();
	m_pPlayer->InteractionEventBinding.Remove(this, TEXT("ConversationEnd"));
	m_pPlayer->InteractionEventBinding.AddDynamic(this, &AConversationNPC::ConversationStart);
}

void AConversationNPC::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	m_pPlayer = Cast<ATPSCharacter>(OtherActor);
	if (m_pPlayer)
	{
		m_pPlayer->OpenInteractionUI(EInteractionType::Talk);
		m_pPlayer->InteractionEventBinding.AddDynamic(this,&AConversationNPC::ConversationStart);
	}
}

void AConversationNPC::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	m_pPlayer = Cast<ATPSCharacter>(OtherActor);
	if (m_pPlayer)
	{
		m_pPlayer->CloseInteractionUI();
		m_pPlayer->InteractionEventBinding.Clear();
		m_pPlayer = nullptr;
	}
}

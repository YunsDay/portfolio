// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicNPC.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BasicNPCNameWidget.h"

// Sets default values
ABasicNPC::ABasicNPC():
	m_WidgetComponent{},
	m_rCameraRotation{},
	m_tNPCName{}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	m_WidgetComponent->SetupAttachment(reinterpret_cast<USceneComponent*>(GetCapsuleComponent()));

}

// Called when the game starts or when spawned
void ABasicNPC::BeginPlay()
{
	Super::BeginPlay();
	Cast<UBasicNPCNameWidget>(m_WidgetComponent->GetUserWidgetObject())->Init(m_tNPCName, m_tRoleText);
}

// Called every frame
void ABasicNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_rCameraRotation =	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraRotation();
	m_WidgetComponent->SetWorldRotation(FRotator(m_rCameraRotation.Pitch * -1.0f, m_rCameraRotation.Yaw + 180.0f, m_rCameraRotation.Roll));
}

// Called to bind functionality to input
void ABasicNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


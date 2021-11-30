// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniSpiderRobot.h"

AMiniSpiderRobot::AMiniSpiderRobot():
	m_classProjectileCannon{},
	m_fCannonDamage{},
	m_fCannonRangeDamage{},
	m_fCannonRangeMagnification{}
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMiniSpiderRobot::BeginPlay()
{
	Super::BeginPlay();
}

void AMiniSpiderRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMiniSpiderRobot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMiniSpiderRobot::Attack_Implementation()
{
	if(!m_bIsDeath)
		PlayAnimMontage(m_animAttack);
}

void AMiniSpiderRobot::FireCannon()
{
	FRotator rRotator;
	rRotator = UKismetMathLibrary::MakeRotFromX(GetMesh()->GetForwardVector());
	rRotator.Yaw += 90;
	GetWorld()->SpawnActor<AProjectileBaseMobCannon>(m_classProjectileCannon, GetMesh()->GetSocketLocation(TEXT("cannon")), rRotator)->Init(m_fCannonDamage,m_fCannonRangeDamage,m_fCannonRangeMagnification,false);
}

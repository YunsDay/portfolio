// Fill out your copyright notice in the Description page of Project Settings.


#include "MySkillSystem.h"

// Sets default values for this component's properties
UMySkillSystem::UMySkillSystem():
	m_arMaxCooltime{},
	m_arCooltime{},
	m_arSkillReady{},
	m_arSkill{}
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	FS_Skill sEmptySkill{};
	PrimaryComponentTick.bCanEverTick = true;
	m_arMaxCooltime.Init(0.0f, 3);
	m_arCooltime.Init(0.0f, 3);
	m_arSkillReady.Init(true, 3);
	m_arSkill.Init(sEmptySkill,3);
	// ...
}


// Called when the game starts
void UMySkillSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMySkillSystem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SetSkillImageBinding.Clear();
}


// Called every frame
void UMySkillSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CoolTimeTick(DeltaTime,0);
	CoolTimeTick(DeltaTime, 1);
	CoolTimeTick(DeltaTime, 2);
	// ...
}

void UMySkillSystem::CoolTimeTick(float fTick, int nIndex)
{
	if (!m_arSkillReady[nIndex])
		m_arCooltime[nIndex] += fTick;

	if (m_arCooltime[nIndex] > m_arMaxCooltime[nIndex])
		m_arSkillReady[nIndex] = true;

}

void UMySkillSystem::SetSkill(int nIndex, UPARAM(ref) const FS_Skill& sSkill)
{
	m_arSkill[nIndex] = sSkill;
	m_arMaxCooltime[nIndex] = sSkill.fCoolTime;
	m_arCooltime[nIndex] = sSkill.fCoolTime;
	if (SetSkillImageBinding.IsBound())
		SetSkillImageBinding.Broadcast();
}

void UMySkillSystem::StartCoolTime(int nIndex)
{
	m_arSkillReady[nIndex] = false;
}

bool UMySkillSystem::GetSkillReady(int nIndex)
{
	return m_arSkillReady[nIndex];
}

void UMySkillSystem::GetSkillInfo(int nIndex, FS_Skill& sSkillInfo)
{
	sSkillInfo = m_arSkill[nIndex];
}

void UMySkillSystem::UseSkill(int nIndex)
{
	m_arCooltime[nIndex] = 0;
	StartCoolTime(nIndex);
}


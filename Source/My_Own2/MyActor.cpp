#include "MyActor.h"
#include "Global.h"

AMyActor::AMyActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

//FString AMyActor::ToString()
//{
//	CLog::Print(Name);
//	return;
//}

void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(5);
}

void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


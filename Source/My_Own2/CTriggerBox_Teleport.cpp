#include "CTriggerBox_Teleport.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerBox.h"
#include "Global.h"

void ACTriggerBox_Teleport::BeginPlay()
{
	Super::BeginPlay();
	FVector BoxLocation = GetActorLocation();
	// 파티클 이펙트 생성
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, BoxLocation);
	OnActorBeginOverlap.AddDynamic(this, &ACTriggerBox_Teleport::OnOverlapBegin);
}

void ACTriggerBox_Teleport::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{

	if (OtherActor && (OtherActor != this))
	{
		// TargetPoint가 설정되어 있고, 위치를 가져올 수 있다면
		if (TargetPoint)
		{
			FVector NewLocation = TargetPoint->GetActorLocation();
			OtherActor->SetActorLocation(NewLocation);
			if (Particle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, NewLocation);
			}
		}
	}
}

#include "CTriggerBox_Teleport.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerBox.h"
#include "Global.h"

void ACTriggerBox_Teleport::BeginPlay()
{
	Super::BeginPlay();
	FVector BoxLocation = GetActorLocation();
	// ��ƼŬ ����Ʈ ����
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, BoxLocation);
	OnActorBeginOverlap.AddDynamic(this, &ACTriggerBox_Teleport::OnOverlapBegin);
}

void ACTriggerBox_Teleport::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{

	if (OtherActor && (OtherActor != this))
	{
		// TargetPoint�� �����Ǿ� �ְ�, ��ġ�� ������ �� �ִٸ�
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

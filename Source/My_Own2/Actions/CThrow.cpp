#include "Actions/CThrow.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"

ACThrow::ACThrow()
{
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
//	CHelpers::CreateComponent(this, &Mesh, "Mesh");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Sphere);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	InitialLifeSpan = 1.5f;

	Projectile->InitialSpeed = 4000.0f;
	Projectile->MaxSpeed = 8000.0f;
	Projectile->ProjectileGravityScale = 0.0f;
	Projectile->SetUpdatedComponent(Sphere);

	//UStaticMesh* mesh;
	//CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Meshes/M_Sphere.M_Sphere'");
	//Mesh->SetStaticMesh(mesh);

	//UMaterialInstanceConstant* material;
	//CHelpers::GetAsset<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/MA_Bullet_Inst.MA_Bullet_Inst'");
	//Mesh->SetMaterial(0, material);
	//Mesh->SetRelativeScale3D(FVector(1.0f, 0.025f, 0.025f));
	//Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
}

void ACThrow::BeginPlay()
{
	Super::BeginPlay();

	TArray<USphereComponent*> components;
	GetComponents<USphereComponent>(components);

	for (USphereComponent* sphere : components)
	{
		sphere->OnComponentBeginOverlap.AddDynamic(this, &ACThrow::OnComponentBeginOverlap);
	//	sphere->bHiddenInGame = false;
	}
}



void ACThrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!!Explosion)
	{
		FTransform transform = ExplosionTransform;
		transform.AddToTranslation(SweepResult.Location);
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, transform);
	}

	if (OnThrowBeginOverlap.IsBound())
		OnThrowBeginOverlap.Broadcast(SweepResult);

	if (OtherActor != GetOwner())
	{
		// Delete the actor
		Destroy();
	}
}




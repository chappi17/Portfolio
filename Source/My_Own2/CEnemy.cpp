#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Name.h"
#include "Widgets/CUserWidget_Health.h"
#include "Widgets/CUserWidget_Damage_Text.h"
#include "CAIController.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

	CHelpers::CreateComponent<UWidgetComponent>(this, &NameWidget, "NameWidget", GetMesh());
	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());
	CHelpers::CreateComponent<UWidgetComponent>(this, &DamageTextWidget, "DamageTextWidget", GetMesh());
	
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Goblin/Mesh/Goblin_Base/SK_Goblin_Red.SK_Goblin_Red'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemy/ABP_CEnemy.ABP_CEnemy_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	TSubclassOf<UCUserWidget_Name> nameClass;
	CHelpers::GetClass<UCUserWidget_Name>(&nameClass, "WidgetBlueprint'/Game/WIdget/WB_Name.WB_Name_C'");
	NameWidget->SetWidgetClass(nameClass);
	NameWidget->bHiddenInGame = true; // 나중에 타겟 카메라 만들었을때 대상 선택되면 켜지게
	NameWidget->SetRelativeLocation(FVector(0, 0, 240));
	NameWidget->SetDrawSize(FVector2D(240, 30));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCUserWidget_Health> healthClass;
	CHelpers::GetClass<UCUserWidget_Health>(&healthClass, "WidgetBlueprint'/Game/WIdget/WB_Health.WB_Health_C'");
	HealthWidget->SetWidgetClass(healthClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidget->SetDrawSize(FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCUserWidget_Damage_Text> DamageClass;
	CHelpers::GetClass<UCUserWidget_Damage_Text>(&DamageClass, "WidgetBlueprint'/Game/WIdget/WB_DamageText.WB_DamageText_C'");
	DamageTextWidget->SetWidgetClass(DamageClass);
	DamageTextWidget->SetRelativeLocation(FVector(0, 0, 190));
	DamageTextWidget->SetDrawSize(FVector2D(120, 20));
	DamageTextWidget->SetWidgetSpace(EWidgetSpace::Screen);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	State->SetHittedMode();

	float currentHealth = Status->GetHealth();

	HealthWidget->SetVisibility(true);
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(currentHealth, Status->GetMaxHealth());

	if (currentHealth <= 0)
	{
		Status->SetStop();
		// 여기에 죽는 로직을 추가할 수 있습니다.
		// 예: State->SetDeadMode();
	}
	else
	{
		if (EventInstigator)
		{
			AActor* CombatTarget = EventInstigator->GetPawn();
			MoveToTarget(CombatTarget);
		}
	}

	return currentHealth;
}

void ACEnemy::BeginPlay()
{
	UMaterialInstanceConstant* body;
//	UMaterialInstanceConstant* logo;



	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Goblin/Material/Goblin_Base/Mat_Goblin_red_Inst.Mat_Goblin_red_Inst'");
	//CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_ChestLogo_Inst.M_UE4Man_ChestLogo_Inst'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	//LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);


	GetMesh()->SetMaterial(0, BodyMaterial);
	//GetMesh()->SetMaterial(1, LogoMaterial);


	Super::BeginPlay();
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	NameWidget->InitWidget();
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(GetActorLabel());

	HealthWidget->InitWidget();
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
	if (HealthWidget)
	{
		HealthWidget->SetVisibility(false); // Hide initially
	}
	
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);

	
	Action->SetAIMode();
}

bool ACEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return nullptr;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void ACEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr || Status->GetHealth() < 0) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.0f);
	EnemyController->MoveTo(MoveRequest);
}

void ACEnemy::MoveToLocation(FVector TargetLocation)
{
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(TargetLocation);
	MoveRequest.SetAcceptanceRadius(15.0f);

	EnemyController->MoveTo(MoveRequest);
}

AActor* ACEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = PatrolTargets.Num();
	if (NumPatrolTargets > 0)
		{
			const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
			return PatrolTargets[TargetSelection];
		}
	return nullptr;
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPatrolTarget();
}

void ACEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(Waitmin, Waitmax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &ACEnemy::PatrolTimerFinished, WaitTime);
		//MoveToTarget(PatrolTarget);
	}
}


void ACEnemy::Hitted()
{
	// 데미지 전달
	Status->SubHealth(DamageValue);
	//CLog::Print(DamageValue);

//	CLog::Log(DamageValue);
	DamageTextWidget->InitWidget();
	UCUserWidget_Damage_Text* DamageTextObj = Cast<UCUserWidget_Damage_Text>(DamageTextWidget->GetUserWidgetObject());

	if (DamageTextObj)
	{
	//	CLog::Log("Casting Succeeded");
		DamageTextObj->DamageText(DamageValue);
	}
	else
	{
	//	CLog::Log("Casting Failed");
	}


	DamageValue = 0.0f;
	// 몽타주 실행
	Status->SetStop();
	// 죽는거 처리
	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		Status->SetStop();
		GetCharacterMovement()->StopMovementImmediately(); // Stop movement immediately
		return;
	}


	Montages->PlayHitted();
	// 맞은쪽으로 회전

	if (!State->IsDeadMode())
	{
		if (DamageInstigator)
		{
			FVector start = GetActorLocation();
			FVector target = DamageInstigator->GetPawn()->GetActorLocation();

			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
			DamageInstigator = NULL;
			// 뒤로 살짝 미는거 
			FVector direction = target - start;
			direction.Normalize();
			LaunchCharacter(-direction * LaunchAmount, true, false);
		}
	}
}

void ACEnemy::Dead()
{
	CheckFalse(State->IsDeadMode());

	Status->SetStop();
	Montages->PlayDead();

}

void ACEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void ACEnemy::Begin_Dead()
{
	Action->OffAllCollision();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACEnemy::End_Dead()
{
	Destroy();
	Action->DestroyComponent();
}

void ACEnemy::ChangeColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
//	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}

void ACEnemy::RestoreColor()
{
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Hitted: Hitted(); break;
		case EStateType::Dead: Dead(); break;
	}
}


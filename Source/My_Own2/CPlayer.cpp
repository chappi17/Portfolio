#include "CPlayer.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CustomMovementComponent.h"
//#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CFeetComponent.h"
#include "Components/CTargetComponent.h"
#include "Widgets/CUserWidget_ActionItemList.h"
#include "Widgets/CUserWidget_PlayerHUD.h"
#include "GameFramework/PlayerController.h"


ACPlayer::ACPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	CHelpers::CreateActorComponent<UCOptionComponent>(this, &Option, "Option");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State,"State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCFeetComponent>(this, &Feet, "Feet");
	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action");
	CHelpers::CreateActorComponent<UCTargetComponent>(this, &Target, "Target");

	CHelpers::CreateComponent<UWidgetComponent>(this, &PlayerHealthWidget, "HealthWidget", GetMesh());
	

	//CustomMovementComponent = CreateDefaultSubobject<UCustomMovementComponent>(TEXT("CustomMovementComponent"));

		CustomMovementComponent = Cast<UCustomMovementComponent>(GetCharacterMovement());

	USkeletalMesh* mesh;
	//	CLog::Print("Error");
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	SpringArm->SetRelativeLocation(FVector(0, 0, 160));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	//SpringArm->TargetArmLength = 400;
	SpringArm->bDoCollisionTest = false; // 카메라의 충돌을 허용할 것이냐 
	SpringArm->bUsePawnControlRotation = true; // 폰의 회전에 같이 회전할 것이냐
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->SocketOffset = FVector(0, 0, 0);
	SpringArm->bEnableCameraLag = true;

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);


	CustomMovementComponent->RotationRate = FRotator(0, 720, 0);
	bUseControllerRotationYaw = false;
	CustomMovementComponent->bOrientRotationToMovement = true;
	CustomMovementComponent->MaxWalkSpeed = 600;
	CustomMovementComponent->JumpZVelocity = 1000;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->bHiddenInGame = true;

	CHelpers::GetClass<UCUserWidget_ActionItemList>(&ActionListClass, "WidgetBlueprint'/Game/WIdget/WB_ActionItemList.WB_ActionItemList_C'");

	TSubclassOf<UCUserWidget_PlayerHUD> healthClass;
	CHelpers::GetClass<UCUserWidget_PlayerHUD>(&healthClass, "WidgetBlueprint'/Game/WIdget/WB_PlayerHUD.WB_PlayerHUD_C'");

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		HealthWidget = CreateWidget<UCUserWidget_PlayerHUD>(PlayerController, healthClass);

		// 뷰포트에 추가
		if (HealthWidget)
		{
			HealthWidget->AddToViewport();
		}
	}
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_ChestLogo_Inst.M_UE4Man_ChestLogo_Inst'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	Action->SetUnarmedMode();

	ActionList = CreateWidget<UCUserWidget_ActionItemList, APlayerController>(GetController<APlayerController>(), ActionListClass);
	ActionList->AddToViewport();
	ActionList->SetVisibility(ESlateVisibility::Hidden);

	ActionList->GetData(0).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnOneHand);
	ActionList->GetData(1).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnTwoHand);
	ActionList->GetData(2).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnGun);
	ActionList->GetData(3).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnShotGun);


	int health = Status->GetHealth();

	CLog::Print(health);
	int maxhealth = Status->GetMaxHealth();
	CLog::Print(maxhealth);
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
//	GetCapsuleComponent()->bHiddenInGame = false;
//	CLog::Print(GetCapsuleComponent()->bHiddenInGame);
	int health = Status->GetHealth();
	int maxhealth = Status->GetMaxHealth();
	HealthWidget->UpdateHealthProgressBar(health, maxhealth);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("Zoom",this,&ACPlayer::CameraZoom);


	PlayerInputComponent->BindAction("Walk",EInputEvent::IE_Pressed,this, &ACPlayer::OnWalk);
	PlayerInputComponent->BindAction("Walk",EInputEvent::IE_Released,this, &ACPlayer::OffWalk);

	PlayerInputComponent->BindAction("Targeting",EInputEvent::IE_Pressed,this, &ACPlayer::OnTarget);
	PlayerInputComponent->BindAction("TargetLeft",EInputEvent::IE_Pressed,this, &ACPlayer::OnTargetLeft);
	PlayerInputComponent->BindAction("TargetRight",EInputEvent::IE_Pressed,this, &ACPlayer::OnTargetRight);

	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::Avoid);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);
//	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Pressed, this, &ACPlayer::Dash);
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnOneHand);
	PlayerInputComponent->BindAction("TwoHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnTwoHand);
	PlayerInputComponent->BindAction("Gun", EInputEvent::IE_Pressed, this, &ACPlayer::OnGun);
	PlayerInputComponent->BindAction("ShotGun", EInputEvent::IE_Pressed, this, &ACPlayer::OnShotGun);

	PlayerInputComponent->BindAction("Climb", EInputEvent::IE_Pressed, this, &ACPlayer::OnClimb);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);
	

	PlayerInputComponent->BindAction("ActionItemList", EInputEvent::IE_Pressed, this, &ACPlayer::OnViewActionList);
	PlayerInputComponent->BindAction("ActionItemList", EInputEvent::IE_Released, this, &ACPlayer::OffViewActionList);
}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamId);
}

void ACPlayer::SubHealth()
{
	MinusHealth();
}

void ACPlayer::OnViewActionList()
{
	CheckFalse(State->IsIdleMode());

	ActionList->SetVisibility(ESlateVisibility::Visible);

	GetController<APlayerController>()->bShowMouseCursor = true;
	GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
}

void ACPlayer::OffViewActionList()
{
	ActionList->SetVisibility(ESlateVisibility::Hidden);
	GetController<APlayerController>()->bShowMouseCursor = false;
	GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

}

void ACPlayer::OnMoveForward(float InAxis)
{
	CheckFalse(Status->CanMove());

	if (Status->CanMove())
	{

		if (!CustomMovementComponent)
		{
		//	CLog::Print("Can't Walk");
			return;
		}
		if (CustomMovementComponent->IsClimbing())
		{
			HandleClimbMovementFowardInput(InAxis);
			//CLog::Print("ClimbMode");
		}
		else
		{
			FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
			FVector direction = FQuat(rotator).GetForwardVector().GetSafeNormal2D();

			AddMovementInput(direction, InAxis);
		}
	}
	else
		Status->SetStop();
		return;
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->CanMove());

	if (Status->CanMove())
	{
		if (!CustomMovementComponent)
			return;

		if (CustomMovementComponent->IsClimbing())
		{
			HandleClimbMovementRightInput(InAxis);
		}

		FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
		FVector direction = FQuat(rotator).GetRightVector().GetSafeNormal2D(); // Use GetForwardVector() instead of GetRightVector()


	//	float leftAxisValue = FMath::Max(InAxis, 0.0f); // Ensure positive value for left movement

		AddMovementInput(direction, InAxis);
	}
	else
		Status->SetStop();
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::Avoid()
{
	CheckFalse(State->IsIdleMode());

	if (Action->IsUnarmedMode())
	{
		Jump();
	}
	else
	{
		if (InputComponent->GetAxisValue("MoveForward") < 0.0f)
		{
			State->SetBackstepMode();
			return;
		}
		State->SetRollMode();

	}
}

void ACPlayer::Dash()
{
	const FVector FowardDir = this->GetActorRotation().Vector();
	LaunchCharacter(FowardDir * DashDistance, true, true);

	MinusHealth();

}

void ACPlayer::MinusHealth()
{
		Status->SubHealth(10);
		int current = Status->GetHealth();

		CLog::Print(current);

		if (Status->GetHealth() <= 0.0f)
		{
			State->SetDeadMode();
			CLog::Print("Dead");
			Dead();
			return;
		}
}

void ACPlayer::GetHitted()
{
	Hitted();
}

void ACPlayer::OnWalk()
{
	CustomMovementComponent->MaxWalkSpeed = 400.0f;
}

void ACPlayer::OffWalk()
{
	CustomMovementComponent->MaxWalkSpeed = 600.0f;
}


//void ACPlayer::GetplayerHealth()
//{
//	Status->GetHealth();
//}
//
//void ACPlayer::GetplayerMaxHealth()
//{
//	Status->GetMaxHealth();
//}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw =false;
	CustomMovementComponent->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector from = start + GetVelocity().GetSafeNormal2D();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, from));

	Montages->PlayRoll();
	UWorld* World = GetWorld();
	if (World)
	{
		AWorldSettings* WorldSetting = World->GetWorldSettings();
		WorldSetting->SetTimeDilation(0.5f);
	}
}

void ACPlayer::Begin_Backstep()
{
	bUseControllerRotationYaw = true;
	CustomMovementComponent->bOrientRotationToMovement = false;

	Montages->PlayBackstep();
}

void ACPlayer::End_Roll()
{
	bUseControllerRotationYaw = true;
	CustomMovementComponent->bOrientRotationToMovement = false;
	State->SetIdleMode();
	UWorld* World = GetWorld();
	if (World)
	{
		AWorldSettings* WorldSetting = World->GetWorldSettings();
		WorldSetting->SetTimeDilation(1.0f);
	}
}

void ACPlayer::End_Backstep()
{
	bUseControllerRotationYaw = true;
	CustomMovementComponent->bOrientRotationToMovement = false;
	State->SetIdleMode();
}

void ACPlayer::CameraZoom(float InAxis)
{
	if (!!InAxis)
	{
		CLog::Log("Zoom");
		float ZoomSpeed = 20.0f;
		float Zoom = ZoomSpeed * InAxis;
		float clampLength = FMath::Clamp(SpringArm->TargetArmLength, 200.0f, 600.0f);
		SpringArm->TargetArmLength = clampLength + Zoom;
	}
}

void ACPlayer::HandleClimbMovementFowardInput(float InAxis)
{
	const FVector UpDirection = GetActorUpVector();
	AddMovementInput(UpDirection, InAxis);
}

void ACPlayer::HandleClimbMovementRightInput(float InAxis)
{
	//const FVector RightDirection = FVector::CrossProduct(
	//	-CustomMovementComponent->GetClimbableSurfaceNormal(),
	//	-GetActorUpVector()
	//);

	const FVector RightDirection = GetActorRightVector();
	AddMovementInput(RightDirection, InAxis);
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Roll: Begin_Roll();  break;
		case EStateType::Backstep: Begin_Backstep(); break;
	}
}

void ACPlayer::OnFist()
{
	CheckFalse(State->IsIdleMode());
	Action->SetFistMode();
}

void ACPlayer::OnOneHand()
{
	CheckFalse(State->IsIdleMode());
	Action->SetOneHandMode();
}

void ACPlayer::OnTwoHand()
{
	CheckFalse(State->IsIdleMode())
		Action->SetTwoHandMode();
}

void ACPlayer::OnGun()
{
	CheckFalse(State->IsIdleMode());
	Action->SetGunMode();
}

void ACPlayer::OnShotGun()
{
	CheckFalse(State->IsIdleMode());
	Action->SetShotGunMode();
}

void ACPlayer::OnDoAction()
{
	Action->DoAction();
//	CLog::Print("DoAction");
}

void ACPlayer::OnClimb()
{
//CLog::Print("Climbing Mode");
	if (!CustomMovementComponent) return;

	if (!CustomMovementComponent->IsClimbing())
	{
		CustomMovementComponent->ToggleClimbing(true);
		
	}
	else
	{
		CustomMovementComponent->ToggleClimbing(false);
	}
}

void ACPlayer::OnTarget()
{
	Target->ToggleTarget();
}

void ACPlayer::OnTargetLeft()
{
	Target->ChangeTargetLeft();
}

void ACPlayer::OnTargetRight()
{
	Target->ChangeTargetRight();
}

void ACPlayer::OnAim()
{
	Action->DoAim();
}

void ACPlayer::OffAim()
{
	Action->UndoAim();
}

void ACPlayer::Hitted()
{
	// 데미지 전달
	//CLog::Print(DamageValue);
	Montages->PlayHitted();

	// 몽타주 실행
	// 죽는거 처리
	MinusHealth();

}

void ACPlayer::Dead()
{
	// 캐릭터를 래그돌 상태로 설정
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	// 캐릭터의 다른 컴포넌트를 비활성화 (옵션)
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACPlayer::ChangeColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}
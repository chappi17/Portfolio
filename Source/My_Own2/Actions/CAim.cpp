#include "Actions/CAim.h"
#include "Global.h"
#include "CHUD.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCAim::UCAim()
{
	CHelpers::GetAssetDynamic<UCurveFloat>(&Curve, "CurveFloat'/Game/Actions/CV_Zoom.CV_Zoom'");
}

void UCAim::BeginPlay(ACharacter* InCharacter)
{
	OwnerCharacter = InCharacter;
	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Action = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);

	HUD = OwnerCharacter->GetWorld()->GetFirstPlayerController()->GetHUD<ACHUD>();

	TimelineFloat.BindUFunction(this, "Zooming");

	Timeline.AddInterpFloat(Curve, TimelineFloat);
	Timeline.SetPlayRate(200);
}

void  UCAim::OnZoom()
{
	if (!IsAvaliable() || bInZoom) return;

	if (HUD == nullptr || Action == nullptr || SpringArm == nullptr) {
		// 로그를 남기거나 적절히 대응
		CLog::Print("Error!");
		return;
	}

	bInZoom = true;
	HUD->SetDrawMode();
	Timeline.PlayFromStart();

	if (Action->IsGunMode() || Action->IsShotGunMode())
	{
		SpringArm->TargetArmLength = 100.0f;
		SpringArm->SocketOffset = Action->IsGunMode() ? FVector(0, 30, 0) : FVector(0, 50, 0);
		SpringArm->bEnableCameraLag = false;
	}

}

void  UCAim::OffZoom()
{
	if (!IsAvaliable() || !bInZoom) return;

	if (HUD == nullptr || SpringArm == nullptr) {
		CLog::Print("Error_In_SpringArm");
		return;
	}

	bInZoom = false;
	HUD->SetUnDrawMode();
	Timeline.ReverseFromEnd();

	SpringArm->SetRelativeLocation(FVector(0, 0, 160));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->SocketOffset = FVector(0, 0, 0);
	SpringArm->bEnableCameraLag = true;
}

void  UCAim::Tick(float DeltaTime)
{
	Timeline.TickTimeline(DeltaTime);
}

void  UCAim::Zooming(float Output)
{
	Camera->FieldOfView = Output;
}

#include "CHUD.h"
#include "Global.h"
#include "Engine/Texture2D.h"
#include "Engine/Canvas.h"

ACHUD::ACHUD()
{
	CHelpers::GetAsset<UTexture2D>(&Texture, "Texture2D'/Game/WIdget/T_Dot.T_Dot'");
}

void ACHUD::DrawHUD()
{
	Super::DrawHUD();

	CheckFalse(bDraw);
	CheckNull(Texture);

	FVector2D center(Canvas->ClipX * 0.47f, Canvas->ClipY * 0.54f);
	FVector2D size(Texture->GetSurfaceWidth() * 0.015f, Texture->GetSurfaceHeight() * 0.015f);
	FVector2D position = center - size;


	FCanvasTileItem item(position, Texture->Resource, FLinearColor::White);
	item.BlendMode = SE_BLEND_Translucent; // 일정 투명값 아래는 아예 안그림

	Canvas->DrawItem(item);
}

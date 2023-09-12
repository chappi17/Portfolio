#pragma once
#include "CoreMinimal.h"
 // 상속 받을 애가 없어서, 걍 CoreMinimal 만 있음 
#define PrintLine(){ CLog::Log(__FUNCTION__, __LINE__); }

class MY_OWN2_API CLog
{
public:
	static void Print(int32 InValue, int32 InKey = -1, float Duration = 10.0f, FColor InColor = FColor::Blue); // 플레이 화면에 나옴
	static void Print(float InValue, int32 InKey = -1, float Duration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(const FString& InValue, int32 InKey = -1, float Duration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(const FVector& InValue, int32 InKey = -1, float Duration = 10.0f, FColor InColor = FColor::Blue);
	static void Print(const FRotator& InValue, int32 InKey = -1, float Duration = 10.0f, FColor InColor = FColor::Blue);

	static void Log(int32 InValue);
	static void Log(float InValue);
	static void Log(const FString& InValue);
	static void Log(const FVector& InValue);
	static void Log(const FRotator& InValue);
	static void Log(const UObject* InObject);
	static void Log(const FString& InFuncName, int32 InLineNumber);
};

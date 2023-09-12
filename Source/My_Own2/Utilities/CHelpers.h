#pragma once
// 자주 쓰는 애들은 글로벌 헤더에 넣어놓고 만들어 놓음


#include "CoreMinimal.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

class MY_OWN2_API CHelpers
{
public:

template<typename T>
   static void GetAsset(T** OutObject, FString InPath)
   {
      ConstructorHelpers::FObjectFinder<T> asset(*InPath); // 정적
      verifyf(asset.Succeeded(), L"asset.Succeeded()");

      *OutObject = asset.Object;
   }

template<typename T>
   static void GetAssetDynamic(T** OutObject, FString InPath)
   {
       T* obj = Cast<T>(StaticLoadObject(T::StaticClass(), NULL, *InPath));
       verifyf(!!obj, L"!!asset"); // null 이면 false, 

       *OutObject = obj;
   }

   template<typename T>
   static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
   {
       ConstructorHelpers::FClassFinder<T> asset(*InPath);
       verifyf(asset.Succeeded(), L"asset.Succeeded()");

       *OutClass = asset.Class;
   }

   template<typename T>
   static void CreateComponent(AActor* InActor, T** InComponent, FName InName, USceneComponent* InParent = NULL)
   {
       *InComponent = InActor->CreateDefaultSubobject<T>(InName);

       if (!!InParent)
       {
           (*InComponent)->SetupAttachment(InParent);

           return;
       }

       InActor->SetRootComponent((*InComponent));
   }

   template<typename T>
   static void CreateActorComponent(AActor* InActor, T** InComponent, FName InName)
   {
       *InComponent = InActor->CreateDefaultSubobject<T>(InName);
   }

    template<typename T>
   static void FindActors(class UWorld* InWorld, TArray<T*>& OutActors)
   {
       OutActors.Empty();

       TArray<AActor*> actors;
       UGameplayStatics::GetAllActorsOfClass(InWorld, T::StaticClass(), actors);

       for (AActor* actor : actors)
           OutActors.Add(Cast<T>(actor));
   }

   template<typename T>
   static T* GetComponent(AActor* InActor)
   {
       return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
   }
};

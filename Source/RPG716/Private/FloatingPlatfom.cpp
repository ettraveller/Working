// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatfom.h"
#include <Components/StaticMeshComponent.h>
#include "TimerManager.h"

// Sets default values
AFloatingPlatfom::AFloatingPlatfom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	StartPoint = FVector::ZeroVector;
	EndPoint = FVector::ZeroVector;

	InterporSpeed = 4.0f;
	bInterping = false;
	InterporTime = 1.0f;
}

// Called when the game starts or when spawned
void AFloatingPlatfom::BeginPlay()
{
	Super::BeginPlay();
	
    StartPoint = GetActorLocation();
	EndPoint += StartPoint;
	
	GetWorldTimerManager().SetTimer(InteporTimer, this, &AFloatingPlatfom::ToggleInterping, InterporTime);

	Distance = (EndPoint - StartPoint).Size();
}
  
// Called every frame
void AFloatingPlatfom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// floor �̵��ϴ°Ϳ� ���� �Լ�
	if (bInterping)
	{
		FVector CurrentLocattion = GetActorLocation();
		//interpellation�� �ϴ� ���� �� �� ���̸� �Ų����� �̵��ϱ� ���� ������ ����Դϴ�.
		FVector Interpor = FMath::VInterpTo(CurrentLocattion, EndPoint, DeltaTime, InterporSpeed);
		SetActorLocation(Interpor);

		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();
		if (Distance - DistanceTraveled <= 1.f)
		{
		     ToggleInterping();

			 GetWorldTimerManager().SetTimer(InteporTimer, this, &AFloatingPlatfom::ToggleInterping, InterporTime);
			 SwapVectors(StartPoint, EndPoint);
		}
	}
	
}

void AFloatingPlatfom::ToggleInterping()
{
    bInterping = !bInterping;
}

// ó����ġ�� ���ư��� �ʿ��� ���� �Լ�
void AFloatingPlatfom::SwapVectors(FVector& Vec1, FVector& Vec2)
{
    FVector Temp = Vec1;
	Vec1 = Vec2;
	Vec2 = Temp;
} 


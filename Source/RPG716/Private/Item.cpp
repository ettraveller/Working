// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include <Components/StaticMeshComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"



// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// item �� �������� �ൿ �Լ� ����
	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollsionVolume"));
	RootComponent = CollisionVolume;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	IdleParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleSystemComp"));
	IdleParticleComp->SetupAttachment(GetRootComponent());

	bRotate = false;
	RotationRate = 45.f;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bRotate)
	{
		// item ���� �����ϵ��� ȸ���ϰ� �����
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);

	}

}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //UE_LOG(LogTemp, Warning, TEXT("Super::Item Overlap Begin!"));

	

}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Super::Item Overlap End!"));
}


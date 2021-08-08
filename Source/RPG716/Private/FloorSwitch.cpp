// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include "TimerManager.h"


// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��ü�� �ν��� �浹ü Trigger ���� 
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;
	//collision ���� (�ٸ��� �����ϰ� pawn�� ������)
	//collision preset �����ϴ°�
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	TriggerBox->SetBoxExtent(FVector(62.f, 62.f, 32.f));

	// �ٴ� mesh ����
	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
	FloorSwitch->SetupAttachment(GetRootComponent());
	
	// �� mesh ����
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());

	SwitchTime = 1.f;
	bCharacterOnSwitch = false;
}

void AFloorSwitch::CloseDoor()
{
    if(!bCharacterOnSwitch)
	{
	    LowerDoor();
	    RaiseFloorSwitch();
	}
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();

	//AddDynamic = �Լ��� �������̺�Ʈ�� ���ε��ϴ°�
	//��� ��������Ʈ�� ����Ͽ� ������
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);

	InitialDoorLocation = Door->GetComponentLocation();
	InitialSwitchLocation = FloorSwitch->GetComponentLocation();

}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// trigger �� ������� ���� �Լ�
void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("Begin OverLap"));
	if(!bCharacterOnSwitch) bCharacterOnSwitch = true;
	RaiseDoor();
	LowerFloorSwitch();
}

// trigger �� ��Ҵٰ� �ȴ�� �Ǹ� ���� �Լ�
void AFloorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("End OverLap"));
	if(bCharacterOnSwitch) bCharacterOnSwitch = false;

	// timer �� �������Լ��� �����Ͽ�, ĳ���Ͱ� ��� floor ���� ����� ���� �����ְ� �ϱ� ���ؼ�
	GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchTime);

}

// ���������� ���� ��ġ �� �����ϰ� �ϴ°�
void AFloorSwitch::UpdateDoorLocation(float Z)
{
    FVector NewLocation = InitialDoorLocation;
	NewLocation.Z += Z;
	Door-> SetWorldLocation(NewLocation);
}

// ���������� floor �� ��ġ�� �����ϰ� �ϴ°�
void AFloorSwitch::UpdateFloorSwitchLocation(float Z)
{
	FVector NewLocation = InitialSwitchLocation;
	NewLocation.Z += Z;
	FloorSwitch->SetWorldLocation(NewLocation);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include <Components/SphereComponent.h>
#include <AIController.h>
#include "MainCharacter.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/SkeletalMeshSocket.h>
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerController.h"
#include <GameFramework/Character.h>


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(700.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(100.f);
	
	// ���� �ȿ� (Ư������) �� collision ��ġ��
	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetMesh(), FName("EnemySocket"));
    //������Ʈ�� �ٸ� ������Ʈ�� ���Ͽ� ������ ���
	//�ٵ� ���� ����? �����𸣰ڴ�..
	//CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("BearSocket"));

	bOverlappingCombatSphere = false;

	Health = 75.f;
	MaxHealth = 100.f;
	Damage = 30.f;

	AttackMinTime = 0.5f;
	AttackMaxTime = 3.5f;

	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;

	DeathDelay = 3.f;

	bHasValidTarget = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);
	//AgroSphere �����϶��� ���߹��� ��Ƶ� ������ �ʰ� �ϱ�
	AgroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//Mesh �� ĸ��������Ʈ�� ī�޶�� �浹���� �ϱ�
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// ���� ������ ������
void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(OtherActor && Alive())
	{

	    AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			MoveToTarget(Main);
		}
	}
    
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ���ݻ��°� �ƴҽ� �Ϲ� ���� ������ ����� �����Ϸ� ���ٰ� ���� �ݰ� �̻��̸� ������ ����
	if (OtherActor)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			bHasValidTarget = false;
			if (Main->CombatTarget == this)
			{
				Main->SetCombatTarget(nullptr);
			}
			Main->setHasCombatTarget(false);

			Main->UpdateCombatTarget();
			
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		// ���Ͱ� �����ϰ� ����� (SpiderAnim_BP �� Attacking)
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		{
		if (Main)
		{
			bHasValidTarget = true;
			Main->SetCombatTarget(this);
			Main->setHasCombatTarget(true);
			
			Main->UpdateCombatTarget();

		    CombatTarget = Main;
		    bOverlappingCombatSphere = true;
			
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
			
			//Attack();
		    //SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
			//attacking animation������ �������Ʈ����
			
		}
		}
	}
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ������ �ݰ濡�� �־����� ���Ͱ� ĳ���� �i�ƿ��� �ϱ�
	if (OtherActor && OtherComp)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			/*SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
			MoveToTarget(Main);*/
			// �������������� ���� Ȯ��
			
			bOverlappingCombatSphere = false;
			MoveToTarget(Main);
			CombatTarget = nullptr;

			if (Main->CombatTarget == this)
			{
				Main->SetCombatTarget(nullptr);
				Main->bHasCombatTarget = false;
				Main->UpdateCombatTarget();

			}

			// ���� ������ healthbar ���ֱ�
			if (Main->MainPlayerController)
			{
				USkeletalMeshComponent* MainMesh = Cast<USkeletalMeshComponent>(OtherComp);
				if (MainMesh)
				{
					Main->MainPlayerController->RemoveEnemyHealthBar();
				}
			}

			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
	}
}

//cs���Ͽ� AIModule �߰��ؾߴ� target ���󰡰� �����
void AEnemy::MoveToTarget(class AMainCharacter* Target)
{
    SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
		//UE_LOG(LogTemp, Warning, TEXT("MoveToTarget()"));
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		// �浹 �ݰ� ������
		MoveRequest.SetAcceptanceRadius(30.0f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

		//ĳ���� �̵���ο� ����Ʈ�� �� ������ �����Ͽ� �̵��ϱ⶧���� ���Ͱ� �𼭸��κ��� �ε巴�� �����ִ�.
		// 
		//  CRASH!!!  NavMesh�� ������ �Ѿ�� collision�̺�Ʈ�� �߻��ǰ� pathpoint �� ã����׷��� ����Ʈ������ ����������.
		// �ذ�
		// 
		//TArray<FNavPathPoint> PathPoint = NavPath->GetPathPoints();

		/*auto PathPoint = NavPath->GetPathPoints();
		for (auto Point : PathPoint)
		{
			FVector Location = Point.Location;
			UKismetSystemLibrary::DrawDebugSphere(this, Location ,25.f, 8, FLinearColor::Blue, 10.f, 0.5f);
		}*/

	}
}
void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			if (Main->HitParticle)
			{
				const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
				if (TipSocket)
				{
					FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticle, SocketLocation, FRotator::ZeroRotator, false);
				}

			}
			if (Main->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, Main->HitSound);
			}
			if (DamageTypeClass)
			{
				// ���� �ϸ� ĳ���Ͱ� �� ��� �ϱ�
				UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
			}
		}

	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    
}

void AEnemy::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UGameplayStatics::PlaySound2D(this, SwingSound);

}

void AEnemy::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
	if (Alive() && bHasValidTarget)
	{
		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
		if (!bAttacking)
		{
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.2f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);

			}
	
		} 
	}
    
}

void AEnemy::AttackEnd()
{
    bAttacking = false;
	if (bOverlappingCombatSphere)
	{
		// ���� ���ݽð��� ���������ν�, �����ð����� ������ ���� -> �������ų� ������ �Ҽ��ְ�
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
		//Attack();
	}
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.f) {
		Health -= DamageAmount;
		Die(DamageCauser);

	}
	else {
		Health -= DamageAmount;
	}

	return DamageAmount;

}

void AEnemy::Die(AActor* Causer)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.2f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);

	}
	

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bAttacking = false;

	AMainCharacter* Main = Cast<AMainCharacter>(Causer);
	if (Main)
	{
		Main->UpdateCombatTarget();
	}

}

void AEnemy::DeathEnd()
{
	// ������ �ִϸ��̼� �� ���̷�Ż ������Ʈ ����
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}

bool AEnemy::Alive()
{
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void AEnemy::Disappear()
{
	Destroy();
}

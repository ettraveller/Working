 // Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include <Components/SkeletalMeshComponent.h>
#include "MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include <Sound/SoundCue.h>
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include <Components/BoxComponent.h>
#include "Enemy.h"
#include "MainPlayerController.h"


AWeapon::AWeapon()
{
    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMesh->SetupAttachment(GetRootComponent());

    CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
    CombatCollision->SetupAttachment(GetRootComponent());

    //�ݸ��� ����, �ٸ������� �ٸ��� eccpawn�� ����������
    CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


    bWeaponParticle = false;

    WeaponState = EWeaponState::EWS_Pickup;

    Damage = 25.f;

    bItemEquipDown = false;

 
}




void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
    CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);
    
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    //WeaponState = EWeaponState::EWS_Pickup;

    if ((WeaponState == EWeaponState::EWS_Pickup) && OtherActor)
    {
        AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
        if (Main)
        {
            Main->setWeapon(this);
            if (Main->CallItemEquip(false, this)) {
               
                bItemEquipDown = true;
                
                //Main->SetActiveOverlappingItem(this);
            }
            else {
                return;
            }
            /*UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin Euqip!!"));
            bItemEquipDown = true;
            Main->CallItemEquip();*/
            
            //Equip(Main);
            //Main->SetActiveOverlappingItem(this);
            
            

           // Main->AddItem(this);

            /*Main->AddToInventory(this);

            GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, *ThumbnailName);*/
        }
    }
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

    //WeaponState = EWeaponState::EWS_Equiped;

	if (OtherActor)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			//Equip(Main);
			//Main->SetActiveOverlappingItem(nullptr);
            Main->UnCallItemEquip();
		}
	}
}

// skeletal ���Ͱ� �ִ� ���������� ĳ���Ϳ� ������ -> ĳ���� ������ �̸��� ������
void AWeapon::Equip(class AMainCharacter* Char)
{
    if (Char)
    {

        SetInstigator(Char->GetController());

        //���� ī�޶�� �浹���ü���
        SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
        SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
        SkeletalMesh->SetSimulatePhysics(false);

        const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");
        if ((WeaponState == EWeaponState::EWS_Pickup) && RightHandSocket)
        {
            RightHandSocket->AttachActor(this, Char->GetMesh());
            bRotate = false;
            //char�� equipweapon�� null �̿��µ� ����ҷ��ؼ� ũ������
            // ��κ�ũ������ �߸��������ͻ������ ���س�
            //Char->GetEquipWeapon()->Destroy();
            Char->SetEquipWeapon(this);
            Char->SetActiveOverlappingItem(nullptr);
            UE_LOG(LogTemp, Warning, TEXT("PassEquip"));
        }
        if (OnEquipSound)
        {
            UGameplayStatics::PlaySound2D(this, OnEquipSound);
        }
        if (!bWeaponParticle)
        {
            // �����ϸ� weapon particle ����� �����
            IdleParticleComp->Deactivate();
        }
    }
}

void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        AEnemy* Enemy = Cast<AEnemy>(OtherActor);
        if (Enemy)
        {
            if (Enemy->HitParticle)
            {
                const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("WeaponSocket");
                if (WeaponSocket)
                {
                    FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
                    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticle, SocketLocation, FRotator::ZeroRotator, false);
                }
                
            }
            if (Enemy->HitSound)
            {
                UGameplayStatics::PlaySound2D(this, Enemy->HitSound);
            }
            if (DamageTypeClass) {
                UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageTypeClass);
            }
        }

    }
}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AWeapon::ActivateCollision()
{
    CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    
}

void AWeapon::DeactivateCollision()
{
    CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}




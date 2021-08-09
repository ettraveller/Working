// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "MainCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundCue.h>


APickUp::APickUp()
{
    //CoinCount = 10;
}

void APickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//UE_LOG(LogTemp, Warning, TEXT("PickUp::Item Overlap Begin!"));
	if (OtherActor)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			FString PickupNamecpp = *PickupName;

			if (PickupNamecpp.Equals("Coin")) {
				// �����̸� �ٷ� ���� ����
				OnPickupBP(Main);
				Destroy();
			}
			else {
				// Overlap �� potion actor ����
				Main->SetPotion(this, PickupNamecpp);
				//������ collision ���ֱ�
				this->SetActorEnableCollision(false);
				//�ƴϸ� potion ��� ���� 
				Main->CallItemPotion(this);
			}

			if (OverlapParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
			}
			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}
		}
	}

}

void APickUp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	// overlap ���� ����� ����â �Ⱥ��̰� �ϱ�.
	if (OtherActor)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			Main->UnCallItemPotion();
		}
	}

}

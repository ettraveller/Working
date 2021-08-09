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
				// 코인이면 바로 코인 습득
				OnPickupBP(Main);
				Destroy();
			}
			else {
				// Overlap 된 potion actor 설정
				Main->SetPotion(this, PickupNamecpp);
				//닿으면 collision 없애기
				this->SetActorEnableCollision(false);
				//아니면 potion 사용 여부 
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

	// overlap 구역 벗어나면 선택창 안보이게 하기.
	if (OtherActor)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			Main->UnCallItemPotion();
		}
	}

}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Explosive.generated.h"

/**
 * 
 */
UCLASS()
class RPG716_API AExplosive : public AItem
{
	GENERATED_BODY()
public:

	// item �� ��ӹ޾Ƽ� item ���� ���ϴ� ���͵��� �� ����
	AExplosive();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage;

	//virtual override �� ����ؼ� �θ�Ŭ������ �Լ��� ��ӹ޾� ����ϱ⶧���� UFuntion��ũ�θ� ����ϸ� �ߺ��Ǿ �ȴ�
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;
	
};

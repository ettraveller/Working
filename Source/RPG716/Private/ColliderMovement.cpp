// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovement.h"

void UColliderMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    //pawn�����ڰ� null, updatedcomp�� null, update��skip �Ǵ��� checking
    if(!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }
    //�ʱ�ȭ.�븻����¡
    FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f);

    //��ǲ�� ������ �̵��ϴ°�
    if(!DesiredMovementThisFrame.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

        //�������ſ� �ε������� �̲����� �̵��ϴ°� 
        if(Hit.IsValidBlockingHit())
        { 
            SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
            UE_LOG(LogTemp, Warning, TEXT("Blocking!"));
        }
    }
}
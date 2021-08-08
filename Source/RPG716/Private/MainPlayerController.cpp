// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include <Blueprint/UserWidget.h>
#include <Math/Vector2D.h>


void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();


    // �⺻ HUD viewport �� ���̰� �ϴ°�
    if (HUDOverlayAsset)
    {
        HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
    }
    HUDOverlay->AddToViewport();
    HUDOverlay->SetVisibility(ESlateVisibility::Visible);

    // ���� HP Bar viewport�� ���̰� �ϴ°�
    if (WEnemyHealthBar)
    {
        EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
        if (EnemyHealthBar)
        {
            EnemyHealthBar->AddToViewport();
            EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
        }

        FVector2D Alignment(0.f, 0.f);
        EnemyHealthBar->SetAlignmentInViewport(Alignment);
    }

    // PauseMenu viewport �� ���̰� �ϴ°�
	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		if (PauseMenu)
		{
            PauseMenu->AddToViewport();
            PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}

	}

	// Inventory viewport �� ���̰� �ϴ°�
	if (WInventory)
	{
		Inventory = CreateWidget<UUserWidget>(this, WInventory);
		if (Inventory)
		{
			Inventory->AddToViewport();
			Inventory->SetVisibility(ESlateVisibility::Hidden);
		}

	}
}


void AMainPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (EnemyHealthBar)
    {
        FVector2D PositionInViewport;
        ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
        PositionInViewport.Y = 50.f;

        FVector2D SizeInViewport(300.f, 25.f);

		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
    }
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
    if (EnemyHealthBar)
	{
        bEnemyHealthBarVisible = true;
        EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
    }

}


void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = true;
        PauseMenu->SetVisibility(ESlateVisibility::Visible);

        // ���� ����� ���콺ũ Ŀ���� ������ ������ Ŀ���� �ִ°�ó�� �ؼ� pausemenu ��ư�鴩���� �ְ� �ϴ°�
        FInputModeGameAndUI InputModeGameAndUI;
        SetInputMode(InputModeGameAndUI);
        // �ؿ��� ������� ���콺ũ Ŀ���� viewport ���� ����
        bShowMouseCursor = true;
	}
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
    if (PauseMenu)
	{
        GameModeOnly();
		bShowMouseCursor = false;

		bPauseMenuVisible = false;
		//PauseMenu->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::TogglePauseMenu()
{
    // �ڿ� Implementation ���̸� �ִϸ��̼� ���� �ȵ����� �θ�Ŭ������ ȣ��ɼ� �ְ� �α�
    if (bPauseMenuVisible)
    {
        RemovePauseMenu();
    }
    else {
        DisplayPauseMenu();
    }

}

void AMainPlayerController::GameModeOnly()
{
	// pasuemenu ���� ������� ���콺 Ŀ���� �������, �Ϲ����� ���Ӹ��� �ٲٱ�
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}

void AMainPlayerController::DisplayInventoryMenu_Implementation()
{
	if (Inventory)
	{
		bInventoryMenu = true;
        Inventory->SetVisibility(ESlateVisibility::Visible);

		// ���� ����� ���콺ũ Ŀ���� ������ ������ Ŀ���� �ִ°�ó�� �ؼ� pausemenu ��ư�鴩���� �ְ� �ϴ°�
		FInputModeGameAndUI InputModeGameAndUI;
		SetInputMode(InputModeGameAndUI);
		// �ؿ��� ������� ���콺ũ Ŀ���� viewport ���� ����
		bShowMouseCursor = true;
	}

}

void AMainPlayerController::RemoveInvnetoryMenu_Implementation()
{
	if (Inventory)
	{
		GameModeOnly();
		bShowMouseCursor = false;

		bInventoryMenu = false;
		Inventory->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::ToggleInventoryMenu()
{
	if (bInventoryMenu)
	{
		RemoveInvnetoryMenu_Implementation();
	}
	else {
		DisplayInventoryMenu_Implementation();
	}
}

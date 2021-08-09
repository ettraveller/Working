// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <Blueprint/UserWidget.h>
#include <Templates/SubclassOf.h>
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPG716_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    
	//Reference to UMG asset
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	//���� ������ ���� ������
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;

	// ���� HP ǥ��
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WEnemyHealthBar;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* EnemyHealthBar;

	// RESUME / SAVEGAME / LOADGAME / QUIT HUD ����� 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WPauseMenu;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* PauseMenu;

	// Inventory HUD �����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WInventory;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* Inventory;

	// ��� �Ⱦ��� �������� �κ��丮 ������ �����ϴ� HUD
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WItemEquip;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* ItemEquip;

	// ���� �Ⱦ��� ������� �κ��丮 ������ �����ϴ� HUD
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WItemPotion;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* ItemPotion;

	bool bEnemyHealthBarVisible;

	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

	// overlap �� ���� enemy ��ġ
	FVector EnemyLocation;

	// overlap �� weapon ���� ��ġ
	FVector WeaponLocation;

	// overlap �� Potion ���� ��ġ
	FVector PotionLocation;

	bool bPauseMenuVisible;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayPauseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemovePauseMenu();

	void TogglePauseMenu();

	UFUNCTION(BlueprintCallable)
	void GameModeOnly();

	// �κ��丮 â �̺�Ʈ
	UFUNCTION(BlueprintNativeEvent, Category = "HUD")
	void DisplayInventoryMenu();

	UFUNCTION(BlueprintNativeEvent, Category = "HUD")
	void RemoveInvnetoryMenu();

	void ToggleInventoryMenu();

	bool bInventoryMenu;

	// ��� ���۽� ���� ���� HUD
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayItemEquipMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveItemEquipMenu();

	//void ToggleItemEquipMenu();

	void ViewItemEquipMenu();

	void UnableItemEquipMenu();

	bool bItemEquipMenu;


	// ���� ���۽� ��� ���� HUD
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayItemPotionMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemoveIItemPotionMenu();

	//void ToggleItemPotionMenu();

	void ViewItemPotionMenu();

	void UnableItemPotionMenu();

	bool bItemPotionMenu;

protected:

    virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};

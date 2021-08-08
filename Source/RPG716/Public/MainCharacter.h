// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"



//��, ���⼭ �����ؾ��� ���� �ִµ�, �𸮾� �������� ���鶧 �ݵ�� �Ϲ����� enum�� �ƴ� enum class�� ������ �Ѵٴ� ���̴�.
//���� enum class�� ������ �ʰ� �Ϲ����� enum���� ���� UENUM() ��ũ�θ� ���̰� �������� �ϸ� ������ �߻��ؼ� �����Ͽ� �����Ѵ�.
//�׸��� UENUM�� uint8���� �����ϱ� ������ �� �κе� ���߸��� �ʰ� �־��־�� �Ѵ�.


UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
// UMETA = ���÷��ǰ� �������Ʈ�� ����

    EMS_Normal UMETA(DisplayName = "EMS_Normal"),
    EMS_Sprinting UMETA(DisplayName = "EMS_Sprinting"),
    EMS_Dead UMETA(DisplayName = "EMS_Dead"),

    EMS_Max UMETA(DisplayName = "EMS_DefaultMax"),

};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
    ESS_Normal UMETA(DisplayName = "ESS_Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "ESS_BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "ESS_Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ESS_ExhaustedRecovering"),

	ESS_Max UMETA(DisplayName = "ESS_DefaultMax"),

};

//Character�� rootcomp�� capsule�ۿ� �ȴ�
UCLASS()
class RPG716_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();
	
	// Weapon ������ ����ÿ� �ʿ� -> Weapon �����͵��� ����Ǿ� �ִ� BP_ItemStorage �� save �ؾ���!
	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	TSubclassOf<class AItemStorage> WeaponStorage;

	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	TSubclassOf<class ASkillDataTable> SkillData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bHasCombatTarget;

	FORCEINLINE void setHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }

	// Enemy healthbar ���Ϳ� ��Ÿ���� �ϱ� ���ؼ�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector CombatTargetLocation;

	// Enemy hp ���� ���ؼ� �ʿ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* MainPlayerController;

	TArray<FVector> PickUpLocations;

	// Tarray �� ������ pcikuplocation ���� ǥ������ �Լ�
	UFUNCTION(BlueprintCallable)
	void ShowPickUpLocation();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StaminaDrainRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSprintStamina;

	float InterSpeed;
	bool bInterToEnemy;
	void SetInterToEnenmy(bool Interp);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class AEnemy* CombatTarget;

	FRotator GetLookAtRotaionYaw(FVector Target);

	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target; }

	// movement ���� ���� �� running speed ����
	//set SetMovementStatus and runnigspeed
	void SetMovementStatus(EMovementStatus Status);

	// sprinting ��忡 ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float SprintSpeed;

	bool bShiftKeyDown;

	// down �� ������ sprinting ���� ��ȯ
	//pressed 
	void ShiftKeyDown();
	// up �� ������ sprinting ��� ����
	//released 
	void ShiftKeyUp();

	// �÷��̾� ���ʿ��� ī�޶� Ȯ�ΰ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class USoundCue* HitSound;

	////////
	// Player Stats
	////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// forward, backward input �� �ҷ�����
	//playerinput
	void MoveForward(float Value);

	// Y �� �� �ҷ�����
	void MoveRight(float Value);

	void Turn(float Value);
	void LookUp(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void Dash();
	void StopDash();
	void ResetDash();
	UPROPERTY()
	FTimerHandle DashTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	float DashDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	float DashCoolDown;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dash")
	float StopDashTime;
	
	bool bCanDash;
	

	// ������ ���¸� Ȯ���Ͽ� sprint �����ϰ� �ϱ�
	bool bMovingForward;
	bool bMovingRight;
	
	// ������ üũ�ؾ��ϴ°͵� üũ�ϴ� �Լ�
	bool CanMove(float Value);

	
	//HUD
	void DecrementHealth(float Amount);
	
	// ���ݴ��ϸ� health �ٰ��ϱ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void IncrementCoins(int32 Amount);

	UFUNCTION(BlueprintCallable)
	void IncrementHealth(float Amount);
	void Die();

	virtual void Jump() override;

	//Sound
	void LMBDown();
	void LMBUp();
	bool bLMBDown;

	//PauseMenu
	void ESCDown();
	void ESCUp();
	bool bESCDown;

	//inventory
	void InventoryDown();
	void InventoryUp();
	bool bInventoryDown;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// ��� �����Ҷ� �ʿ�
	//SetEquipWeapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	class AWeapon* EquipWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AItem* ActiveOverlappingItem;

	void SetEquipWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquipWeapon() { return EquipWeapon; }
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bAttacking;

	void Attack();

	// �����̰ų� �����Ҷ��� ���� ���ϰ� �ϱ� BP���� ����
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	// CombatMontage ����� �а� ����ϱ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	// ĳ���� �״� anim �����
	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	// �θ����� ���Ͱ� ������ �Ѹ��� ���̸� �ٸ� �Ѹ����� ���ݴ�� �ٲٰ� �ϱ�
	void UpdateCombatTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AEnemy> EnemyFilter;

	// level�� ��ȯ�ÿ� �ʿ�
	void Switchlevel(FName LevelName);

	// ���� ����ÿ� �ʿ�
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPosition);

	void LoadGameNoSwitch();
	//
//private:
//	/*Returns a fixed transform based on the given spring arm comp*/
	UFUNCTION()
	FTransform GetFixedSpringArmTransform(USpringArmComponent* SpringArm);
	//
	//	/*Returns an array of transform in order to determine how many skills will get spawned*/
	UFUNCTION()
	TArray<FTransform> GetSpawnTransforms(int32 level);
	//
	//	/*The root component in which the spring arm components will be attached*/
	//protected:
	/*Returns the skills component*/
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SkillsRootComp;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* LevelOneSpringArm;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* LevelTwoSpringArm;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* LevelThreeSpringArm;

	//	/*Skills Component reference*/
	UPROPERTY(VisibleAnywhere)
	class USkillComponent* SkillsComponent;

	UFUNCTION(BlueprintCallable, Category = TLSkillsTree)
	USkillComponent* GetSkillsComponent() const { return SkillsComponent; }

	UPROPERTY(BlueprintReadWrite, Category = "skill")
	class ASkill* ValueSkill6;
	UFUNCTION(BlueprintCallable, Category = TLSkillsTree)
    bool IsValidGetLevel_Fire();

	UFUNCTION(BlueprintCallable, Category = TLSkillsTree)
    bool IsValidGetLevel_Poision();

	UFUNCTION(BlueprintCallable, Category = TLSkillsTree)
	void Fire_FireBall();

	UFUNCTION(BlueprintCallable, Category = TLSkillsTree)
	void Fire_PoisionBall();

	void ResetFire();

	bool bCanFire;

	float FireCoolDown;


};

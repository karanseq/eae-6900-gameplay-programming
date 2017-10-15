// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "WheeledVehicle.h"

#include "EAE_6900Pawn.generated.h"

// forward declarations
class AEAE_6900Bullet;
class UAudioComponent;
class UBoxComponent;
class UCameraComponent;
class UInputComponent;
class UPhysicalMaterial;
class USpringArmComponent;

/**
 * 
 */
UCLASS(config = Game)
class EAE_6900_API AEAE_6900Pawn : public AWheeledVehicle
{
	//~==============================================================================
	// Initialization
    GENERATED_UCLASS_BODY()

public:
    void SetupVehicleProperties();

	//~==============================================================================
	// Movement and Input
public:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void OnHandbrakePressed();
    void OnHandbrakeReleased();
    void OnResetPressed();
    void OnFirePressed();
    void OnFireReleased();

    static const FName EngineAudioRPM;

    void ResetVehicle();

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerResetVehicle();

private:
    void UpdatePhysicsMaterial();
	
private:
    bool bIsLowFriction;
    UPhysicalMaterial* SlipperyMaterial;
    UPhysicalMaterial* NonSlipperyMaterial;

	//~==============================================================================
	// Health and Ammo
protected:
    UPROPERTY(Category = Stats, EditDefaultsOnly)
    float MaxHealth = 10000.0f;

    UPROPERTY(Category = Stats, VisibleAnywhere, ReplicatedUsing = OnRep_Health)
    float Health = MaxHealth;

    UFUNCTION()
    void OnRep_Health();

    UPROPERTY(Category = Stats, EditDefaultsOnly)
    int32 MaxAmmo = 3000;

    UPROPERTY(Category = Stats, VisibleAnywhere, ReplicatedUsing = OnRep_Ammo)
    int32 Ammo = MaxAmmo;

    UFUNCTION()
    void OnRep_Ammo();

    UPROPERTY(Category = State, VisibleAnywhere, BlueprintReadOnly, Replicated)
    bool bIsFirePressed = false;
    UPROPERTY(Category = State, VisibleAnywhere, BlueprintReadOnly, Replicated)
    bool bCanFire = true;
    UPROPERTY(Category = State, VisibleAnywhere, BlueprintReadOnly, Replicated)
    float FireRate = 0.2f;

public:
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//~==============================================================================
	// HUD
public:
    UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
    FText HealthText;

    UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
    FText AmmoText;

    UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
    FText SpeedText;

    UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
    FText RPMText;

private:
    void UpdateHealthText();
	void UpdateAmmoText();
	void UpdateHUDStrings();

	//~==============================================================================
	// Weapons and Firing
public:
    UPROPERTY(EditAnywhere, Category = AssetReferences)
    TSubclassOf<AEAE_6900Bullet> DefaultBulletBP;

private:
    void RequestStartFiringWeapon();
    void StartFiringWeapon();

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerStartFiringWeapon();

    void RequestStopFiringWeapon();
    void StopFiringWeapon();

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerStopFiringWeapon();

    void FireWeapon();

	//~==============================================================================
	// Lifecycle
public:
	// begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// end Pawn interface

	// begin Actor interface
	virtual void Tick(float Delta) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private:
	int32 PlayerID = -1;

	//~==============================================================================
	// Components
public:
    FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
    FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
    FORCEINLINE UAudioComponent* GetEngineSound() const { return EngineSound; }

private:
    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;

    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera;

    UPROPERTY(Category = Audio, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UAudioComponent* EngineSound;

};

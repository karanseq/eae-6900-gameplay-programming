// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// engine includes
#include "CoreMinimal.h"
#include "WheeledVehicle.h"

#include "EAE_6900Pawn.generated.h"

// forward declarations
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
    GENERATED_BODY()

    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;

    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera;

    UPROPERTY(Category = Audio, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UAudioComponent* EngineSound;

public:
    AEAE_6900Pawn();

    void SetupVehicleProperties();

    UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
    FText SpeedText;
    UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
    FText RPMText;

    // begin Pawn interface
    virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
    // end Pawn interface

    // begin Actor interface
    virtual void Tick(float Delta) override;
protected:
    virtual void BeginPlay() override;

public:
    // end Actor interface

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
    void ServerResetVehicle_Implementation();
    bool ServerResetVehicle_Validate();

    UFUNCTION()
    void EventOnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

private:
    void UpdateHUDStrings();

    void UpdatePhysicsMaterial();

    bool bIsLowFriction;
    UPhysicalMaterial* SlipperyMaterial;
    UPhysicalMaterial* NonSlipperyMaterial;

protected:
    UPROPERTY(Category = Stats, EditAnywhere)
    float MaxHealth = 100.0f;

    UPROPERTY(Category = Stats, VisibleAnywhere, Transient, ReplicatedUsing = OnRep_Health)
    float Health = MaxHealth;

    UFUNCTION()
    void OnRep_Health();

    UPROPERTY(Category = Stats, EditAnywhere)
    int32 MaxAmmo = 30;

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
    UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
    FText HealthText;

    UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
    FText AmmoText;

private:
    void UpdateHealthText();
    void UpdateAmmoText();

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    void StartFiringWeapon();
    void RequestStartFiringWeapon();

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerStartFiringWeapon();
    void ServerStartFiringWeapon_Implementation();
    bool ServerStartFiringWeapon_Validate();

    void StopFiringWeapon();
    void RequestStopFiringWeapon();

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerStopFiringWeapon();
    void ServerStopFiringWeapon_Implementation();
    bool ServerStopFiringWeapon_Validate();

    void FireWeapon();

public:
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
    FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
    FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
    FORCEINLINE UAudioComponent* GetEngineSound() const { return EngineSound; }
};

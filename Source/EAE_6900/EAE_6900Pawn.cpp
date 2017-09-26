// Fill out your copyright notice in the Description page of Project Settings.

#include "EAE_6900Pawn.h"

// engine includes
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
#include "WheeledVehicleMovementComponent4W.h"

// game includes
#include "EAE_6900.h"
#include "EAE_6900Bullet.h"
#include "EAE_6900GameInstance.h"
#include "EAE_6900Hud.h"
#include "EAE_6900WheelFront.h"
#include "EAE_6900WheelRear.h"

const FName AEAE_6900Pawn::EngineAudioRPM("RPM");

AEAE_6900Pawn::AEAE_6900Pawn() : Health(MaxHealth),
Ammo(MaxAmmo)
{
    bReplicates = 1;
    bAlwaysRelevant = 1;
    bReplicateMovement = 1;

    // car mesh
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/VehicleAdv/Vehicle/Vehicle_SkelMesh.Vehicle_SkelMesh"));
    GetMesh()->SetSkeletalMesh(CarMesh.Object);
    GetMesh()->bGenerateOverlapEvents = 1;

    // car animation
    static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/VehicleAdv/Vehicle/VehicleAnimationBlueprint"));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

    // setup friction materials
    static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> SlipperyMat(TEXT("/Game/VehicleAdv/PhysicsMaterials/Slippery.Slippery"));
    SlipperyMaterial = SlipperyMat.Object;

    static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> NonSlipperyMat(TEXT("/Game/VehicleAdv/PhysicsMaterials/Slippery.Slippery"));
    NonSlipperyMaterial = NonSlipperyMat.Object;

    SetupVehicleProperties();

    // spring arm
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 34.0f));
    SpringArm->SetWorldRotation(FRotator(-75.0f, 0.0f, 0.0f));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 1200.0f;
    SpringArm->bEnableCameraLag = true;
    SpringArm->bEnableCameraRotationLag = false;
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritRoll = false;
    SpringArm->bInheritYaw = false;

    // camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->SetRelativeLocation(FVector(-125.0f, 0.0f, 0.0f));
    Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    Camera->bUsePawnControlRotation = false;
    Camera->FieldOfView = 90.0f;

    // engine sound
    static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/VehicleAdv/Sound/Engine_Loop_Cue.Engine_Loop_Cue"));
    EngineSound = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
    EngineSound->SetSound(SoundCue.Object);
    EngineSound->SetupAttachment(GetMesh());

    bIsLowFriction = false;

    // visuals
    UpdateHealthText();
    UpdateAmmoText();

    OnActorHit.AddDynamic(this, &AEAE_6900Pawn::EventOnActorHit);
}

void AEAE_6900Pawn::SetupVehicleProperties()
{
    // setup vehicle movement component
    UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
    check(Vehicle4W->WheelSetups.Num() == 4);

    // Wheels/Tyres
    // setup the wheels
    Vehicle4W->WheelSetups[0].WheelClass = UEAE_6900WheelFront::StaticClass();
    Vehicle4W->WheelSetups[0].BoneName = FName("PhysWheel_FL");
    Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -8.f, 0.f);

    Vehicle4W->WheelSetups[1].WheelClass = UEAE_6900WheelFront::StaticClass();
    Vehicle4W->WheelSetups[1].BoneName = FName("PhysWheel_FR");
    Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 8.f, 0.f);

    Vehicle4W->WheelSetups[2].WheelClass = UEAE_6900WheelRear::StaticClass();
    Vehicle4W->WheelSetups[2].BoneName = FName("PhysWheel_BL");
    Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -8.f, 0.f);

    Vehicle4W->WheelSetups[3].WheelClass = UEAE_6900WheelRear::StaticClass();
    Vehicle4W->WheelSetups[3].BoneName = FName("PhysWheel_BR");
    Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 8.f, 0.f);

    // Adjust the tire loading
    Vehicle4W->MinNormalizedTireLoad = 0.0f;
    Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
    Vehicle4W->MaxNormalizedTireLoad = 2.0f;
    Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;

    // Engine 
    // Torque setup
    Vehicle4W->MaxEngineRPM = 6000.0f;
    Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
    Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
    Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
    Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f, 400.0f);

    // Adjust the steering 
    Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
    Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
    Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
    Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);

    // Transmission	
    // We want 4wd
    Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;

    // Drive the front wheels a little more than the rear
    Vehicle4W->DifferentialSetup.FrontRearSplit = 0.65;

    // Automatic gearbox
    Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
    Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
    Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

    // Physics settings
    // Adjust the center of mass - the buggy is quite low
    UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(Vehicle4W->UpdatedComponent);
    if (UpdatedPrimitive)
    {
        UpdatedPrimitive->BodyInstance.COMNudge = FVector(8.0f, 0.0f, 0.0f);
    }

    // Set the inertia scale. This controls how the mass of the vehicle is distributed.
    Vehicle4W->InertiaTensorScale = FVector(1.0f, 1.333f, 1.2f);
}

void AEAE_6900Pawn::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
    Super::SetupPlayerInputComponent(InputComponent);
    check(InputComponent);

    InputComponent->BindAxis("MoveForward", this, &AEAE_6900Pawn::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AEAE_6900Pawn::MoveRight);
    InputComponent->BindAction("Handbrake", IE_Pressed, this, &AEAE_6900Pawn::OnHandbrakePressed);
    InputComponent->BindAction("Handbrake", IE_Released, this, &AEAE_6900Pawn::OnHandbrakeReleased);
    InputComponent->BindAction("ResetVehicle", IE_Pressed, this, &AEAE_6900Pawn::OnResetPressed);
    InputComponent->BindAction("Fire", IE_Pressed, this, &AEAE_6900Pawn::OnFirePressed);
    InputComponent->BindAction("Fire", IE_Released, this, &AEAE_6900Pawn::OnFireReleased);
}

void AEAE_6900Pawn::Tick(float Delta)
{
    Super::Tick(Delta);

    UpdatePhysicsMaterial();

    UpdateHUDStrings();

    float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
    EngineSound->SetFloatParameter(EngineAudioRPM, GetVehicleMovement()->GetEngineRotationSpeed() * RPMToAudioScale);

    if (bIsFirePressed && bCanFire && Role == ROLE_Authority)
    {
        FireWeapon();
    }
}

void AEAE_6900Pawn::BeginPlay()
{
    Super::BeginPlay();

    //EngineSound->Play();
    EngineSound->SetVolumeMultiplier(0.0f);
}

void AEAE_6900Pawn::MoveForward(float Value)
{
    GetVehicleMovementComponent()->SetThrottleInput(Value);
}

void AEAE_6900Pawn::MoveRight(float Value)
{
    GetVehicleMovementComponent()->SetSteeringInput(Value);
}

void AEAE_6900Pawn::OnHandbrakePressed()
{
    GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AEAE_6900Pawn::OnHandbrakeReleased()
{
    GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AEAE_6900Pawn::OnFirePressed()
{
    RequestStartFiringWeapon();
}

void AEAE_6900Pawn::OnFireReleased()
{
    RequestStopFiringWeapon();
}

void AEAE_6900Pawn::OnResetPressed()
{
    if (Role < ROLE_Authority)
    {
        ServerResetVehicle();
    }
    else
    {
        ResetVehicle();
    }
}

void AEAE_6900Pawn::ResetVehicle()
{
    FVector NewLocation = GetActorLocation();
    NewLocation.Z += 75.0f;

    FRotator NewRotation = GetActorRotation();
    NewRotation.Roll = 0.0f;
    NewRotation.Pitch = 0.0f;

    SetActorLocationAndRotation(NewLocation, NewRotation, false, nullptr, ETeleportType::TeleportPhysics);
}

void AEAE_6900Pawn::ServerResetVehicle_Implementation()
{
    ResetVehicle();
}

bool AEAE_6900Pawn::ServerResetVehicle_Validate()
{
    return true;
}

void AEAE_6900Pawn::EventOnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    //UE_LOG(NSLog, Log, TEXT(__FUNCTION__));
}

void AEAE_6900Pawn::UpdateHUDStrings()
{
    // get speed from vehicle
    float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
    int32 KPH_int = FMath::FloorToInt(KPH);

    SpeedText = FText::FromString(FString::Printf(TEXT("%d km/h"), KPH_int));

    // get RPM from vehicle
    int RPM = FMath::FloorToInt(GetVehicleMovement()->GetEngineRotationSpeed());
    static int MaxRPM = FMath::FloorToInt(GetVehicleMovement()->GetEngineMaxRotationSpeed());

    RPMText = FText::FromString(FString::Printf(TEXT("RPM: %d/%d"), RPM, MaxRPM));
}

void AEAE_6900Pawn::UpdatePhysicsMaterial()
{
    if (GetActorUpVector().Z < 0)
    {
        if (bIsLowFriction == true)
        {
            GetMesh()->SetPhysMaterialOverride(NonSlipperyMaterial);
            bIsLowFriction = false;
        }
        else
        {
            GetMesh()->SetPhysMaterialOverride(SlipperyMaterial);
            bIsLowFriction = true;
        }
    }
}

void AEAE_6900Pawn::OnRep_Health()
{
    UpdateHealthText();
}

void AEAE_6900Pawn::UpdateHealthText()
{
    HealthText = FText::FromString(FString::Printf(TEXT("Health: %d/%d"), FMath::FloorToInt(Health), FMath::FloorToInt(MaxHealth)));
}

void AEAE_6900Pawn::OnRep_Ammo()
{
    UpdateAmmoText();
}

void AEAE_6900Pawn::UpdateAmmoText()
{
    AmmoText = FText::FromString(FString::Printf(TEXT("Ammo: %d/%d"), Ammo, MaxAmmo));
}

void AEAE_6900Pawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AEAE_6900Pawn, Health);
    DOREPLIFETIME(AEAE_6900Pawn, Ammo);
    DOREPLIFETIME(AEAE_6900Pawn, bIsFirePressed);
    DOREPLIFETIME(AEAE_6900Pawn, bCanFire);
}

void AEAE_6900Pawn::RequestStartFiringWeapon()
{
    if (Role < ROLE_Authority)
    {
        ServerStartFiringWeapon();
    }
    else
    {
        StartFiringWeapon();
    }
}

void AEAE_6900Pawn::StartFiringWeapon()
{
    bIsFirePressed = true;
}

void AEAE_6900Pawn::ServerStartFiringWeapon_Implementation()
{
    StartFiringWeapon();
}

bool AEAE_6900Pawn::ServerStartFiringWeapon_Validate()
{
    return true;
}

void AEAE_6900Pawn::RequestStopFiringWeapon()
{
    if (Role < ROLE_Authority)
    {
        ServerStopFiringWeapon();
    }
    else
    {
        StopFiringWeapon();
    }
}

void AEAE_6900Pawn::StopFiringWeapon()
{
    bIsFirePressed = false;
}

void AEAE_6900Pawn::ServerStopFiringWeapon_Implementation()
{
    StopFiringWeapon();
}

bool AEAE_6900Pawn::ServerStopFiringWeapon_Validate()
{
    return true;
}

void AEAE_6900Pawn::FireWeapon()
{
    bCanFire = false;

    if (Ammo <= 0)
    {
        return;
    }

    // reduce ammo
    //--Ammo;

    // initialize spawn parameters
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Instigator = this;
    SpawnParameters.Owner = GetController();

    // get a reference to the game instance
    UEAE_6900GameInstance* GameInstance = Cast<UEAE_6900GameInstance>(GetGameInstance());
    check(GameInstance);

    // calculate the bullet's initial position
    const FVector BulletLocation = GetActorLocation() + GetActorForwardVector() * 80.0f + GetActorUpVector() * 50.0f;

    // spawn the bullet
    GetWorld()->SpawnActor<AEAE_6900Bullet>(GameInstance->DefaultBulletBP, BulletLocation, GetActorRotation(), SpawnParameters);

    // set a timer based on the gun's rate of fire
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;

    TimerDelegate.BindLambda([&]()
    {
        bCanFire = true;
    });

    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, FireRate, false);
}

float AEAE_6900Pawn::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    if (Role == ROLE_Authority)
    {
        // reduce health
        //Health -= DamageAmount;
        Health = FMath::Clamp(Health, 0.0f, MaxHealth);

        // slow down
        GetVehicleMovementComponent()->SetHandbrakeInput(true);
    }

    return DamageAmount;
}


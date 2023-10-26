// Fill out your copyright notice in the Description page of Project Settings.


#include "COAAvatar.h"
#include "GameFramework/CharacterMovementComponent.h"

ACOAAvatar::ACOAAvatar()
{
    mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    mSpringArm->TargetArmLength = 300.0f;
    mSpringArm->SetupAttachment(RootComponent);

    mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
    mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);

    mCamera->bUsePawnControlRotation = false;
    mSpringArm->bUsePawnControlRotation = true;
    bUseControllerRotationYaw = false;
}

// Called a input 
void ACOAAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);

    PlayerInputComponent->BindAxis("MoveForward", this, &ACOAAvatar::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACOAAvatar::MoveRight);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACOAAvatar::RunPressed);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ACOAAvatar::RunReleased);


}
// Called when the game starts or when spawned
void ACOAAvatar::BeginPlay()
{
    Super::BeginPlay();

}

// Hareket hýzýný güncellemek 
void ACOAAvatar::UpdateMovementParams()
{
    GetCharacterMovement()->MaxWalkSpeed = bRunning && !bStaminaDrained ? RunSpeed : WalkSpeed;
}

void ACOAAvatar::RunPressed()
{
    bRunning = true;
    UpdateMovementParams();
}

void ACOAAvatar::RunReleased()
{
    bRunning = false;
    UpdateMovementParams();
}

void ACOAAvatar::Tick(float DeltaTime)
{
   if(GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
       if (bRunning && !bStaminaDrained)
       {
           if (!GetCharacterMovement()->Velocity.IsNearlyZero(0.01f))
           {
               Stamina = FMath::Max(0.0f, Stamina - StaminaDrainRate * DeltaTime);
               if (Stamina == 0.0f)
               {
                   bStaminaDrained = true;
                   UpdateMovementParams();
               }
           }
       }
       else
       {
           Stamina = FMath::Min(MaxStamina, Stamina + StaminaGainRate * DeltaTime);
           if (Stamina >= MaxStamina)
           {
               bStaminaDrained = false;
               UpdateMovementParams();
           }
       }
   GEngine->AddOnScreenDebugMessage(0, DeltaTime, FColor::Red, FString::Printf(TEXT("Stamina %f"), Stamina));
}

void ACOAAvatar::MoveForward(float value)
{
    FRotator Rotation = GetController()->GetControlRotation();
    FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, value);
}
void ACOAAvatar::MoveRight(float value)
{
    FRotator Rotation = GetController()->GetControlRotation();
    FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
    FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, value);
}

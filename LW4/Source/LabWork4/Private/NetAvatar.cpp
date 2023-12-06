// Fill out your copyright notice in the Description page of Project Settings.


#include "NetAvatar.h"
#include "GameFrameWork/CharacterMovementComponent.h"

ANetAvatar::ANetAvatar()
{
	//SpringArm ve Camera bileþenlerini oluþturma 
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void ANetAvatar::BeginPlay()
{
	Super::BeginPlay();

	Camera->bUsePawnControlRotation = false;
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

// Oyuncu giriþi için gerekli ayarlarý yapýlandýran iþlev
void ANetAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Kamera kontrolü için eksenler
	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &ACharacter::AddControllerPitchInput);

	// Hareket için eksenleri ve koþma
	PlayerInputComponent->BindAxis("MoveForward", this, &ANetAvatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANetAvatar::MoveRight);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ANetAvatar::RunPressed);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ANetAvatar::RunReleased);

}

//kontrol rotasyonunu_ileri
void ANetAvatar::MoveForward(float Scale)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, Scale);
}

// Karakterin kontrol rotasyonunu_sað
void ANetAvatar::MoveRight(float Scale)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(ForwardDirection, Scale);
}

// Hareket parametrelerini güncelleme
void ANetAvatar::UpdateCharacterMovement()
{
	// Koþma tuþuna basýlýysa yürüme hýzýný güncelle
	if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f; //koþma
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f; //yürüme
	}
}

// Koþma tuþuna basýldýðýnda çaðrýlanlar
void ANetAvatar::RunPressed()
{
	if (HasAuthority()) // Sunucuda çalýþtýrýlýyorsa
	{
		bIsRunning = true;
		UpdateCharacterMovement(); // Hareket parametrelerini güncelle
	}
	else // Sunucuda deðilse
	{
		InitiateServerRun(); // Sunucuya koþma komutunu gönderir
	}
}

// Pressedteki iþlemin tersi
void ANetAvatar::RunReleased()
{
	if (HasAuthority()) // Sunucuda çalýþtýrýlýyorsa
	{
		bIsRunning = false;
		UpdateCharacterMovement(); // Hareket parametrelerini güncelle
	}
	else // Sunucuda deðilse
	{
		CeaseServerRun(); // sunucuya koþmama komutu gönderme 
	}
}

// Sunucuda koþma baþlatma
void ANetAvatar::InitiateServerRun_Implementation()
{
	RunPressed();
}

void ANetAvatar::CeaseServerRun_Implementation()
{
	RunReleased();
}

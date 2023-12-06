// Fill out your copyright notice in the Description page of Project Settings.


#include "NetAvatar.h"
#include "GameFrameWork/CharacterMovementComponent.h"

ANetAvatar::ANetAvatar()
{
	//SpringArm ve Camera bile�enlerini olu�turma 
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

// Oyuncu giri�i i�in gerekli ayarlar� yap�land�ran i�lev
void ANetAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Kamera kontrol� i�in eksenler
	PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &ACharacter::AddControllerPitchInput);

	// Hareket i�in eksenleri ve ko�ma
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

// Karakterin kontrol rotasyonunu_sa�
void ANetAvatar::MoveRight(float Scale)
{
	FRotator Rotation = GetController()->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(ForwardDirection, Scale);
}

// Hareket parametrelerini g�ncelleme
void ANetAvatar::UpdateCharacterMovement()
{
	// Ko�ma tu�una bas�l�ysa y�r�me h�z�n� g�ncelle
	if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f; //ko�ma
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f; //y�r�me
	}
}

// Ko�ma tu�una bas�ld���nda �a�r�lanlar
void ANetAvatar::RunPressed()
{
	if (HasAuthority()) // Sunucuda �al��t�r�l�yorsa
	{
		bIsRunning = true;
		UpdateCharacterMovement(); // Hareket parametrelerini g�ncelle
	}
	else // Sunucuda de�ilse
	{
		InitiateServerRun(); // Sunucuya ko�ma komutunu g�nderir
	}
}

// Pressedteki i�lemin tersi
void ANetAvatar::RunReleased()
{
	if (HasAuthority()) // Sunucuda �al��t�r�l�yorsa
	{
		bIsRunning = false;
		UpdateCharacterMovement(); // Hareket parametrelerini g�ncelle
	}
	else // Sunucuda de�ilse
	{
		CeaseServerRun(); // sunucuya ko�mama komutu g�nderme 
	}
}

// Sunucuda ko�ma ba�latma
void ANetAvatar::InitiateServerRun_Implementation()
{
	RunPressed();
}

void ANetAvatar::CeaseServerRun_Implementation()
{
	RunReleased();
}

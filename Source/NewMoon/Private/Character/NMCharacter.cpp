// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NMCharacter.h"

ANMCharacter::ANMCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set Collision Capsule
	GetCapsuleComponent()->InitCapsuleSize(45.0f, 100.0f);
	
	// Set Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("/Game/Assets/Characters/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));

	// Set Movement
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	
	// Set Control Rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	SpringArm->bUsePawnControlRotation = true;

	// Set Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));

	// Set Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> Mannequin_Anim(TEXT("/Game/Blueprints/Character/Animation/ABP_NMCharacter.ABP_NMCharacter_C"));
	if (Mannequin_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Mannequin_Anim.Class);
	}
}

void ANMCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ANMCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &ANMCharacter::RunButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &ANMCharacter::RunButtonReleased);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ANMCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Prone"), IE_Pressed, this, &ANMCharacter::ProneButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &ANMCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &ANMCharacter::AimButtonReleased);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ANMCharacter::AttackButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ANMCharacter::InteractButtonPressed);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ANMCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ANMCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ANMCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ANMCharacter::LookUp);
}

void ANMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANMCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void ANMCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void ANMCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ANMCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ANMCharacter::Jump()
{
	Super::Jump();
}

void ANMCharacter::RunButtonPressed()
{
}

void ANMCharacter::RunButtonReleased()
{
}

void ANMCharacter::CrouchButtonPressed()
{
}

void ANMCharacter::ProneButtonPressed()
{
}

void ANMCharacter::AimButtonPressed()
{
}

void ANMCharacter::AimButtonReleased()
{
}

void ANMCharacter::AttackButtonPressed()
{
}

void ANMCharacter::InteractButtonPressed()
{
}


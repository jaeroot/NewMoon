// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NMCharacter.h"

#include "Character/NMCharacterMovementComponent.h"

ANMCharacter::ANMCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNMCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	// Set Collision Capsule
	StandHalfHeight = 100.0f;
	CrouchHalfHeight = 70.0f;
	ProneHalfHeight = 45.0f;
	Radius = 45.0f;
	GetCapsuleComponent()->InitCapsuleSize(Radius, StandHalfHeight);
	
	// Set Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("/Game/Assets/Characters/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin"));
	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -StandHalfHeight), FRotator(0.0f, -90.0f, 0.0f));

	// Set Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = false;
	GetCharacterMovement()->SetIsReplicated(true);
	
	// Set Control Rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	ControlRotationBlocked = false;
	ControlRotation = FRotator::ZeroRotator;
	
	CameraRotTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RotTimeline"));
	CameraRotTimelineFunction.BindUFunction(this, FName("CameraRotInterp"));
	CameraRotTimelineFinish.BindUFunction(this, FName("CameraRotFinish"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> Camera_Rot_Curve(TEXT("/Game/Blueprints/Character/Animation/CameraRotCurve.CameraRotCurve"));
	if (Camera_Rot_Curve.Succeeded())
	{
		CameraRotCurve = Camera_Rot_Curve.Object;
		CameraRotTimeline->AddInterpFloat(CameraRotCurve, CameraRotTimelineFunction);
		CameraRotTimeline->SetTimelineFinishedFunc(CameraRotTimelineFinish);
	}
	CameraRotTimeline->SetLooping(false);

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
	Camera->bUsePawnControlRotation = false;

	// Set Animation
	static ConstructorHelpers::FClassFinder<UAnimInstance> Mannequin_Anim(TEXT("/Game/Blueprints/Character/Animation/ABP_NMCharacter.ABP_NMCharacter_C"));
	if (Mannequin_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Mannequin_Anim.Class);
	}

	// Set Crouch timeline
	CrouchTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CrouchTimeline"));
	CrouchTimelineFunction.BindUFunction(this, FName("CrouchInterp"));
	CrouchTimelineFinish.BindUFunction(this, FName("CrouchFinish"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Crouch_Curve(TEXT("/Game/Blueprints/Character/Animation/CrouchCurve.CrouchCurve"));
	if (Crouch_Curve.Succeeded())
	{
		CrouchCurve = Crouch_Curve.Object;
		CrouchTimeline->AddInterpFloat(CrouchCurve, CrouchTimelineFunction);
		CrouchTimeline->SetTimelineFinishedFunc(CrouchTimelineFinish);
	}
	CrouchTimeline->SetLooping(false);

	// Set Prone timeline
	ProneTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ProneTimeline"));
	ProneTimelineFunction.BindUFunction(this, FName("ProneInterp"));
	ProneTimelineFinish.BindUFunction(this, FName("ProneFinish"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> Prone_Curve(TEXT("/Game/Blueprints/Character/Animation/ProneCurve.ProneCurve"));
	if (Crouch_Curve.Succeeded())
	{
		ProneCurve = Prone_Curve.Object;
		ProneTimeline->AddInterpFloat(ProneCurve, ProneTimelineFunction);
		ProneTimeline->SetTimelineFinishedFunc(ProneTimelineFinish);
	}
	ProneTimeline->SetLooping(false);
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
	PlayerInputComponent->BindAction(TEXT("ControlRotation"), IE_Pressed, this, &ANMCharacter::ControlRotationButtonPressed);
	PlayerInputComponent->BindAction(TEXT("ControlRotation"), IE_Released, this, &ANMCharacter::ControlRotationButtonReleased);

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
	UNMCharacterMovementComponent* CharacterMovementComponent = Cast<UNMCharacterMovementComponent>(GetCharacterMovement());
	if (!CharacterMovementComponent)
		return;

	switch (CharacterMovementComponent->GetCurrentCharacterMotion())
	{
	case ECharacterMotion::ECM_Stand:
		Super::Jump();
		break;
	case ECharacterMotion::ECM_Crouch:
		CrouchButtonPressed();
		break;
	case ECharacterMotion::ECM_Prone:
		ProneButtonPressed();
		break;
	default:
		break;
	}
}

void ANMCharacter::RunButtonPressed()
{
}

void ANMCharacter::RunButtonReleased()
{
}

void ANMCharacter::CrouchButtonPressed()
{
	if (UNMCharacterMovementComponent* CharacterMovementComponent = Cast<UNMCharacterMovementComponent>(GetCharacterMovement()))
	{
		if (HasAuthority())
		{			
			CharacterMovementComponent->DoCrouch();
		}
		else
		{
			ServerCrouchButtonPressed();
		}
	}
}

void ANMCharacter::ServerCrouchButtonPressed_Implementation()
{
	if (UNMCharacterMovementComponent* CharacterMovementComponent = Cast<UNMCharacterMovementComponent>(GetCharacterMovement()))
	{
		CharacterMovementComponent->DoCrouch();
	}
}

void ANMCharacter::ProneButtonPressed()
{
	if (UNMCharacterMovementComponent* CharacterMovementComponent = Cast<UNMCharacterMovementComponent>(GetCharacterMovement()))
	{
		if (HasAuthority())
		{			
			CharacterMovementComponent->DoProne();
		}
		else
		{
			ServerProneButtonPressed();
		}
	}
}

void ANMCharacter::ServerProneButtonPressed_Implementation()
{
	if (UNMCharacterMovementComponent* CharacterMovementComponent = Cast<UNMCharacterMovementComponent>(GetCharacterMovement()))
	{
			CharacterMovementComponent->DoProne();
	}
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

void ANMCharacter::ControlRotationButtonPressed()
{
	if (CameraRotTimeline->IsPlaying())
		CameraRotTimeline->Stop();

	ControlRotationBlocked = true;
	ControlRotation = GetControlRotation();
	bUseControllerRotationYaw = false;
}

void ANMCharacter::ControlRotationButtonReleased()
{
	ControlRotationBlocked = false;
	CameraRotTimeline->PlayFromStart();
}

void ANMCharacter::CameraRotInterp(float Value)
{
	FRotator CurrentRot = GetControlRotation();
	Controller->SetControlRotation(FMath::Lerp(CurrentRot, ControlRotation, Value));
}

void ANMCharacter::CameraRotFinish()
{
	bUseControllerRotationYaw = true;
}

void ANMCharacter::CrouchInterp_Implementation(float Value)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(StandHalfHeight, CrouchHalfHeight, Value));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, FMath::Lerp(-StandHalfHeight, -CrouchHalfHeight, Value)));
	CacheInitialMeshOffset(FVector(0.0f, 0.0f, FMath::Lerp(-StandHalfHeight, -CrouchHalfHeight, Value)), FRotator(0.0f, -90.0f, 0.0f));
	
}

void ANMCharacter::CrouchFinish_Implementation()
{
	if (UNMCharacterMovementComponent* CharacterMovementComponent = Cast<UNMCharacterMovementComponent>(GetCharacterMovement()))
	{
		CharacterMovementComponent->SetWalkSpeed();

		if (HasAuthority())
			CharacterMovementComponent->CrouchPlaying = false;
	}
}

void ANMCharacter::ProneInterp_Implementation(float Value)
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(StandHalfHeight, ProneHalfHeight, Value));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, FMath::Lerp(-StandHalfHeight, -ProneHalfHeight, Value)));
	CacheInitialMeshOffset(FVector(0.0f, 0.0f, FMath::Lerp(-StandHalfHeight, -ProneHalfHeight, Value)), FRotator(0.0f, -90.0f, 0.0f));
}

void ANMCharacter::ProneFinish_Implementation()
{
	if (UNMCharacterMovementComponent* CharacterMovementComponent = Cast<UNMCharacterMovementComponent>(GetCharacterMovement()))
	{
		CharacterMovementComponent->SetWalkSpeed();
		
		if (HasAuthority())
			CharacterMovementComponent->PronePlaying = false;
	}
}

bool ANMCharacter::CheckCanStand(float HalfHeight)
{
	float Height = StandHalfHeight * 2 - HalfHeight - Radius;
	FHitResult HitResult;
	FVector StartTrace = GetActorLocation();
	FVector EndTrace = (GetActorUpVector() * Height) + StartTrace;
	/*bool bResult = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartTrace,
		EndTrace,
		ECollisionChannel::ECC_Visibility
	);*/

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartTrace,
		EndTrace,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(Radius));


# if ENABLE_DRAW_DEBUG

	FVector Center = (StartTrace + EndTrace) / 2;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(GetActorUpVector()).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	/*DrawDebugLine(GetWorld(),
		StartTrace,
		EndTrace,
		DrawColor,
		false,
		5.0f
	);*/

	DrawDebugCapsule(GetWorld(),
		Center,
		Height * 0.5 + Radius,
		Radius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

# endif

	return bResult;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "NetBaseCharacter.h"

#include "NetGameInstance.h"
#include "NetPlayerState.h"

static UDataTable* SBodyParts = nullptr;


static FString GiveBodyPart(EBodyPart& Part)
{
	switch (Part)
	{
		case EBodyPart::BP_Face: return TEXT("Face");
		case EBodyPart::BP_Hair: return TEXT("Hair");
		case EBodyPart::BP_Chest: return TEXT("Chest");
		case EBodyPart::BP_Hands: return TEXT("Hands");
		case EBodyPart::BP_Legs: return TEXT("Legs");
		case EBodyPart::BP_Beard: return TEXT("Beard");
		default: return TEXT("Error");
	}
}
// Sets default values
ANetBaseCharacter::ANetBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PartFace = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	PartFace->SetupAttachment(GetMesh());
	PartFace->SetLeaderPoseComponent(GetMesh());

	PartChest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Chest"));
	PartChest->SetupAttachment(GetMesh());
	PartChest->SetLeaderPoseComponent(GetMesh());

	PartHands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	PartHands->SetupAttachment(GetMesh());
	PartHands->SetLeaderPoseComponent(GetMesh());

	PartLegs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	PartLegs->SetupAttachment(GetMesh());
	PartLegs->SetLeaderPoseComponent(GetMesh());

	PartHair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	PartHair->SetupAttachment(PartFace, FName("headSocket"));

	PartBeard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beard"));
	PartBeard->SetupAttachment(PartFace, FName("headSocket"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Eyes(TEXT("/Game/StylizedModularChar/Meshes/SM_Eyes"));

	PartEyes = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Eyes"));
	PartEyes->SetupAttachment(PartFace, FName("headSocket"));
	PartEyes->SetStaticMesh(SM_Eyes.Object);

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_BodyParts(TEXT("/Game/Tables/DT_BodyParts"));
	SBodyParts = DT_BodyParts.Object;
}

// Called when the game starts or when spawned
void ANetBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(GetNetMode() == ENetMode::NM_Standalone) return;
	SetActorHiddenInGame(true);
	CheckPlayerState();
}

void ANetBaseCharacter::SubmitPlayerInfoToServer_Implementation(FSPlayerInfo Info)
{
	ANetPlayerState *PState = GetPlayerState<ANetPlayerState>();
	PState->Data.NickName = Info.NickName;
	PState->Data.CustomizationData = Info.CustomizationData;
	PState->Data.TeamID = PState->TeamID;
	bPlayerInfoReceived = true;
}

// Called every frame
void ANetBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANetBaseCharacter::ChangeBodyPart(EBodyPart index, int Value, bool bDirectSet)
{
	FSMeshAssetList* List = GetBodyPartList(index, BodyPartIndices[(int)EBodyPart::BP_BodyType] != 0);
	if (List==nullptr) return;

	int CurrentIndex = BodyPartIndices[(int)index];

	CurrentIndex = (bDirectSet) ? Value : CurrentIndex + Value;

	int Num = List->ListSkeletal.Num() + List->ListStatic.Num();

	CurrentIndex = (CurrentIndex < 0) ? CurrentIndex + Num : CurrentIndex % Num;

	BodyPartIndices[(int)index] = CurrentIndex;

	switch (index)
	{
	case EBodyPart::BP_Face: PartFace->SetSkeletalMesh(List->ListSkeletal[CurrentIndex]); break;
	case EBodyPart::BP_Beard: PartBeard->SetStaticMesh(List->ListStatic[CurrentIndex]); break;
	case EBodyPart::BP_Chest: PartChest->SetSkeletalMesh(List->ListSkeletal[CurrentIndex]); break;
	case EBodyPart::BP_Hair: PartHair->SetStaticMesh(List->ListStatic[CurrentIndex]); break;
	case EBodyPart::BP_Hands: PartHands->SetSkeletalMesh(List->ListSkeletal[CurrentIndex]); break;
	case EBodyPart::BP_Legs: PartLegs->SetSkeletalMesh(List->ListSkeletal[CurrentIndex]); break;
	default: break;
	}
}

void ANetBaseCharacter::CheckPlayerState()
{
	ANetPlayerState *PState = GetPlayerState<ANetPlayerState>();

	if (PState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player state is null"));

		GWorld->GetTimerManager().SetTimer(ClientDataCheckTimer, this,
			&ANetBaseCharacter::CheckPlayerState, .25f, false);
	}
	else
	{
		if (IsLocallyControlled())
		{
			UNetGameInstance* GameInstance = Cast<UNetGameInstance>(GWorld->GetGameInstance());
			if(GameInstance) SubmitPlayerInfoToServer(GameInstance->PlayerInfo);
		}

		CheckPlayerInfo();
	}
}

void ANetBaseCharacter::CheckPlayerInfo()
{
	ANetPlayerState *PState = GetPlayerState<ANetPlayerState>();

	if (PState && bPlayerInfoReceived)
	{
		ParseCustomizationData(PState->Data.CustomizationData);
		UpdateBodyParts();
		OnPlayerInfoChanged();
		SetActorHiddenInGame(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player state not Recieved!"));

		GWorld->GetTimerManager().SetTimer(ClientDataCheckTimer, this,
			&ANetBaseCharacter::CheckPlayerInfo, .25f, false);
	}
}

FString ANetBaseCharacter::GetCustomizationData()
{
	FString Data;
	for (size_t i = 0; i < (int)EBodyPart::BP_Count; i++)
	{
		Data += FString::FromInt(BodyPartIndices[i]);
		if( i < ((int)(EBodyPart::BP_Count)-1)) Data += TEXT(",");
	}
	return Data;
}

void ANetBaseCharacter::ParseCustomizationData(FString BodyPartData)
{
	TArray<FString> ArrayData;
	BodyPartData.ParseIntoArray(ArrayData, TEXT(","));
	for (size_t i = 0; i < (int)ArrayData.Num(); i++)
	{
		BodyPartIndices[i] = FCString::Atoi(*ArrayData[i]);
	}
}

void ANetBaseCharacter::UpdateBodyParts()
{
	ChangeBodyPart(EBodyPart::BP_Face, 0, false);
	ChangeBodyPart(EBodyPart::BP_Beard, 0, false);
	ChangeBodyPart(EBodyPart::BP_Chest, 0, false);
	ChangeBodyPart(EBodyPart::BP_Hair, 0, false);
	ChangeBodyPart(EBodyPart::BP_Hands, 0, false);
	ChangeBodyPart(EBodyPart::BP_Legs, 0, false);
}

void ANetBaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ANetBaseCharacter, PartSelection);
}

void ANetBaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateBodyParts();
}

// Called to bind functionality to input
void ANetBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANetBaseCharacter::ChangeGender(bool bIsFemale)
{
	//PartSelection.bIsFemale = bIsFemale; idk what need to be chanced
	UpdateBodyParts();
}

FSMeshAssetList* ANetBaseCharacter::GetBodyPartList(EBodyPart Part, bool bIsFemale)
{
	FString Name = FString::Printf(TEXT("%s%s"), bIsFemale ? TEXT("Female") : TEXT("Male"), *GiveBodyPart(Part));
	return SBodyParts ? SBodyParts->FindRow<FSMeshAssetList>(*Name, nullptr) : nullptr;
}
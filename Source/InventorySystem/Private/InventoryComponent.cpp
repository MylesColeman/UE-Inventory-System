// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	MaxCarryWeight = 150.0f;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddItem(FName ItemID)
{
	if (!ItemID.IsNone() && ItemDataTable)
	{
		FItemData NewItemData;
		if (GetItemData(ItemID, NewItemData))
		{
			float NewTotalWeight = GetCurrentWeight() + NewItemData.ItemWeight;
			if (NewTotalWeight > MaxCarryWeight)
			{
				UE_LOG(LogTemp, Warning, TEXT("Cannot add item: %s max carry weight reached!"), *ItemID.ToString());
				return false;
			}
			ItemIDs.Add(ItemID);
			UE_LOG(LogTemp, Log, TEXT("Added Item: %s!"), *ItemID.ToString());
			return true;
		}
	}
	return false;
}

void UInventoryComponent::RemoveItem(FName ItemID)
{
	if (!ItemID.IsNone())
	{
		ItemIDs.RemoveSingle(ItemID);
		UE_LOG(LogTemp, Log, TEXT("Removed Item: %s!"), *ItemID.ToString())
	}
}

bool UInventoryComponent::GetItemData(FName ItemID, FItemData& OutItemData) const
{
	if (!ItemDataTable)
		return false;

	FItemData* Row = ItemDataTable->FindRow<FItemData>(ItemID, TEXT(""));
	if (Row)
	{
		OutItemData = *Row;
		return true;
	}

	return false;
}

float UInventoryComponent::GetCurrentWeight() const
{
	float TotalWeight = 0.0f;

	for (const FName& ID : ItemIDs)
	{
		FItemData ItemData;
		if (GetItemData(ID, ItemData))
			TotalWeight += ItemData.ItemWeight;
	}

	return TotalWeight;
}

void UInventoryComponent::DropLastItem()
{
	if (ItemIDs.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No items to drop, inventory is empty!"));
		return;
	}

	FName ItemIDToDrop = ItemIDs.Last();
	FItemData ItemData;

	if (GetItemData(ItemIDToDrop, ItemData))
	{
		AActor* Owner = GetOwner();
		if (Owner == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Character not found!"));
			return;
		}

		FVector Location = Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 100.f);
		FRotator Rotation = Owner->GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AItemClass* DroppedActor = GetWorld() ->SpawnActor<AItemClass>(ItemData.ItemActorClass, Location, Rotation, SpawnParams);

		if (DroppedActor)
		{
			DroppedActor->ItemID = ItemIDToDrop;
			if (ItemData.ItemMesh)
				DroppedActor->MyMesh->SetStaticMesh(ItemData.ItemMesh);
			
			UE_LOG(LogTemp, Warning, TEXT("Dropped Item: %s!"), *ItemIDToDrop.ToString());
			RemoveItem(ItemIDToDrop);
		}
	}
}
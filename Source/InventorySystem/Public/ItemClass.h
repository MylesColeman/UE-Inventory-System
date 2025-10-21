// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemClass.generated.h"


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category="Item Data")
	FText ItemName = FText::FromString("My Item");

	UPROPERTY(EditAnywhere, Category="Item Data")
	FText ItemDesc = FText::FromString("This is an item.");

	UPROPERTY(EditAnywhere, Category="Item Data")
	float ItemWeight = 1.f;

	UPROPERTY( EditAnywhere, Category="Item Data")
	float ItemValue = 1.f;
};
UCLASS(Blueprintable,BlueprintType)
class INVENTORYSYSTEM_API AItemClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemClass();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Data")
	FName ItemID;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MyMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

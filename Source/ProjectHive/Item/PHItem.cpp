// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/PHItem.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APHItem::APHItem()
{
	ItemCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ItemCollision"));
	SetRootComponent(ItemCollision);
}

void APHItem::ShowUI()
{

}

void APHItem::Interact()
{

}


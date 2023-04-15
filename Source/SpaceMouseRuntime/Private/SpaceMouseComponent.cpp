// Fill out your copyright notice in the Description page of Project Settings.

#include "SmInputDevice.h"
#include "SpaceMouseComponent.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PlayerController.h"
#include "Editor/EditorEngine.h"
#include "Engine/LocalPlayer.h"

// Sets default values for this component's properties
USpaceMouseComponent::USpaceMouseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpaceMouseComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void USpaceMouseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ULocalPlayer* const LocalPlayer = GEngine->GetLocalPlayerFromControllerId(GetWorld(), 0);
	auto tranformComponent =LocalPlayer->PlayerController->PlayerCameraManager->GetTransformComponent();

	auto CameraLocation = tranformComponent->K2_GetComponentLocation();
	auto CameraRotation = tranformComponent->K2_GetComponentRotation();
	FTransform DeltaTransformCameraFrame = FTransform(CameraRotation, CameraLocation);

	auto DeltaTranslation = FSmInputDevice::Manager->GetTranslation() * PositionScale;
	auto DeltaRotation = FSmInputDevice::Manager->GetRotation() * RotationScale;
	FTransform DeltaTranslationC = DeltaTransformCameraFrame.Inverse() * FTransform(DeltaTranslation) * DeltaTransformCameraFrame;
	FTransform DeltaRotationC = DeltaTransformCameraFrame.Inverse() * FTransform(DeltaRotation) * DeltaTransformCameraFrame;

	auto OwnerActor = GetOwner();
	auto OwnerTranform = OwnerActor->GetActorTransform();
	
	auto newRotation = FTransform(OwnerTranform.GetRotation()) * FTransform(DeltaRotationC.GetRotation());
	auto newLocation = FTransform(OwnerTranform.GetLocation()) * FTransform(DeltaTranslationC.GetLocation());

	OwnerActor->SetActorTransform(FTransform(newRotation.GetRotation(), newLocation.GetLocation()));


}


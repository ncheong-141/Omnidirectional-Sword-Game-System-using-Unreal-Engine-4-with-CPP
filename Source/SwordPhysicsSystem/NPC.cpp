// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Avatar.h"

// Sets default values
ANPC::ANPC(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

	proximitySphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Proximity Sphere"));
	proximitySphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	proximitySphere->SetSphereRadius(32.f);

	// Code to make ANMPC::proc() run when this proximity sphere overlaps another actor
	proximitySphere->OnComponentBeginOverlap.AddDynamic(this, &ANPC::proximityCheck);
}

// Note, although this was eclared in the header as NPC::Prox() it is now NPC::Prox_Implementaiton here
int ANPC::proximityCheck_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	//This is where our code will fo for what happens when there is an intersection

	// If the overlapped actor is not the player, you should just simply return from the funciton
	if (Cast < AAvatar >(OtherActor) == nullptr) {
		return -1;
	}

	return 0;
}


// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


// Copyright 2019 Seokjin Lee. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM()
enum class EWeaponState : uint8
{
	NeverDeployed, Deploying, Idle, Firing, Reloading, Holstering, Unequipped
};

UCLASS()
class AWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class USkeletalMeshComponent* Mesh;
	
public:	
	AWeapon();

private:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	void PostInitializeComponents() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDeploy();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHolster(AWeapon* To);

	virtual void FireP() {}
	virtual void FireR() {}
	virtual void AimP() {}
	virtual void AimR() {}
	virtual void Reload() {}

	virtual bool CanDeploy() const;
	virtual bool CanHolster() const;

	uint8 GetSlot() const { return Slot; }
	EWeaponState GetState() const { return State; }
	bool IsVisible() const;
	void SetVisibility(bool bNewVisibility) const;

	class AFpsCharacter* const Owner = nullptr;

protected:
	virtual void Deploy();
	virtual void Holster(AWeapon* To);

private:
	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	FText Description;

	UPROPERTY(EditAnywhere)
	float DeployTime;

	UPROPERTY(EditAnywhere)
	float HolsterTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	uint8 Slot;

	UPROPERTY(VisibleInstanceOnly, Replicated, Transient)
	EWeaponState State;

	FTimerHandle StateSetTimer;
};
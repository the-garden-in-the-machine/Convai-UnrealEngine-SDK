// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "StreamFboSequence.generated.h"

UCLASS()
class CONVAI_API AStreamFboSequence : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStreamFboSequence();
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rendering")
	class UTextureRenderTarget2D* RenderTarget;

	void CaptureImage(TArray<FColor>& OutBitmap, TArray<uint8>& OutCompressedBitmap);
	void CaptureAndSaveRenderTarget();//for debugin if image is actually saved
	void CaptureAndStoreImage();


	int32 FrameCounter;
	static const int32 CaptureInterval = 64;
	TArray<TArray<uint8>> StoredImages;

};

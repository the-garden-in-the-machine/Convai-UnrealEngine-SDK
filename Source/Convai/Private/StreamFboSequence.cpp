// Fill out your copyright notice in the Description page of Project Settings.


#include "StreamFboSequence.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Texture2D.h"
#include "Misc/FileHelper.h"
#include "ImageUtils.h"

// Sets default values
AStreamFboSequence::AStreamFboSequence()
{
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	RootComponent = SceneCaptureComponent;

	RenderTarget = nullptr; // Set this to your Render Target in the rendering  of the StreamFBO blueprint 
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
 	
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStreamFboSequence::BeginPlay()
{
	Super::BeginPlay();
	FrameCounter = 0;
	
}

// Called every frame
void AStreamFboSequence::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//count increment
	FrameCounter++;
	//UE_LOG(LogTemp, Warning, TEXT("FrameCounter: %d"), FrameCounter);

	if (FrameCounter >= CaptureInterval) {
		CaptureAndSaveRenderTarget();
		FrameCounter = 0;
		//CaptureAndStoreImage();
	}
}

//Capture image
void AStreamFboSequence::CaptureImage(TArray<FColor>& OutBitmap, TArray<uint8>& OutCompressedBitmap)
{
	if (!RenderTarget || !SceneCaptureComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("RenderTarget or SceneCaptureComponent is null."));
		return;
	}

	FTextureRenderTargetResource* RTResource = RenderTarget->GameThread_GetRenderTargetResource();
	if (!RTResource)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Render Target resource."));
		return;
	}

	RTResource->ReadPixels(OutBitmap);
	FImageUtils::CompressImageArray(RTResource->GetSizeX(), RTResource->GetSizeY(), OutBitmap, OutCompressedBitmap);
}


void AStreamFboSequence::CaptureAndStoreImage()
{
	TArray<FColor> Bitmap;
	TArray<uint8> CompressedBitmap;

	CaptureImage(Bitmap, CompressedBitmap);

	StoredImages.Add(MoveTemp(CompressedBitmap));

	UE_LOG(LogTemp, Warning, TEXT("Stored screenshot in memory. Total stored images: %d"), StoredImages.Num());
}
//for debuging 
void AStreamFboSequence::CaptureAndSaveRenderTarget()
{
	TArray<FColor> Bitmap;
	TArray<uint8> CompressedBitmap;

	CaptureImage(Bitmap, CompressedBitmap);

	FString BaseFilePath = FPaths::ProjectContentDir() + TEXT("CapturedImages/");
	FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*BaseFilePath);

	FString FileName = FString::Printf(TEXT("Capture_%d.png"), FPlatformTime::Seconds());
	FString FinalPath = FPaths::Combine(*BaseFilePath, *FileName);

	if (FFileHelper::SaveArrayToFile(CompressedBitmap, *FinalPath))
	{
		UE_LOG(LogTemp, Warning, TEXT(" from convai Saved screenshot to: %s"), *FinalPath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save screenshot to: %s"), *FinalPath);
	}
}

//dont think this will be needde if im on convai ?
//const TArray<TArray<uint8>>& AStreamFboSequence::StreamStoredImages() const
//{
//	return StoredImages;
//}
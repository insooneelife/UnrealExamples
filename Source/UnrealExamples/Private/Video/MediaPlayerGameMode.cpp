// Fill out your copyright notice in the Description page of Project Settings.


#include "Video/MediaPlayerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Misc/MediaBlueprintFunctionLibrary.h"

AMediaPlayerGameMode::AMediaPlayerGameMode()
{
	VideoRecorder = MakeShareable(new FVideoRecorder());
}


void AMediaPlayerGameMode::StartPlay()
{
	Super::StartPlay();

	if (MediaPlayer != nullptr)
	{

		OnVideoCaptureHandle = VideoRecorder->OnCapture.AddUObject(
			this, &AMediaPlayerGameMode::OnVideoCapture);


		VideoRecorder->StartRecording(MediaPlayer);

		MediaPlayer->OnMediaOpened.AddDynamic(this, &AMediaPlayerGameMode::OnMediaOpened);

		TArray<FMediaCaptureDevice> OutDevices;
		UMediaBlueprintFunctionLibrary::EnumerateVideoCaptureDevices(OutDevices);

		if (OutDevices.Num() > 0)
		{
			if (!OutDevices[0].Url.IsEmpty())
			{
				MediaPlayer->OpenUrl(OutDevices[0].Url);
			}
		}

		if (UUserWidget* CreatedWidget = CreateWidget(
			GetGameInstance(), CameraDebugWidgetBP, TEXT("CameraDebugWidgetBP")))
		{
			CreatedWidget->AddToViewport();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MediaPlayer is nullptr."));
	}
}

void AMediaPlayerGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	VideoRecorder->StopRecording();
	MediaPlayer->Close();
	MediaPlayer->OnMediaOpened.RemoveDynamic(this, &AMediaPlayerGameMode::OnMediaOpened);

	VideoRecorder->OnCapture.Remove(OnVideoCaptureHandle);

	Super::EndPlay(EndPlayReason);
}

void AMediaPlayerGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMediaPlayerGameMode::OnMediaOpened(FString InOpenedUrl)
{
	const int32 NV12_Frame30 = 321;
	int32 NumFormats = MediaPlayer->GetNumTrackFormats(EMediaPlayerTrack::Video, 0);

	if (MediaPlayer->SetTrackFormat(EMediaPlayerTrack::Video, 0, NV12_Frame30))
	{
		UE_LOG(LogTemp, Log, TEXT("SetTrackFormat  Success.  NumFormats : %d"), NumFormats);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SetTrackFormat  Failed.  NumFormats : %d"), NumFormats);
	}
}


void AMediaPlayerGameMode::OnVideoCapture(
	const uint8* InBuffer, int32 InNum, int32 InWidth, int32 InHeight, EMediaTextureSampleFormat InFormat)
{
	VideoBuffer.Reset();
	VideoBuffer.Append(InBuffer, InNum);

	VideoWidth = InWidth;
	VideoHeight = InHeight;
	VideoFormat = InFormat;
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Video/VideoRecorder.h"
#include "MediaPlayerGameMode.generated.h"

// https://docs.unrealengine.com/4.27/ko/WorkingWithMedia/IntegratingMedia/MediaFramework/HowTo/UsingWebCams/
UCLASS()
class UNREALEXAMPLES_API AMediaPlayerGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AMediaPlayerGameMode();

public:
	virtual void StartPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable)
		class UMediaPlayer* GetMediaPlayer() const { return MediaPlayer; }

public:
	UFUNCTION()
		void OnMediaOpened(FString InOpenedUrl);

	void OnVideoCapture(
		const uint8* InBuffer, int32 InNum, int32 InWidth, int32 InHeight, EMediaTextureSampleFormat InFormat);

private:
	UPROPERTY(EditAnywhere)
		class UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> CameraDebugWidgetBP;

	TSharedPtr<class FVideoRecorder> VideoRecorder;

	TArray<uint8> VideoBuffer;
	int32 VideoWidth;
	int32 VideoHeight;
	EMediaTextureSampleFormat VideoFormat;

	FDelegateHandle OnVideoCaptureHandle;
};


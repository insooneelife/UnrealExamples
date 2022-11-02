// Fill out your copyright notice in the Description page of Project Settings.


#include "Video/VideoRecorder.h"

FVideoRecorder::FVideoRecorder()
	:
	FrameCount(0),
	bRecording(false)
{
}



void FVideoRecorder::StartRecording(UMediaPlayer* InMediaPlayer)
{
	TSharedRef<FMediaPlayerFacade, ESPMode::ThreadSafe> PlayerFacade = InMediaPlayer->GetPlayerFacade();

	IMediaModule* MediaModule = FModuleManager::LoadModulePtr<IMediaModule>("Media");

	if (MediaModule == nullptr)
	{
		return;
	}

	ClockSink = MakeShared<FVideoRecorderClockSink, ESPMode::ThreadSafe>(*this);
	MediaModule->GetClock().AddSink(ClockSink.ToSharedRef());

	SampleQueue = MakeShared<FMediaTextureSampleQueue, ESPMode::ThreadSafe>();

	PlayerFacade->AddVideoSampleSink(SampleQueue.ToSharedRef());

	bRecording = true;
}

void FVideoRecorder::StopRecording()
{
	SampleQueue.Reset();
	ClockSink.Reset();
	bRecording = false;
}

void FVideoRecorder::TickRecording()
{
	SCOPE_CYCLE_COUNTER(STAT_TickRecording)
		;

	if (!bRecording)
		return;

	TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe> Sample;
	while (SampleQueue->Dequeue(Sample))
	{
		FIntPoint Size = Sample->GetDim();

		Stream.Reset();
		{
			SCOPE_CYCLE_COUNTER(STAT_Convert)
				;

			//VideoUtils::ConvertSampleYUY2ToBGRStream(Sample, BGRStream);

			int32 PixelSize = VideoUtils::GetPixelSize(Sample);
			const int64 NumberOfTexel = Size.Y * Size.X;
			const int64 BufferNum = NumberOfTexel * PixelSize;
			const uint8* Buffer = reinterpret_cast<const uint8*>(Sample->GetBuffer());
			Stream.Append(Buffer, BufferNum);

			//UE_LOG(LogTemp, Error, TEXT("TickRecording  PixelSize : %d  NumberOfTexel : %d  BufferNum : %d  ArrayNum : %d"), 
			//	PixelSize, NumberOfTexel, BufferNum, Stream.Num());
		}

		{
			SCOPE_CYCLE_COUNTER(STAT_Broadcast)
				;

			OnCapture.Broadcast(Stream.GetData(), Stream.Num(), Size.X, Size.Y, Sample->GetFormat());
		}



	}

}


void FVideoRecorder::SaveImage(TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe> InSample)
{
	TUniquePtr<TImagePixelData<FColor>> PixelData = VideoUtils::ConvertSampleYUY2ToFColor(InSample);
	EImageFormat TargetImageFormat = EImageFormat::PNG;
	int32 CompressionQuality = 0;
	FString DirectoryPath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("recorded"));
	FString Filename = FPaths::Combine(DirectoryPath, TEXT("test"));
	Filename = FString::Printf(TEXT("%s_%08d"), *Filename, FrameCount);
	IFileManager::Get().MakeDirectory(*DirectoryPath, true);
	VideoUtils::SaveImage(MoveTemp(PixelData), TargetImageFormat, CompressionQuality, Filename);
	FrameCount++;
}
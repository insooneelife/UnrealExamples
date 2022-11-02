// Fill out your copyright notice in the Description page of Project Settings.


#include "Video/VideoUtils.h"
#include "ImageWriteQueue.h"
#include "MediaTexture.h"

void VideoUtils::SaveImage(
	TUniquePtr<TImagePixelData<FColor>> PixelData,
	EImageFormat TargetImageFormat,
	int32 CompressionQuality,
	FString Filename)
{
	IImageWriteQueue* ImageWriteQueue = &FModuleManager::LoadModuleChecked<IImageWriteQueueModule>("ImageWriteQueue").GetWriteQueue();

	// save image
	TUniquePtr<FImageWriteTask> ImageTask = MakeUnique<FImageWriteTask>();
	ImageTask->PixelData = MoveTemp(PixelData);
	ImageTask->Format = TargetImageFormat;
	ImageTask->CompressionQuality = CompressionQuality;
	ImageTask->bOverwriteFile = true;
	ImageTask->Filename = Filename;
	ImageWriteQueue->Enqueue(MoveTemp(ImageTask), false);
}

TUniquePtr<TImagePixelData<FColor>> VideoUtils::ConvertSampleInterpretedAsFloat16ColorToFColor(
	TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe> InSample,
	const FIntPoint InSize)
{
	const int64 NumberOfTexel = InSize.Y * InSize.X;
	TUniquePtr<TImagePixelData<FColor>> PixelData = MakeUnique<TImagePixelData<FColor>>(InSize);
	PixelData->Pixels.Reset(NumberOfTexel);

	const FFloat16Color* Buffer = reinterpret_cast<const FFloat16Color*>(InSample->GetBuffer());
	for (int64 i = 0; i < NumberOfTexel; i++)
	{
		FColor Output;
		Output.R = (uint8)(FMath::Clamp<float>(Buffer[0].R, 0.0f, 1.0f) * 255);
		Output.G = (uint8)(FMath::Clamp<float>(Buffer[0].G, 0.0f, 1.0f) * 255);
		Output.B = (uint8)(FMath::Clamp<float>(Buffer[0].B, 0.0f, 1.0f) * 255);
		Output.A = (uint8)(FMath::Clamp<float>(Buffer[0].A, 0.0f, 1.0f) * 255);
		PixelData->Pixels.Add(Output);
		Buffer++;
	}

	return MoveTemp(PixelData);
}



TUniquePtr<TImagePixelData<FColor>> VideoUtils::ConvertSampleYUY2ToFColor(
	TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe> InSample)
{
	FIntPoint Size = InSample->GetOutputDim();
	const uint8* Buffer = reinterpret_cast<const uint8*>(InSample->GetBuffer());
	return ConvertYUY2ToFColor(Size, Buffer);
}

TUniquePtr<TImagePixelData<FColor>> VideoUtils::ConvertYUY2ToFColor(
	FIntPoint InSize, const uint8* InBuffer)
{
	const int64 NumberOfTexel = InSize.Y * InSize.X;
	TUniquePtr<TImagePixelData<FColor>> PixelData = MakeUnique<TImagePixelData<FColor>>(InSize);
	PixelData->Pixels.Reset(NumberOfTexel);

	for (int64 i = 0; i < NumberOfTexel / 2; i++)
	{
		int32 Y0 = InBuffer[0];
		int32 U0 = InBuffer[1];
		int32 Y1 = InBuffer[2];
		int32 V0 = InBuffer[3];

		FColor Color1, Color2;
		YUY2ToBGR(Y0, U0, Y1, V0, Color1, Color2);

		InBuffer = InBuffer + 4;

		PixelData->Pixels.Add(Color1);
		PixelData->Pixels.Add(Color2);
	}

	return MoveTemp(PixelData);
}

TUniquePtr<TImagePixelData<FColor>> VideoUtils::ConvertBGRToFColor(
	FIntPoint InSize, const uint8* InBuffer)
{
	const int64 NumberOfTexel = InSize.Y * InSize.X;
	TUniquePtr<TImagePixelData<FColor>> PixelData = MakeUnique<TImagePixelData<FColor>>(InSize);
	PixelData->Pixels.Reset(NumberOfTexel);

	for (int64 i = 0; i < NumberOfTexel; i++)
	{
		FColor Color;

		Color.B = InBuffer[0];
		Color.G = InBuffer[1];
		Color.R = InBuffer[2];

		InBuffer = InBuffer + 3;

		PixelData->Pixels.Add(Color);
	}

	return MoveTemp(PixelData);
}

void VideoUtils::ConvertSampleYUY2ToBGRStream(
	TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe> InSample, TArray<uint8>& OutArray)
{
	FIntPoint Size = InSample->GetOutputDim();
	const int64 NumberOfTexel = Size.Y * Size.X;
	const uint8* Buffer = reinterpret_cast<const uint8*>(InSample->GetBuffer());

	for (int64 i = 0; i < NumberOfTexel / 2; i++)
	{
		int32 Y0 = Buffer[0];
		int32 U0 = Buffer[1];
		int32 Y1 = Buffer[2];
		int32 V0 = Buffer[3];

		FColor Color1, Color2;
		YUY2ToBGR(Y0, U0, Y1, V0, Color1, Color2);

		Buffer = Buffer + 4;
		OutArray.Add(Color1.B);
		OutArray.Add(Color1.G);
		OutArray.Add(Color1.R);
		OutArray.Add(Color2.B);
		OutArray.Add(Color2.G);
		OutArray.Add(Color2.R);
	}
}


int32 VideoUtils::GetPixelSize(const TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe>& Sample)
{
	switch (Sample->GetFormat())
	{
	case EMediaTextureSampleFormat::CharAYUV:
	case EMediaTextureSampleFormat::CharBGRA:
	case EMediaTextureSampleFormat::CharBMP:
	case EMediaTextureSampleFormat::CharUYVY:
	case EMediaTextureSampleFormat::CharYUY2:
	case EMediaTextureSampleFormat::CharYVYU:
		//return PF_B8G8R8A8;
		return 4;

	case EMediaTextureSampleFormat::CharNV12:
	case EMediaTextureSampleFormat::CharNV21:
		//return PF_G8; 
		return 1;

	case EMediaTextureSampleFormat::FloatRGB:
		//return PF_FloatRGB;
		return 12;

	case EMediaTextureSampleFormat::FloatRGBA:
		//return PF_FloatRGBA;
		return 16;

	case EMediaTextureSampleFormat::CharBGR10A2:
		//return PF_A2B10G10R10;
		return 4;

	case EMediaTextureSampleFormat::YUVv210:
		//return PF_R32G32B32A32_UINT;
		return 16;

	case EMediaTextureSampleFormat::Y416:
		//return PF_A16B16G16R16;
		return 8;

	default:
		return 0;
	}
}

void VideoUtils::YUY2ToBGR(int32 Y0, int32 U0, int32 Y1, int32 V0, FColor& Color1, FColor& Color2)
{
	int32 C = Y0 - 16;
	int32 D = U0 - 128;
	int32 E = V0 - 128;

	Color1.B = FMath::Clamp<int32>((298 * C + 516 * D + 128) >> 8, 0, 255);
	Color1.G = FMath::Clamp<int32>((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
	Color1.R = FMath::Clamp<int32>((298 * C + 409 * E + 128) >> 8, 0, 255);
	Color1.A = 255;

	C = Y1 - 16;

	Color2.B = FMath::Clamp<int32>((298 * C + 516 * D + 128) >> 8, 0, 255);
	Color2.G = FMath::Clamp<int32>((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
	Color2.R = FMath::Clamp<int32>((298 * C + 409 * E + 128) >> 8, 0, 255);
	Color2.A = 255;
}
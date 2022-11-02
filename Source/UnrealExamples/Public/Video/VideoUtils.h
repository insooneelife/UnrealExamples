// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MediaTexture.h"
#include "ImagePixelData.h"
#include "ImageWriteBlueprintLibrary.h"

class IImageWriteQueue;
class IMediaTextureSample;

class UNREALEXAMPLES_API VideoUtils
{
public:

	static void SaveImage(
		TUniquePtr<TImagePixelData<FColor>> PixelData,
		EImageFormat TargetImageFormat,
		int32 CompressionQuality,
		FString Filename);

	template<class TColorType>
	static TUniquePtr<TImagePixelData<TColorType>> CreatePixelData(
		TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe> InSample,
		const FIntPoint InSize,
		int32 InNumChannels)
	{
		const int64 NumberOfTexel = InSize.Y * InSize.X;
		TUniquePtr<TImagePixelData<TColorType>> PixelData = MakeUnique<TImagePixelData<TColorType>>(InSize);
		PixelData->Pixels.Reset(NumberOfTexel);

		const void* Buffer = InSample->GetBuffer();
		const uint64 Stride = InSample->GetStride();
		if (NumberOfTexel == (Stride * InSize.Y) / InNumChannels)
		{
			PixelData->Pixels.Append(reinterpret_cast<const TColorType*>(Buffer), NumberOfTexel);
		}
		else
		{
			for (int64 IndexY = 0; IndexY < InSize.Y; ++IndexY)
			{
				PixelData->Pixels.Append(reinterpret_cast<const TColorType*>(reinterpret_cast<const uint8*>(Buffer) + (Stride * IndexY)), InSize.X);
			}
		}

		return MoveTemp(PixelData);
	};

	static TUniquePtr<TImagePixelData<FColor>> ConvertSampleInterpretedAsFloat16ColorToFColor(
		TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe> InSample,
		const FIntPoint InSize);

	static TUniquePtr<TImagePixelData<FColor>> ConvertSampleYUY2ToFColor(
		TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe> InSample);

	static TUniquePtr<TImagePixelData<FColor>> ConvertYUY2ToFColor(
		FIntPoint InSize, const uint8* InBuffer);

	static TUniquePtr<TImagePixelData<FColor>> ConvertBGRToFColor(
		FIntPoint InSize, const uint8* InBuffer);

	static void ConvertSampleYUY2ToBGRStream(
		TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe> InSample,
		TArray<uint8>& OutArray);


	static int32 GetPixelSize(const TSharedPtr<IMediaTextureSample, ESPMode::ThreadSafe>& Sample);

	static void YUY2ToBGR(int32 Y0, int32 U0, int32 Y1, int32 V0, FColor& Color1, FColor& Color2);


};
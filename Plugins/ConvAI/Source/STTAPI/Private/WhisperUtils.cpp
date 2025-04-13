#include "WhisperUtils.h"
#include "HttpModule.h"
#include "Http.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JsonObjectConverter.h"

TSharedRef<IHttpRequest> FWhisperUtils::CreateRequest(const TArray<uint8>& AudioBuffer)
{
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

    Request->SetURL(TEXT("http://localhost:5000/transcribe"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW"));

    FString Boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    FString BeginBoundary = "--" + Boundary + "\r\n";
    FString EndBoundary = "\r\n--" + Boundary + "--\r\n";

    FString Header = FString::Printf(TEXT(
        "Content-Disposition: form-data; name=\"audio\"; filename=\"audio.wav\"\r\n"
        "Content-Type: audio/wav\r\n\r\n"
    ));

    TArray<uint8> Payload;
    AppendStringToByteArray(BeginBoundary + Header, Payload);
    Payload.Append(AudioBuffer);
    AppendStringToByteArray(EndBoundary, Payload);

    Request->SetContent(Payload);

    return Request;
}

bool FWhisperUtils::ParseResponse(TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> Response, FWhisperResponse& OutResponse)
{
    FString ContentString = Response->GetContentAsString();
    if (FJsonObjectConverter::JsonObjectStringToUStruct<FWhisperResponse>(ContentString, &OutResponse, 0, 0))
    {
        return true;
    }

    return false;
}

void FWhisperUtils::AppendStringToByteArray(const FString& InString, TArray<uint8>& OutBytes)
{
    FTCHARToUTF8 Converter(*InString);
    OutBytes.Append(reinterpret_cast<const uint8*>(Converter.Get()), Converter.Length());
} 
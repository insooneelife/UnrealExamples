// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class UnrealExamples : ModuleRules
{
	public UnrealExamples(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core", "CoreUObject", "Engine", "InputCore", "Sockets", "Networking", "Json", "JsonUtilities", "HTTP", 
			"AudioCapture", "AudioCaptureCore", "AudioCaptureRtAudio", "UMG", "ConvAI", "LLMAPI", "TTSAPI", "STTAPI", "AudioMixer"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "Flatbuffers", "AudioCaptureEditor" });

		PrivateDependencyModuleNames.AddRange(new string[] { "JsonUtilities", "Json" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Media", "MediaAssets", "MediaUtils", "ImageWriteQueue" });

		
		PrivateIncludePaths.AddRange(new string[] {
			Path.GetFullPath(Path.Combine(EngineDirectory, "Source/Runtime/AudioCaptureImplementations/AudioCaptureRtAudio/Private"))
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

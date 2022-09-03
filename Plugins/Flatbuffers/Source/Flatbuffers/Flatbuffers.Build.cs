// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System.Diagnostics;

public class Flatbuffers : ModuleRules
{
	string ThirdPartyDir;
	string FlatbuffersDir;

	public Flatbuffers(ReadOnlyTargetRules Target) : base(Target)
	{
		ThirdPartyDir = Path.Combine(PluginDirectory, "Source", "ThirdParty");
		FlatbuffersDir = Path.Combine(ThirdPartyDir, "flatbuffers");

		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);


		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				Path.Combine(FlatbuffersDir, "include"),
				Path.Combine(FlatbuffersDir, "generated")

			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
				// ... add other public dependencies that you statically link with here ...
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		GenerateFiles();
	}

	public void GenerateFiles()
	{
		System.DateTime begin = System.DateTime.Now;

		string flatcPath = Path.Combine(FlatbuffersDir, "flatc.exe");
		string flatcOutDir = Path.Combine(FlatbuffersDir, "generated");
		string flatcInDir = Path.Combine(ThirdPartyDir, "localserve", "fbs");

		string[] fbsFiles = new string[]
		{
			"response.fbs", "stream.fbs"
		};

		string files = "";
		foreach (string file in fbsFiles)
		{
			files += $"\"{Path.Combine(flatcInDir, file)}\" ";
		}

		string arguments = $"-c -o \"{flatcOutDir}\" -I . {files}";
		System.Console.WriteLine($"cmd : \n{flatcPath} {arguments}");

		// Prepare the process to run
		ProcessStartInfo startInfo = new ProcessStartInfo();
		startInfo.Arguments = arguments;
		startInfo.FileName = flatcPath;
		startInfo.WindowStyle = ProcessWindowStyle.Hidden;
		startInfo.CreateNoWindow = true;

		// Run the external process & wait for it to finish
		using (Process proc = Process.Start(startInfo))
		{
			int exitCode;
			proc.WaitForExit();

			// Retrieve the app's exit code
			exitCode = proc.ExitCode;

			System.DateTime end = System.DateTime.Now;
			double seconds = (end - begin).TotalSeconds;

			System.Console.WriteLine($"GenerateFiles  exitCode : {exitCode}   seconds : {seconds}");
		}
	}
}

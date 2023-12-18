using UnrealBuildTool;
using System.IO;
using System.Diagnostics;
using System;

public class Memcached : ModuleRules
{
	public Memcached(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "Json", "JsonUtilities"
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
			}
			);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string ThirdPartyDir = Path.Combine(PluginDirectory, "Source", "ThirdParty");
			string IncPath = Path.Combine(ThirdPartyDir, "libmemcached", "include");
			string LibPath = Path.Combine(ThirdPartyDir, "libmemcached", "lib");
			string BinaryPath = Path.Combine(ThirdPartyDir, "libmemcached", "bin");
			string PluginDir = PluginDirectory;

			string[] LibNames = new string[]
			{
			"hashkit", "memcached", "memcachedprotocol", "memcachedutil", "p9y"
			};

			string[] DllNames = new string[]
			{
			"hashkit", "memcached", "memcachedprotocol", "memcachedutil"
			};

			PrivateIncludePaths.Add(IncPath);

			foreach (string Name in LibNames)
			{
				PublicAdditionalLibraries.Add(Path.Combine(LibPath, Name + ".lib"));
			}

			foreach (string Name in DllNames)
			{
				string DLLName = Name + ".dll";
				string DLLPath = Path.Combine(BinaryPath, DLLName);
				string DestinationDir = Path.Combine(PluginDir, "Binaries", Target.Platform.ToString());
				CopyToBinaries(DestinationDir, DLLPath);

				string DestinationFilePath = Path.Combine(DestinationDir, DLLName);
				// stage dll files to Plugins/Memcached/Binaries folder
				RuntimeDependencies.Add(DestinationFilePath, StagedFileType.NonUFS);

				// stage dll files to ProjectName/Binaries folder
				RuntimeDependencies.Add($"$(TargetOutputDir)/{DLLName}", DLLPath);
			}
		}

		
	}

	private void CopyToBinaries(string DestinationDir, string SourceFilePath)
	{
		string SourceFileName = Path.GetFileName(SourceFilePath);

		if (!Directory.Exists(DestinationDir))
			Directory.CreateDirectory(DestinationDir);

		string DestinationFilePath = Path.Combine(DestinationDir, SourceFileName);

		Console.WriteLine($"CopyToBinaries  DestinationDir : {DestinationDir}  SourceFilePath : {SourceFilePath}  DestinationFilePath : {DestinationFilePath}");

		if (!File.Exists(DestinationFilePath))  
			File.Copy(SourceFilePath, DestinationFilePath, true);
	}
}
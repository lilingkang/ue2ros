// Engine/Source/Runtime/WebBrowser/WebBrowser.Build.cs

using UnrealBuildTool;
using System.IO;

public class WebBrowserUI : ModuleRules
{
	public WebBrowserUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"ApplicationCore",
				"RHI",
				"InputCore",
				"Serialization",
				"HTTP"
			}
		);

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
				"Slate",
				"SlateCore"
            }
        );

		if (Target.Platform == UnrealTargetPlatform.Win64
		||  Target.Platform == UnrealTargetPlatform.Win32
		||  Target.Platform == UnrealTargetPlatform.Mac
		||  Target.Platform == UnrealTargetPlatform.Linux)
		{
			PrivateDependencyModuleNames.Add("CEF3Utils");
			AddEngineThirdPartyPrivateStaticDependencies(Target,
				"CEF3"
				);

			if (Target.Type != TargetType.Server)
			{
				if (Target.Platform == UnrealTargetPlatform.Mac)
				{
					// Add contents of UnrealCefSubProcess.app directory as runtime dependencies
					foreach (string FilePath in Directory.EnumerateFiles(Target.RelativeEnginePath + "/Binaries/Mac/UnrealCEFSubProcess.app", "*", SearchOption.AllDirectories))
					{
						RuntimeDependencies.Add(FilePath);
					}
				}
				else if (Target.Platform == UnrealTargetPlatform.Linux)
				{
					RuntimeDependencies.Add("$(EngineDir)/Binaries/" + Target.Platform.ToString() + "/UnrealCEFSubProcess");
				}
				else
				{
					RuntimeDependencies.Add("$(EngineDir)/Binaries/" + Target.Platform.ToString() + "/UnrealCEFSubProcess.exe");
				}
			}
		}
	}
}

namespace UnrealBuildTool.Rules
{
    using System.IO; // ToDo: Replace with standard mechenism
    using System.Diagnostics;

    public class PhantomController : ModuleRules
    {
        // This will give zou a relitive path to the module ../PhantomController/
        private string ModulePath 
        {
            get { return ModuleDirectory; }
        }

        // This will give you a relative path to ../PhantomController/ThirdParty/PhantomSDK
        private string MyLibraryPath
        {
            get { return Path.GetFullPath(Path.Combine(ModulePath, "ThirdParty", "PhantomSDK")); }
        }

        public PhantomController(TargetInfo Target)
        {
            PCHUsage = PCHUsageMode.NoSharedPCHs;

            // ... add public include paths required here ...
            PublicIncludePaths.AddRange(new string[] {
                "PhantomController/Public",
                "PhantomController/Classes",
            });

            // ... add other private include paths required here ...
            PrivateIncludePaths.AddRange(new string[] {
                "PhantomController/Private",
            });

            // ... add other public dependencies that you statically link with here ...
            PublicDependencyModuleNames.AddRange(new string[] { 
                "Core", 
                "CoreUObject",      // Provides Actors and Structs
                "Engine",           // Used by Actor
                "Slate",            // Used by InputDevice to fire bespoke FKey events
                "InputCore",        // Provides LOCTEXT and other Input features
                "InputDevice",      // Provides IInputInterface
            });

            // ... add private dependencies that you statically link with here ...
            PrivateDependencyModuleNames.AddRange(new string[] {
            });

            // ... add any modules that your module loads dynamically here ...
            DynamicallyLoadedModuleNames.AddRange(new string[] { 
            });

            LoadPhantomLibraries(Target);
        }

        /**
         * Adds the libs for the two phantom devices to the classpath
         **/
        public bool LoadPhantomLibraries(TargetInfo Target)
        {
            bool isLibrarySupported = false;

            //Right now only available for windows
            if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
            {
                isLibrarySupported = true;

                string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "Win32";

                string FinalLibPath = Path.Combine(MyLibraryPath, "lib", PlatformString, "ReleaseAcademicEdition");

                //First one
                PublicAdditionalLibraries.Add(Path.Combine(FinalLibPath, "hd.lib"));
                PublicDelayLoadDLLs.Add(Path.Combine(FinalLibPath, "hd.dll"));
                RuntimeDependencies.Add(new RuntimeDependency("$(EngineDir)/Binaries/ThirdParty/PhantomOmni/" + Target.Platform.ToString() + "/" + "hd.dll"));

                //Second one
                PublicAdditionalLibraries.Add(Path.Combine(FinalLibPath, "hl.lib"));
                PublicDelayLoadDLLs.Add(Path.Combine(FinalLibPath, "hl.dll"));
                RuntimeDependencies.Add(new RuntimeDependency("$(EngineDir)/Binaries/ThirdParty/PhantomOmni/" + Target.Platform.ToString() + "/" + "hl.dll"));

                //Add all the header files
                PrivateIncludePaths.Add(Path.Combine(MyLibraryPath,  "include", "HD"));
                PrivateIncludePaths.Add(Path.Combine(MyLibraryPath, "include", "HL"));
                PrivateIncludePaths.Add(Path.Combine(MyLibraryPath, "include", "HDU"));

            }

            Definitions.Add(string.Format("PHANTOM_LIB={0}", isLibrarySupported ? 1 : 0));

            Debug.WriteLine(isLibrarySupported);

            return isLibrarySupported;

            
        }
    }
}
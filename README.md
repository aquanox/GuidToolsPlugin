## GUID Tools Plugin

This plugin exposes several utility functions dealing with GUIDs to blueprints and a graph pin customization for `FGuid`.

This project is mostly a showcase on how to make your own pin type customizations.

Any parameter with meta specifier `UPARAM(meta=(GuidFormat=Digits))` or a function that has it will use custom pin visual.
Conditions can be viewed and changed in factory class.

Utility functions located in `UGuidToolsLibrary`:
- To String with format specifier (wrapper for `FGuid::ToString` with format)
- Parse with format specifier (wrapper for `FGuid::ParseExact`)
- Combine Guid / Combine Guid Array wrappers for `FGuid::Combine`
- Make Guid From Array (`byte`, `int`)
- Make Guid From String  (`FString`)
- Make Guid From Path (uses `FGuid::NewDeterministicGuid` and takes `FString`, `FSoftObjectPath`, `FSoftClassPath` input)

Pin Type Customization:
- `GuidToolsEditorModule` registers factory by calling `FEdGraphUtilities::RegisterVisualPinFactory`
- The `FGuidToolsPinFactory` implements `FGraphPanelPinFactory` that handles creation of `SGraphPin`s
- When conditions are met factory creates & returns a `SGraphPinGuid` widget that is displayed in graph editor

## Q & A

#### Q: Where can I find more about custom pins?

A: There are many pin type customizations in engine code, in your IDE (Rider, Visual Studio) navigate to `FGraphPanelPinFactory` and run `Show Hierarchy` or `Find Usages` for it.

#### Q: Why there is no Private/Public folders in Source?

A: By using `PublicIncludePaths.Add(ModuleDirectory)` in `ModuleName.Build.cs` you can opt out of it. 
All include paths will be relative to module directory and not Public/Classes/Private

#### Q: Why use of `TBaseStructure<FGuid>`?

A: Because FGuid is an intrinsic type, you can not use `StaticStruct<T>` to acquire `UScriptStruct` for it.

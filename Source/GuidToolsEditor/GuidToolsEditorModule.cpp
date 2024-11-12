// Copyright 2024, Aquanox.

#include "Modules/ModuleManager.h"
#include "EdGraphUtilities.h"
#include "GuidToolsPinFactory.h"

struct FGuidToolsEditorModule : public FDefaultModuleImpl
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TSharedPtr<FGuidToolsPinFactory> PinFactory;
};

IMPLEMENT_MODULE(FGuidToolsEditorModule, GuidToolsEditor);

void FGuidToolsEditorModule::StartupModule()
{
	PinFactory = MakeShared<FGuidToolsPinFactory>();
	FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);
}

void FGuidToolsEditorModule::ShutdownModule()
{
	FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
	PinFactory.Reset();
}

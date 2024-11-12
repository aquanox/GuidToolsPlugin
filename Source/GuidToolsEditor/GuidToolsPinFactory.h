// Copyright 2024, Aquanox.

#pragma once

#include "EdGraphUtilities.h"
#include "EdGraphSchema_K2.h"

class FGuidToolsPinFactory : public FGraphPanelPinFactory
{
	using Super = FGraphPanelPinFactory;
public:
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override;
};

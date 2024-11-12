// Copyright 2024, Aquanox.

#pragma once

#include "SGraphPin.h"
#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SWidget;
class UEdGraphPin;
class SEditableTextBox;

class SGraphPinGuid : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SGraphPinGuid) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;

};

// Copyright 2024, Aquanox.

#include "SGraphPinGuid.h"

#include "GuidToolsLibrary.h"
#include "K2Node_CallFunction.h"
#include "ScopedTransaction.h"
#include "SSimpleComboButton.h"
#include "Components/HorizontalBox.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SBox.h"

/**
 * The slate widget that will be displaying pin data
 */
class SGuidDefaultValueWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGuidDefaultValueWidget)
		{}
	SLATE_END_ARGS()

	void Construct(const FArguments& Arguments, UEdGraphPin* InGraphPinObj);

	FText GetTextValue() const;
	void OnTextChanged(const FText& NewText);
	void OnTextCommitted(const FText& NewText, ETextCommit::Type CommitInfo);
	void OnTextRefresh();
	
	/**
	 * Callback called when new pin value has to be set
	 */
	void OnSetValue(const FGuid& NewValue);
	
	/**
	 * Invalidate button handler
	 */
	void OnInvalidate();
	
	/**
	 * Generate button handler
	 */
	void OnGenerate();

	/**
	 * Determines display format type from pin data
	 */
	EGuidFormatType DetermineFormatType() const;

	/**
	 * Temporary changes displayed format type.
	 * It will be eventually reset to data specified in metadata
	 */
	void OnSetFormatType(EGuidFormatType Type);

	/**
	 * Set textbox error hint value or clear it
	 */
	void SetTextError(const FText& InErrorMsg);

	/**
	 * Should textbox be in read-only mode?
	 */
	bool IsTextReadOnly() const;
private:
	FEdGraphPinReference GraphPinObjRef;
	
	TSharedPtr<SEditableTextBox> TextWidget;
	
	EGuidFormatType FormatType = EGuidFormatType::Digits;
};

void SGraphPinGuid::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

TSharedRef<SWidget> SGraphPinGuid::GetDefaultValueWidget()
{
	return SNew(SGuidDefaultValueWidget, GraphPinObj);
}

void SGuidDefaultValueWidget::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	GraphPinObjRef = InGraphPinObj;
	FormatType = DetermineFormatType();

	// a simple horizontal box consisting of textbox and menu combobox
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
			.MinDesiredWidth(32)
			[
				SAssignNew(TextWidget, SEditableTextBox)
				.Text(this, &SGuidDefaultValueWidget::GetTextValue)
				.Style(FAppStyle::Get(), "Graph.EditableTextBox")
				.ForegroundColor(FSlateColor::UseForeground())
				.SelectAllTextWhenFocused(true)
				.ClearKeyboardFocusOnCommit(false)
				.OnTextChanged(this, &SGuidDefaultValueWidget::OnTextChanged)
				.OnTextCommitted(this, &SGuidDefaultValueWidget::OnTextCommitted)
				.SelectAllTextOnCommit(true)
				.IsReadOnly(this, &SGuidDefaultValueWidget::IsTextReadOnly)
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SSimpleComboButton)
			.Icon(FAppStyle::Get().GetBrush("DetailsView.ViewOptions"))
			.OnGetMenuContent_Lambda([this]()
			{
				FMenuBuilder Menu(true, nullptr);

				// Construct submenu for display format
				// takind items from reflection data for enum 
				Menu.AddSubMenu(
					INVTEXT("Display Format"),
					INVTEXT("Temporary changes Display Format used to display GUID"),
					FNewMenuDelegate::CreateLambda([this](FMenuBuilder& Menu)
					{
						const UEnum* EnumData = StaticEnum<EGuidFormatType>();
						//NumEnums() - 1, because the last item in an enum is the _MAX item
						for(int Index = 0; Index < EnumData->NumEnums() - 1; ++Index)
						{
							EGuidFormatType Format = static_cast<EGuidFormatType>(EnumData->GetValueByIndex(Index));
							
							Menu.AddMenuEntry(
								EnumData->GetDisplayNameTextByIndex(Index),
								EnumData->GetToolTipTextByIndex(Index),
								FSlateIcon(),
								FUIAction(
                                	FExecuteAction::CreateSP(this, &SGuidDefaultValueWidget::OnSetFormatType, Format)
                                )
							);
						}
					}),
					false, FSlateIcon(), true, NAME_None
				);
				
				Menu.AddMenuEntry(
					INVTEXT("Invalidate"),
					INVTEXT("Invalidate GUID"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateSP(this, &SGuidDefaultValueWidget::OnInvalidate)
					),
					NAME_None,
					EUserInterfaceActionType::Button
				);
				Menu.AddMenuEntry(
					INVTEXT("New GUID"),
					INVTEXT("Generate new random GUID"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateSP(this, &SGuidDefaultValueWidget::OnGenerate)
					),
					NAME_None,
					EUserInterfaceActionType::Button
				);
				return Menu.MakeWidget();
			})
		]
	];
}

FText SGuidDefaultValueWidget::GetTextValue() const
{
	FGuid Value;
	if (const UEdGraphPin* GraphPinObj = GraphPinObjRef.Get())
	{
		// acquire internal string representation of type
		FString PinValue = GraphPinObj->GetDefaultAsString();
		// decode internal representation into actual value object
		const UScriptStruct* Struct = TBaseStructure<FGuid>::Get();
		Struct->ImportText(*PinValue, &Value, nullptr, EPropertyPortFlags::PPF_None, GLog, Struct->GetName());
	}
	// format for display in textbox
	return FText::FromString(Value.ToString(static_cast<EGuidFormats>(FormatType)));
}

void SGuidDefaultValueWidget::OnTextChanged(const FText& NewText)
{
	FText TextErrorMsg;

	FGuid LocalValue;
	if (!FGuid::ParseExact(NewText.ToString(), static_cast<EGuidFormats>(FormatType), LocalValue))
	{
		TextErrorMsg = INVTEXT("Invalid GUID format");
	}

	// Update or clear the error message
	SetTextError(TextErrorMsg);
}

void SGuidDefaultValueWidget::OnTextCommitted(const FText& NewTypeInValue, ETextCommit::Type CommitInfo)
{
	FGuid LocalValue;
	// parse from display format to value
	if (FGuid::ParseExact(NewTypeInValue.ToString(), static_cast<EGuidFormats>(FormatType), LocalValue))
	{
		OnSetValue(LocalValue);
	}
}

void SGuidDefaultValueWidget::OnTextRefresh()
{
	
}

void SGuidDefaultValueWidget::OnSetValue(const FGuid& NewValue)
{
	if (UEdGraphPin* GraphPinObj = GraphPinObjRef.Get())
	{
		FString NewValueString;
		// encode value into internal representation format
		const UScriptStruct* Struct = TBaseStructure<FGuid>::Get();
		Struct->ExportText(NewValueString, &NewValue, nullptr, nullptr, EPropertyPortFlags::PPF_None, nullptr);
		//  check for modification (optional) and assign internal value
		if (!GraphPinObj->GetDefaultAsString().Equals(NewValueString))
		{
			const FScopedTransaction Transaction(INVTEXT("Change String Pin Value"));
			GraphPinObj->Modify();

			GraphPinObj->GetSchema()->TrySetDefaultValue(*GraphPinObj, NewValueString);
		}
	}
}

void SGuidDefaultValueWidget::OnInvalidate()
{
	OnSetValue(FGuid());
}

void SGuidDefaultValueWidget::OnGenerate()
{
	OnSetValue(FGuid::NewGuid());
}

EGuidFormatType SGuidDefaultValueWidget::DetermineFormatType() const
{
	EGuidFormatType Result = EGuidFormatType::Digits;
	if (UEdGraphPin* GraphPinObj = GraphPinObjRef.Get())
	{
		if (UK2Node_CallFunction* Node = Cast<UK2Node_CallFunction>(GraphPinObj->GetOwningNode()))
		{
			FString FormatString = Node->GetPinMetaData(GraphPinObj->PinName, FName("GuidFormat"));
			int64 FormatValue = StaticEnum<EGuidFormatType>()->GetValueByNameString(FormatString);
			if (FormatValue != INDEX_NONE)
			{
				Result = static_cast<EGuidFormatType>(FormatValue);
			}
		}
	}
	return Result;
}

void SGuidDefaultValueWidget::OnSetFormatType(EGuidFormatType Type)
{
	FormatType  = Type;
#ifdef TRY_PERSIST_METADATA
	FString Value = StaticEnum<EGuidFormatType>()->GetNameStringByValue(static_cast<int64>(Type));
	
	if (UEdGraphPin* GraphPinObj = GraphPinObjRef.Get())
	{
		if (UK2Node_CallFunction* Node = Cast<UK2Node_CallFunction>(GraphPinObj->GetOwningNode()))
		{
			if (UFunction* Function = Node->GetTargetFunction())
			{
                if (FProperty* Property = Function->FindPropertyByName(GraphPinObj->PinName))
                {
                	Property->SetMetaData(FName("GuidFormat"), *Value);
                }
			}
		}
	}
#endif
}

void SGuidDefaultValueWidget::SetTextError(const FText& InErrorMsg)
{
	if (TextWidget.IsValid())
	{
		TextWidget->SetError(InErrorMsg);
	}
}

bool SGuidDefaultValueWidget::IsTextReadOnly() const
{
	if (UEdGraphPin* GraphPinObj = GraphPinObjRef.Get())
	{
		return GraphPinObj->bDefaultValueIsReadOnly;
	}
	return false;
}

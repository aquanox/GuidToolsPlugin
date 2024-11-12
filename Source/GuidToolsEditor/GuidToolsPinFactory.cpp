// Copyright 2024, Aquanox.

#include "GuidToolsPinFactory.h"

#include "K2Node_CallFunction.h"
#include "SGraphPinGuid.h"

TSharedPtr<SGraphPin> FGuidToolsPinFactory::CreatePin(UEdGraphPin* InPin) const
{
	static const FName Name_Guid("Guid");
	static const FName Name_GuidFormat("GuidFormat");
	static const FName Name_MakeLiteralGuid("MakeLiteralGuid");
	
	if (InPin->Direction == EEdGraphPinDirection::EGPD_Input &&
		InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
	{
		if (UScriptStruct* PinStructType = Cast<UScriptStruct>(InPin->PinType.PinSubCategoryObject.Get()))
		{
			if (PinStructType == TBaseStructure<FGuid>::Get())
			{
#ifdef ALLOW_ANY_GUID_PIN
				return SNew(SGraphPinGuid, InPin);
#else
				// only function nodes taking guid pin
				if (UK2Node_CallFunction* CallNode = Cast<UK2Node_CallFunction>(InPin->GetOuter()))
				{
					if (!CallNode->GetPinMetaData(InPin->GetFName(), Name_GuidFormat).IsEmpty())
					{ // pin with formatter meta
						return SNew(SGraphPinGuid, InPin);
					}
					//if (CallNode->FunctionReference.GetMemberName() == Name_MakeLiteralGuid)
					//{ // library function
					//	return SNew(SGraphPinGuid, InPin);
					//}
				}
#endif
			}
		}
		
	}
	return nullptr;
}

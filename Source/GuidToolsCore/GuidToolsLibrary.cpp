// Copyright 2024, Aquanox.

#include "GuidToolsLibrary.h"

FGuid UGuidToolsLibrary::CombineGuid(const FGuid& First, const FGuid& Second)
{
	return FGuid::Combine(First, Second);
}

FGuid UGuidToolsLibrary::CombineGuidArray(const TArray<FGuid>& InGuids)
{
	FGuid Result;
	if (InGuids.Num())
	{
		Result = InGuids[0];
		for(int32 Idx = 1; Idx < InGuids.Num(); ++Idx)
		{
			Result = FGuid::Combine(Result, InGuids[Idx]);
		}
	}
	return Result;
}

bool UGuidToolsLibrary::MakeGuid_FromByteArray(const TArray<uint8>& InValue, FGuid& OutGuid)
{
	const uint32 DataSizeExpected = 4 * sizeof(uint32);
	
	if (InValue.Num() != DataSizeExpected)
	{
		return false;
	}

	const uint32* Data = reinterpret_cast<const uint32*>(InValue.GetData());
	OutGuid = FGuid(Data[0], Data[1], Data[2], Data[3]);
	return true;
}

bool UGuidToolsLibrary::MakeGuid_FromIntArray(const TArray<int32>& InValue, FGuid& OutGuid)
{
	const uint32 DataSizeExpected = 4;
	
	if (InValue.Num() != DataSizeExpected)
	{
		return false;
	}

	OutGuid = FGuid(static_cast<uint32>(InValue[0]),
				 static_cast<uint32>(InValue[1]),
				 static_cast<uint32>(InValue[2]),
				 static_cast<uint32>(InValue[3])
	);
	return true;
}

FGuid UGuidToolsLibrary::MakeGuid_FromPackagePath(const FString& InPath)
{
	return FGuid::NewDeterministicGuid(InPath);
}

FGuid UGuidToolsLibrary::MakeGuid_FromObjectPath(const FSoftObjectPath& InPath)
{
	return FGuid::NewDeterministicGuid(InPath.ToString());
}

FGuid UGuidToolsLibrary::MakeGuid_FromClassPath(const FSoftClassPath& InPath)
{
	return FGuid::NewDeterministicGuid(InPath.ToString());
}

FGuid UGuidToolsLibrary::MakeGuid_FromString(FString Value)
{
	return FGuid(Value);
}

FGuid UGuidToolsLibrary::MakeLiteralGuid(FGuid Value)
{
	return Value;
}

FString UGuidToolsLibrary::Conv_GuidToStringFormat(const FGuid& InGuid, EGuidFormatType Format)
{
	return InGuid.ToString(static_cast<EGuidFormats>(Format));
}

void UGuidToolsLibrary::TryParse_StringFormatToGuid(const FString& GuidString, EGuidFormatType Format, FGuid& OutGuid, bool& Success)
{
	Success = FGuid::ParseExact(GuidString, static_cast<EGuidFormats>(Format), OutGuid);
}

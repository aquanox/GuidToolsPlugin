// Copyright 2024, Aquanox.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "GuidToolsLibrary.generated.h"

/**
 * 
 */
UENUM()
enum class EGuidFormatType : uint8
{
	/**
	 * 32 digits.
	 *
	 * For example: "00000000000000000000000000000000"
	 */
	Digits = EGuidFormats::Digits,

	/**
	 * 32 digits in lowercase
	 *
	 * For example: "0123abc456def789abcd123ef4a5b6c7"
	 */
	DigitsLower = EGuidFormats::DigitsLower,

	/**
	 * 32 digits separated by hyphens.
	 *
	 * For example: 00000000-0000-0000-0000-000000000000
	 */
	DigitsWithHyphens = EGuidFormats::DigitsWithHyphens,

	/**
	 * 32 digits separated by hyphens, in lowercase as described by RFC 4122.
	 *
	 * For example: bd048ce3-358b-46c5-8cee-627c719418f8
	 */
	DigitsWithHyphensLower = EGuidFormats::DigitsWithHyphensLower,

	/**
	 * 32 digits separated by hyphens and enclosed in braces.
	 *
	 * For example: {00000000-0000-0000-0000-000000000000}
	 */
	DigitsWithHyphensInBraces = EGuidFormats::DigitsWithHyphensInBraces,

	/**
	 * 32 digits separated by hyphens and enclosed in parentheses.
	 *
	 * For example: (00000000-0000-0000-0000-000000000000)
	 */
	DigitsWithHyphensInParentheses = EGuidFormats::DigitsWithHyphensInParentheses,

	/**
	 * Comma-separated hexadecimal values enclosed in braces.
	 *
	 * For example: {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}
	 */
	HexValuesInBraces = EGuidFormats::HexValuesInBraces,

	/**
	 * This format is currently used by the FUniqueObjectGuid class.
	 *
	 * For example: 00000000-00000000-00000000-00000000
	*/
	UniqueObjectGuid = EGuidFormats::UniqueObjectGuid,

	/**
	 * Base64 characters with dashes and underscores instead of pluses and slashes (respectively)
	 *
	 * For example: AQsMCQ0PAAUKCgQEBAgADQ
	*/
	Short = EGuidFormats::Short,

	/**
	 * Base-36 encoded, compatible with case-insensitive OS file systems (such as Windows).
	 *
	 * For example: 1DPF6ARFCM4XH5RMWPU8TGR0J
	 */
	Base36Encoded = EGuidFormats::Base36Encoded
};

UCLASS(MinimalAPI)
class UGuidToolsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Returns a GUID which is a combinationof the two provided ones.
	 *
	 * @return The combined GUID.
	 */
	UFUNCTION(BlueprintPure, Category="Guid", meta=(DisplayName="Combine Guid", BlueprintThreadSafe))
	static FGuid CombineGuid(const FGuid& First, const FGuid& Second);
	
	/**
	 * Returns a GUID which is a combinationof the provided ones.
	 *
	 * @return The combined GUID.
	 */
	UFUNCTION(BlueprintPure, Category="Guid", meta=(DisplayName="Combine Guid Array", BlueprintThreadSafe))
	static FGuid CombineGuidArray(const TArray<FGuid>& InGuids);
	
	/**
	 * Create a guid from given byte array.
	 * @param InValue 
	 * @return A new GUID.
	 */
	UFUNCTION(BlueprintPure, Category="Guid", meta=(DisplayName="Make Guid From Array (byte)", BlueprintThreadSafe))
	static bool MakeGuid_FromByteArray(const TArray<uint8>& InValue, FGuid& OutGuid);

	/**
	 * Create a guid from given int32 array
	 * @param InValue 
	 * @return A new GUID.
	 */
	UFUNCTION(BlueprintPure, Category="Guid", meta=(DisplayName="Make Guid From Array (int)", BlueprintThreadSafe))
	static bool MakeGuid_FromIntArray(const TArray<int32>& InValue, FGuid& OutGuid);

	/**
	 * Create a guid by hashing the given path
	 * @return A new GUID.
	 */
	UFUNCTION(BlueprintPure, Category="Guid", meta=(DisplayName="Make Guid From Path", BlueprintThreadSafe))
	static FGuid MakeGuid_FromPackagePath(const FString& InPath);
	
	/**
	 * Create a guid by hashing the given path
	 * @return A new GUID.
	 */
	UFUNCTION(BlueprintPure, Category="Guid", meta=(DisplayName="Make Guid From Object Path", BlueprintThreadSafe))
	static FGuid MakeGuid_FromObjectPath(const FSoftObjectPath& InPath);

	/**
	 * Create a guid by hashing the given path
	 * @return A new GUID.
	 */
	UFUNCTION(BlueprintPure, Category="Guid", meta=(DisplayName="Make Guid From Class Path", BlueprintThreadSafe))
	static FGuid MakeGuid_FromClassPath(const FSoftClassPath& InPath);

	/**
	 * Create a guid 
	 * @return A new GUID.
	 */
	UFUNCTION(BlueprintPure, Category="Guid", meta=(DisplayName="Make Literal Guid", BlueprintThreadSafe))
	static FGuid MakeLiteralGuid(UPARAM(meta=(GuidFormat=Digits)) FGuid Value);
	
	/**
     * Create a guid from input string
     * @return A new GUID.
     */
    UFUNCTION(BlueprintPure, Category="Guid", meta=(DisplayName="Make Guid From String", BlueprintThreadSafe))
    static FGuid MakeGuid_FromString(FString Value);
	
	/**
	 * Converts this GUID to its string representation.
	 *
	 * @param Format The string format to use.
	 * @return The string representation.
	 */
	UFUNCTION(BlueprintPure, Category="Guid", meta=(DisplayName="To Format String (Guid)", BlueprintThreadSafe))
	static FString Conv_GuidToStringFormat(const FGuid& InGuid, EGuidFormatType Format);

	/**
	 * Converts a string with the specified format to a GUID.
	 *
	 * @param GuidString The string to convert.
	 * @param Format The string format to parse.
	 * @param OutGuid Will contain the parsed GUID.
	 * @param Success true if the string was converted successfully, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category="Guid", meta=(DisplayName="Parse String Format to Guid", BlueprintThreadSafe))
	static void TryParse_StringFormatToGuid(const FString& GuidString, EGuidFormatType Format, FGuid& OutGuid, bool& Success);
};

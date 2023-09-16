/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Types/WSEnums.h"
#include "Engine/Font.h"
#include "WSTypography.generated.h"

/**
 * Struct with font weight, size, and letter spacing
 */
USTRUCT(BlueprintType)
struct FFontStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	EFontWeight Weight = EFontWeight::Regular;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	int32 Size = 12;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	int32 LetterSpacing = 0;
};

/**
 * Data asset that contains the current theme font and its styling
 */
UCLASS(BlueprintType)
class WIDGETSTUDIORUNTIME_API UWidgetStudioTypography : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	UFont* Typeface;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle H1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle H2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle H3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle H4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle H5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle H6;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle Subtitle1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle Subtitle2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle Body1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle Body2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle Button;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle Caption;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Typography")
	FFontStyle Overline;
};



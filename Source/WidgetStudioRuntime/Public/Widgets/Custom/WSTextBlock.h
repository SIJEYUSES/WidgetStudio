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

#include "Components/TextBlock.h"
#include "WSTextBlock.generated.h"

/**
 * A customized TextBlock widget that exposes required functionality
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioTextBlock : public UTextBlock
{
	GENERATED_BODY()

public:
	
	/**
	 * Wrap text at specific length.
	 * Note: This function is just exposing an unexposed UTextBlock property.
	 * @param InWrapTextAt The length to wrap the text at.
	 */
	UFUNCTION(BlueprintCallable, Category = "Text Block") void SetWrapTextAt(float InWrapTextAt);


	/**
	 * Change how the text is wrapped.
	 * Note: This function is just exposing an unexposed UTextBlock property.
	 * @param InWrappingPolicy The new policy to wrap the text at.
	 */
	UFUNCTION(BlueprintCallable, Category = "Text Block") void SetWrappingPolicy(ETextWrappingPolicy InWrappingPolicy);
	
};

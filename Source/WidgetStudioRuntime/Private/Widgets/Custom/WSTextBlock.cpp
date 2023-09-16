/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#include "Widgets/Custom/WSTextBlock.h"
#include "Widgets/Text/STextBlock.h"

void UWidgetStudioTextBlock::SetWrapTextAt(const float InWrapTextAt)
{
	WrapTextAt = InWrapTextAt;
	if (MyTextBlock)
	{
		MyTextBlock->SetWrapTextAt(InWrapTextAt);
	}
}

void UWidgetStudioTextBlock::SetWrappingPolicy(const ETextWrappingPolicy InWrappingPolicy)
{
	WrappingPolicy = InWrappingPolicy;
	if (MyTextBlock)
	{
		MyTextBlock->SetWrappingPolicy(InWrappingPolicy);
	}
}
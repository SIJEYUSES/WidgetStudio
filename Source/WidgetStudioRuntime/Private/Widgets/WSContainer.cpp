/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#include "Widgets/WSContainer.h"

void UWidgetStudioContainer::ConstructOption(FButtonOptions Option)
{
	// Override in child class
}

void UWidgetStudioContainer::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UWidgetStudioContainer::IndividualHoverStateChanged(UWidgetStudioBase* CallingWidget, const bool bIsHovering)
{
	OnIndividualHoverStateChanged.Broadcast(CallingWidget, bIsHovering);
}

int32 UWidgetStudioContainer::GetCurrentIndex() const
{
	return CurrentIndex;
}

int32 UWidgetStudioContainer::GetOptionCount() const
{
	return Options.Num();
}

TArray<FButtonOptions> UWidgetStudioContainer::GetOptions() const
{
	return Options;
}

FButtonOptions UWidgetStudioContainer::GetOptionAtIndex(const int32 Index)
{
	if (Options.IsValidIndex(Index))
	{
		return Options[Index];
	}

	return FButtonOptions();
}

FButtonOptions UWidgetStudioContainer::GetCurrentOption() const
{
	if (Options.IsValidIndex(CurrentIndex))
	{
		return Options[CurrentIndex];
	}
	return FButtonOptions();
}

int32 UWidgetStudioContainer::GetOptionIndexViaText(const FText InText)
{
	for (int32 i = 0; i < Options.Num(); i++)
	{
		if (Options[i].Text.ToString().ToLower() == InText.ToString().ToLower())
		{
			return i;
		}
	}

	return -1;
}

void UWidgetStudioContainer::IncrementCurrentIndex(const bool bBroadcast)
{
	SetCurrentIndex(FMath::Clamp(CurrentIndex + 1, 0, Options.Num()), bBroadcast);
}

void UWidgetStudioContainer::DecrementCurrentIndex(const bool bBroadcast)
{
	SetCurrentIndex(FMath::Clamp(CurrentIndex - 1, 0, Options.Num()), bBroadcast);
}

void UWidgetStudioContainer::ClearCurrentIndex(const bool bBroadcast)
{
	SetCurrentIndex(-1, bBroadcast);
}

void UWidgetStudioContainer::SetOptions(const TArray<FButtonOptions> NewOptions)
{
	ClearOptions();
	Options = NewOptions;

	for (int32 i = 0; i < Options.Num(); i++)
	{
		ConstructOption(Options[i]);
	}
}

void UWidgetStudioContainer::SetCurrentIndex(const int32 Index, const bool bBroadcast)
{
	if (Index > -1 && Index < Options.Num())
	{
		CurrentIndex = Index;
		if (bBroadcast)
		{
			OnCurrentIndexChanged.Broadcast(CurrentIndex, Options[CurrentIndex]);
		}
	}
	else
	{
		CurrentIndex = -1;
		if (bBroadcast)
		{
			OnCurrentIndexChanged.Broadcast(CurrentIndex, FButtonOptions());
		}
	}
}

void UWidgetStudioContainer::AddOption(const FButtonOptions NewOption)
{
	Options.Add(NewOption);
	ConstructOption(NewOption);
}

void UWidgetStudioContainer::ClearOptions()
{
	Options.Empty();
	CurrentIndex = -1;
}

bool UWidgetStudioContainer::SetCurrentIndexViaOptionText(const FText InText, const bool bBroadcast)
{
	for (int32 i = 0; i < Options.Num(); i++)
	{
		if (Options[i].Text.ToString().ToLower() == InText.ToString().ToLower())
		{
			SetCurrentIndex(i, bBroadcast);
			return true;
		}
	}
	return false;
}

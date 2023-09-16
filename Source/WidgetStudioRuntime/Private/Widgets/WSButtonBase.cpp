/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/WSButtonBase.h"
#include "InputCoreTypes.h"
#include "TimerManager.h"

void UWidgetStudioButtonBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	OnPressed.AddDynamic(this, &UWidgetStudioButtonBase::ExecuteOnPressedLambda);
}

FReply UWidgetStudioButtonBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (!bIsInteractable)
	{
		return FReply::Unhandled();
	}
	
	if (InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
	{
		bIsPressed = true;
		
		if (IsCheckable())
		{
			if (bIsCheckedStateLocked) { return FReply::Unhandled(); }
			SetChecked(!IsChecked());
			OnToggled.Broadcast(this, IsChecked());
		}
		else
		{
			if (GetWorld()->GetTimerManager().IsTimerActive(DoublePressTimer))
			{
				OnDoublePressed.Broadcast(this);
				GetWorld()->GetTimerManager().ClearTimer(DoublePressTimer);
			}
			else
			{
				GetWorld()->GetTimerManager().SetTimer(DoublePressTimer, 0.65f, false);
			}
		}
		
		OnPressed.Broadcast(this);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply UWidgetStudioButtonBase::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	bIsPressed = false;
	OnReleased.Broadcast(this);
	return FReply::Handled();
}

void UWidgetStudioButtonBase::NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	Super::NativeOnMouseCaptureLost(CaptureLostEvent);
	bIsPressed = false;
}

void UWidgetStudioButtonBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	bIsPressed = false;
}

void UWidgetStudioButtonBase::SetCheckable(const bool bNewCheckableState)
{
	if(bForceCheckable)
	{
		bIsCheckable = true;
	}
	else
	{
		bIsCheckable = bNewCheckableState;
	}
}

void UWidgetStudioButtonBase::SetChecked(const bool bNewCheckedState, const bool bBroadcast)
{
	if (bIsCheckable || bForceCheckable)
	{
		if (bIsCheckedStateLocked)
		{
			UE_LOG(LogWidgetStudio, Warning, TEXT("%s: Checked state NOT changed due to checked state being locked."), *FString(__func__));
			return;
		}
		bIsChecked = bNewCheckedState;
		if (bBroadcast)
		{
			OnToggled.Broadcast(this, IsChecked());
		}
	}
}

void UWidgetStudioButtonBase::SetCheckedLockedState(const bool bNewCheckedState)
{
	bIsCheckedStateLocked = bNewCheckedState;
}

bool UWidgetStudioButtonBase::IsCheckable() const
{
	return bIsCheckable;
}

bool UWidgetStudioButtonBase::IsChecked() const
{
	return bIsCheckable && bIsChecked;
}

bool UWidgetStudioButtonBase::IsCheckedStateLocked() const
{
	return bIsCheckedStateLocked;
}

bool UWidgetStudioButtonBase::IsPressed() const
{
	return bIsPressed;
}

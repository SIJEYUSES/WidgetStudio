/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/WSSliderBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"

void UWidgetStudioSliderBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	Value = FilterValue(Value);
}

float UWidgetStudioSliderBase::FilterValue(const float InValue) const
{
	float TempValue = InValue;

	// Snap to Step Size
	TempValue = UKismetMathLibrary::GridSnap_Float(TempValue, StepSize);

	// Clamp the value between the minimum and maximum values.
	if (ClampMethod == EValueClamp::Clamped)
	{
		TempValue = UKismetMathLibrary::FClamp(TempValue, Min, Max);
	}

	// Rounds the value to the nearest whole number.
	if (Enumeration == EEnumeration::Int)
	{
		TempValue = UKismetMathLibrary::Round(TempValue);
	}
	
	return TempValue;
}

EValueClamp UWidgetStudioSliderBase::GetClampMethod() const
{
	return ClampMethod;
}

float UWidgetStudioSliderBase::GetValue() const
{
	return Value;
}

FString UWidgetStudioSliderBase::GetValueAsString() const
{
	return  GetValueAsText().ToString();
}

FText UWidgetStudioSliderBase::GetValueAsText() const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("Value"), FText::AsNumber(Value));
	FText FormattedText = FText::Format(NSLOCTEXT("YourNamespace", "YourKey", "{Value}"), Args);

	return FormattedText;
}


float UWidgetStudioSliderBase::GetMinValue() const
{
	return Min;
}

float UWidgetStudioSliderBase::GetMaxValue() const
{
	return Max;
}

float UWidgetStudioSliderBase::GetStepSize() const
{
	return StepSize;
}

EEnumeration UWidgetStudioSliderBase::GetEnumerationType() const
{
	return Enumeration;
}

bool UWidgetStudioSliderBase::IsIncrementLocked() const
{
	return bLockIncrement;
}

bool UWidgetStudioSliderBase::IsDecrementLocked() const
{
	return bLockDecrement;
}

void UWidgetStudioSliderBase::SetLockIncrement(const bool NewState)
{
	bLockIncrement = NewState;
}

void UWidgetStudioSliderBase::SetLockDecrement(const bool NewState)
{
	bLockDecrement = NewState;
}

void UWidgetStudioSliderBase::SetClampMethod(const EValueClamp NewMethod)
{
	ClampMethod = NewMethod;
	SynchronizeProperties();
}

void UWidgetStudioSliderBase::SetEnumerationType(const EEnumeration NewType)
{
	Enumeration = NewType;
	SynchronizeProperties();
}

void UWidgetStudioSliderBase::Increment(const float Multiplier, const bool bBroadcast)
{
	SetValue(Value + (StepSize * Multiplier), bBroadcast);
}

void UWidgetStudioSliderBase::Decrement(const float Multiplier, const bool bBroadcast)
{
	SetValue(Value - (StepSize * Multiplier), bBroadcast);
}

void UWidgetStudioSliderBase::SetMinValue(const float NewValue)
{
	Min = NewValue;
	SynchronizeProperties();
}

void UWidgetStudioSliderBase::SetMaxValue(const float NewValue)
{
	Max = NewValue;
	SynchronizeProperties();
}

void UWidgetStudioSliderBase::SetStepSize(const float NewValue)
{
	StepSize = Enumeration == EEnumeration::Int ? UKismetMathLibrary::Round(NewValue) : NewValue;
	SynchronizeProperties();
}

void UWidgetStudioSliderBase::SetValue(const float NewValue, const bool bBroadcast)
{
	// If incrementing is locked, set the slider to the stored value.
	if (bLockIncrement && NewValue > Value)
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("%s: Attempted to increase the value when incrementing is locked."), *FString(__func__))
		SynchronizeProperties();
		return;
	}
	
	// If decrementing is locked, set the slider to the stored value.
	if (bLockDecrement && NewValue < Value)
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("%s: Attempted to decrease the value when decrementing is locked."), *FString(__func__))
		SynchronizeProperties();
		return;
	}
	
	Value = FilterValue(NewValue);

	if (bBroadcast)
	{
		OnValueChanged.Broadcast(Value);
	}
	
	SynchronizeProperties();
}

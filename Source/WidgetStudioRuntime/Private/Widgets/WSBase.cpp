/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/WSBase.h"

void UWidgetStudioBase::InitializeStyling()
{
	// Override in child class
}

void UWidgetStudioBase::UpdateStyling()
{
	if (!IsConstructed()) { return; }
	// Override in child class
}

void UWidgetStudioBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	this->OnHoverStateChanged.Broadcast(this, true);
}

void UWidgetStudioBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	this->OnHoverStateChanged.Broadcast(this, false);
}

void UWidgetStudioBase::NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	Super::NativeOnMouseCaptureLost(CaptureLostEvent);
	this->OnHoverStateChanged.Broadcast(this, false);
}

FVector2D UWidgetStudioBase::GetDimensions() const
{
	const FVector2D ControlDimensions = UWidgetStudioFunctionLibrary::GetControlDimensions();

	const float Width = UWidgetStudioFunctionLibrary::GetSizeByModifier(SizeModifier, OverrideDimensions.X > 0 ? OverrideDimensions.X : ControlDimensions.X);
	const float Height = UWidgetStudioFunctionLibrary::GetSizeByModifier(SizeModifier, OverrideDimensions.Y > 0 ? OverrideDimensions.Y : ControlDimensions.Y);
	
	return FVector2D(
		Width > MinimumDimensions.X ? Width : MinimumDimensions.X,
		Height > MinimumDimensions.Y ? Height : MinimumDimensions.Y
		);
}

int32 UWidgetStudioBase::GetBorderRadius() const
{
	return OverrideBorderRadius > -1 ? OverrideBorderRadius : UWidgetStudioFunctionLibrary::GetBorderRadius();
}

void UWidgetStudioBase::ForceStyleUpdate()
{
	UpdateStyling();
}

void UWidgetStudioBase::SetSizeModifier(const ESizeModifier InSizeModifier)
{
	SizeModifier = InSizeModifier;
	UpdateStyling();
}

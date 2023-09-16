/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/Modern/WSModernCard.h"

#include "Blueprint/WidgetTree.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"

TSharedRef<SWidget> UWidgetStudioModernCard::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{
		
		// Construct Widgets
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Size Box"));
		ScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Scale Box"));
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		DropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("DropShadow"));
		Background = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background"));
			
		// Construct Hierarchy
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(ScaleBox);
		ScaleBox->AddChild(Overlay);
		Overlay->AddChild(DropShadow);
		Overlay->AddChild(Background);

		// Additional Construction Parameters
		bCanOverrideDimensions = true;
		MinimumDimensions = FVector2D(16, 16);

		InitializeStyling();

	}
	return Widget;
}

int32 UWidgetStudioModernCard::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (bDisablePainting) { return LayerId; }
	
	// Smoothly update Size Box
	UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, GetDimensions().X, GetDimensions().Y, 0);
	
	/* Smoothly lerp the background color based on current states and properties */
	UWidgetStudioFunctionLibrary::InterpImageColor(Background, Color, AnimationTime);
	
	// Smoothly lerp corner style
	const float NewRoundness = UWidgetStudioFunctionLibrary::GetBorderRadius();
	UWidgetStudioFunctionLibrary::InterpBrushImageSize(Background, NewRoundness, NewRoundness, AnimationTime);

	// Animate the drop shadow.
	if (ShadowStyle != EShadowStyle::None)
	{
		UWidgetStudioFunctionLibrary::InterpBrushImageSize(DropShadow, NewRoundness, NewRoundness, AnimationTime);
		
		// Smoothly update DropShadow opacity
		const float NewDropShadowOpacity = ShadowStyle != EShadowStyle::None  ? 0.5f  : 0.f;
		UWidgetStudioFunctionLibrary::InterpWidgetOpacity(DropShadow, NewDropShadowOpacity, AnimationTime);
	
		// Smoothly update DropShadow location
		const float NewDropShadowLoc = ShadowStyle == EShadowStyle::Long ? 5.f : 3.0f;
		UWidgetStudioFunctionLibrary::InterpWidgetTranslation(DropShadow, FVector2D(0, IsHovered() && bEnableShadowHoverAnimation ? NewDropShadowLoc + 2.f : NewDropShadowLoc), AnimationTime);
	}
	
	return LayerId;
}

void UWidgetStudioModernCard::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	InitializeStyling();
	UpdateStyling();
}

void UWidgetStudioModernCard::InitializeStyling()
{
	Super::InitializeStyling();

	// Get the correct sizing
	const float RoundedSize = FMath::Clamp(static_cast<float>(GetBorderRadius()), 0.f, GetDimensions().Y);
	
	// Initialize SizeBox widget styling
	if (SizeBox)
	{
		SizeBox->SetMinDesiredWidth(GetDimensions().X);
		SizeBox->SetMaxDesiredHeight(GetDimensions().Y);
		Cast<UOverlaySlot>(SizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(SizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the ScaleBox widget styling
	if (ScaleBox)
	{
		ScaleBox->SetStretch(EStretch::Fill);
		Cast<USizeBoxSlot>(ScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(ScaleBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the Overlay widget styling
	if (Overlay)
	{
		Cast<UScaleBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UScaleBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the DropShadow widget styling
	if (DropShadow)
	{
		DropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
		DropShadow->SetColorAndOpacity(FLinearColor::Black);
		DropShadow->SetRenderOpacity(BaseDropShadowOpacity);
		DropShadow->SetRenderOpacity(0.35f);

		// 复制一个 FSlateBrush 对象并修改
		FSlateBrush DropShadowBrush = DropShadow->GetBrush();
		DropShadowBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		DropShadowBrush.DrawAs = ESlateBrushDrawType::Box;
		// 将修改后的 FSlateBrush 设置回去
		DropShadow->SetBrush(DropShadowBrush);

		UWidgetStudioFunctionLibrary::SetBrushImageSize(DropShadow, RoundedSize, RoundedSize);
		Cast<UOverlaySlot>(DropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(DropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UOverlaySlot>(DropShadow->Slot)->SetPadding(FMargin(1, 1, 1, 0));
	}

	// Initialize the Background widget styling
	if (Background)
	{
		Background->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(Color));

		// 复制一个 FSlateBrush 对象并修改
		FSlateBrush BackgroundBrush = Background->GetBrush();
		BackgroundBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		BackgroundBrush.DrawAs = ESlateBrushDrawType::Box;
		// 将修改后的 FSlateBrush 设置回去
		Background->SetBrush(BackgroundBrush);

		UWidgetStudioFunctionLibrary::SetBrushImageSize(Background, RoundedSize, RoundedSize);
		Cast<UOverlaySlot>(Background->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Background->Slot)->SetVerticalAlignment(VAlign_Fill);
	}


}

void UWidgetStudioModernCard::UpdateStyling()
{
	Super::UpdateStyling();

	// Update the SizeBox widget styling
	if (SizeBox)
	{
		if(bDisablePainting)
		{
			SizeBox->SetMinDesiredWidth(GetDimensions().X);
			SizeBox->SetMaxDesiredHeight(GetDimensions().Y);
		}
	}

	// Update the DropShadow widget styling
	if (DropShadow)
	{
		DropShadow->SetVisibility(ShadowStyle != EShadowStyle::None ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

EPalette UWidgetStudioModernCard::GetColor() const
{
	return Color;
}

bool UWidgetStudioModernCard::IsShadowHoverAnimationEnabled() const
{
	return bEnableShadowHoverAnimation;
}

EShadowStyle UWidgetStudioModernCard::GetDropShadowStyle() const
{
	return ShadowStyle;
}

void UWidgetStudioModernCard::SetShadowStyle(const EShadowStyle NewStyle)
{
	ShadowStyle = NewStyle;
	SynchronizeProperties();
}

void UWidgetStudioModernCard::SetShadowHoverAnimationEnabled(const bool NewState)
{
	bEnableShadowHoverAnimation = NewState;
	SynchronizeProperties();
}

void UWidgetStudioModernCard::SetColor(const EPalette NewColor)
{
	Color = NewColor;
	SynchronizeProperties();
}

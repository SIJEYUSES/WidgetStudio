/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/Modern/WSModernProgressBar.h"

#include "Blueprint/WidgetTree.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Kismet/KismetMathLibrary.h"

TSharedRef<SWidget> UWidgetStudioModernProgressBar::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{

		/*
		* RootWidget (InvalidationBox)
		* - Size Box
		* -- Overlay
		* --- Track Scale Box
		* ---- Track Overlay
		* ----- Track Drop Shadow
		* ----- Track
		* --- Fill Track Size Box
		* ---- Fill Track
		* --- Indicator Size Box
		* ---- Indicator
		*/
		
		/* Construct Widgets */
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Size Box"));
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		TrackScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Track Scale Box"));
		TrackOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Track Overlay"));
		TrackDropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Track DropShadow"));
		Track = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Track0"));
		FillTrackSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Fill Track Size Box"));
		FillTrack = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Fill Track"));
		IndicatorSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Indicator Size Box"));
		Indicator = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Indicator"));

		/* Construct Hierarchy */
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(Overlay);
		
		Overlay->AddChild(TrackScaleBox);
		TrackScaleBox->AddChild(TrackOverlay);
		TrackOverlay->AddChild(TrackDropShadow);
		TrackOverlay->AddChild(Track);

		Overlay->AddChild(FillTrackSizeBox);
		FillTrackSizeBox->AddChild(FillTrack);

		Overlay->AddChild(IndicatorSizeBox);
		IndicatorSizeBox->AddChild(Indicator);

		// Additional Construction Parameters
		MinimumDimensions = FVector2D(225, 13);
		bCanOverrideBorderRadius = false;

		InitializeStyling();
	}
	return Widget;
}

int32 UWidgetStudioModernProgressBar::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (bDisablePainting) { return LayerId; }
	
	/* Smoothly update size box */
	UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, GetDimensions().X, 0, AnimationTime);
	
	/* Smoothly lerp Track Color */
	UWidgetStudioFunctionLibrary::InterpImageColor(Track, TrackColor, AnimationTime);

	/* Smoothly lerp Fill Track Color */
	UWidgetStudioFunctionLibrary::InterpImageColor(FillTrack, FillColor, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(FillTrack, Percent > 0.f ? 1.f : 0.f, AnimationTime * 4);

	// Get fill width
	const float NewFillWidth = UKismetMathLibrary::MapRangeClamped(Percent, .0f, 100.f, 0, Track->GetPaintSpaceGeometry().GetLocalSize().X);
	
	/* 平滑地插值填充轨道宽度 */
	UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(FillTrackSizeBox, NewFillWidth, FillTrackSizeBox->GetHeightOverride(), AnimationTime * 2, false);

	/* Smoothly lerp indicator width */
	if (bDisplayIndicator)
	{
		const float TargetIndicatorWidth = bForwardProgress ? NewFillWidth : 0.f;
		float NewIndicatorWidth;

		if (IndicatorSizeBox->GetWidthOverride() == TargetIndicatorWidth)
		{
			NewIndicatorWidth = bForwardProgress ? 0.f : NewFillWidth;
		}
		else
		{
			NewIndicatorWidth = UKismetMathLibrary::FInterpTo(
				IndicatorSizeBox->GetWidthOverride(),
				TargetIndicatorWidth,
				GetWorld()->GetDeltaSeconds(),
				AnimationTime * .5f
			);
		}
		IndicatorSizeBox->SetWidthOverride(NewIndicatorWidth);
		IndicatorSizeBox->SetRenderOpacity(UKismetMathLibrary::MapRangeClamped(NewIndicatorWidth, 0.f, NewFillWidth, bForwardProgress ? 0.5f : 0.0f, bForwardProgress ? 0.0f : 0.75f));
	}

	return LayerId;
}

void UWidgetStudioModernProgressBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioModernProgressBar::InitializeStyling()
{
	Super::InitializeStyling();

	// Cache size
	const float Height = GetDimensions().Y * .25f;

	// Initialize the SizeBox widget styling
	if (SizeBox)
	{
		SizeBox->SetMinDesiredWidth(GetDimensions().X);
		
		Cast<UOverlaySlot>(SizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(SizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	// Initialize the Overlay widget styling
	if (Overlay)
	{
		Cast<USizeBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the TrackScaleBox widget styling
	if (TrackScaleBox)
	{
		TrackScaleBox->SetStretch(EStretch::ScaleToFit);
		
		Cast<UOverlaySlot>(TrackScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(TrackScaleBox->Slot)->SetVerticalAlignment(VAlign_Center);
	}
	
	// Initialize the TrackOverlay widget styling
	if (TrackOverlay)
	{
		Cast<UScaleBoxSlot>(TrackOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UScaleBoxSlot>(TrackOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the TrackDropShadow widget styling
	if (TrackDropShadow)
	{
		TrackDropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(TrackDropShadow, Height, Height);
		TrackDropShadow->SetRenderTranslation(FVector2D(0.f, 3.f));
		TrackDropShadow->SetColorAndOpacity(FLinearColor::Black);
		TrackDropShadow->SetRenderOpacity(0.3f);

		FSlateBrush NewBrush = TrackDropShadow->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		TrackDropShadow->SetBrush(NewBrush);

		Cast<UOverlaySlot>(TrackDropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(TrackDropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	
	// Initialize the Track widget styling
	if (Track)
	{
		Track->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(Track, Height, Height);

		FSlateBrush NewBrush = Track->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		Track->SetBrush(NewBrush);

		Track->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(TrackColor));

		Cast<UOverlaySlot>(Track->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Track->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	
	// Initialize the FillTrackScaleBox widget styling
	if (FillTrackSizeBox)
	{
		Cast<UOverlaySlot>(FillTrackSizeBox->Slot)->SetHorizontalAlignment(HAlign_Left);
		Cast<UOverlaySlot>(FillTrackSizeBox->Slot)->SetVerticalAlignment(VAlign_Center);
	}
	
	// Initialize the FillTrack widget styling
	if (FillTrack)
	{
		FillTrack->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(FillTrack, Height, Height);

		FSlateBrush NewBrush = FillTrack->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		FillTrack->SetBrush(NewBrush);

		FillTrack->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(FillColor));

		Cast<USizeBoxSlot>(FillTrack->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(FillTrack->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	
	// Initialize the IndicatorSizeBox widget styling
	if (IndicatorSizeBox)
	{
		IndicatorSizeBox->SetVisibility(bDisplayIndicator ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		
		Cast<UOverlaySlot>(IndicatorSizeBox->Slot)->SetHorizontalAlignment(HAlign_Left);
		Cast<UOverlaySlot>(IndicatorSizeBox->Slot)->SetVerticalAlignment(VAlign_Center);
	}

	// Initialize the Indicator widget styling
	if (Indicator)
	{
		Indicator->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(Indicator, Height, Height);

		FSlateBrush NewBrush = Indicator->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		Indicator->SetBrush(NewBrush);

		Indicator->SetColorAndOpacity(FLinearColor::White);

		Cast<USizeBoxSlot>(Indicator->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(Indicator->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

}

void UWidgetStudioModernProgressBar::UpdateStyling()
{
	Super::UpdateStyling();

	// Update the IndicatorSizeBox widget styling
	if (IndicatorSizeBox)
	{
		IndicatorSizeBox->SetVisibility(bDisplayIndicator ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

float UWidgetStudioModernProgressBar::GetProgress() const
{
	return Percent;
}

bool UWidgetStudioModernProgressBar::IsIndicatorVisible() const
{
	return bDisplayIndicator;
}

EPalette UWidgetStudioModernProgressBar::GetTrackColor() const
{
	return TrackColor;
}

EPalette UWidgetStudioModernProgressBar::GetFillColor() const
{
	return FillColor;
}

void UWidgetStudioModernProgressBar::SetProgress(const float NewValue)
{
	const float TempValue = UKismetMathLibrary::FClamp(NewValue, 0.0f, 100.0f);
	bForwardProgress = TempValue >= Percent;
	Percent = TempValue;
	OnProgressChanged.Broadcast(Percent);
}

void UWidgetStudioModernProgressBar::SetIndicatorVisibility(const bool bNewState)
{
	bDisplayIndicator = bNewState;
	SynchronizeProperties();
}

void UWidgetStudioModernProgressBar::SetTrackColor(const EPalette NewColor)
{
	TrackColor = NewColor;
}

void UWidgetStudioModernProgressBar::SetFillColor(const EPalette NewColor)
{
	FillColor = NewColor;
}

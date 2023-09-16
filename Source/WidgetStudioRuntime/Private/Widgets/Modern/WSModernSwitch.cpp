/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/Modern/WSModernSwitch.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"

TSharedRef<SWidget> UWidgetStudioModernSwitch::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{
		/* Construct Widgets */
		HBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HBox"));
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		TrackSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Track Size Box"));
		TrackScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Track Scale Box"));
		TrackOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Track Overlay"));
		TrackDropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Track DropShadow"));
		Track = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Track0"));
		HandleScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Handle Scale Box"));
		HandleOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Handle Overlay"));
		HandleDropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Handle DropShadow"));
		Handle = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Handle"));
		LabelItem = WidgetTree->ConstructWidget<UWidgetStudioText>(UWidgetStudioText::StaticClass(), TEXT("Label Item"));

		/* Construct Hierarchy */
		RootWidget->AddChild(HBox);
		
		if (LabelPlacement == ELabelPlacement::Left) { HBox->AddChild(LabelItem); }

		HBox->AddChild(Overlay);
		Overlay->AddChild(TrackSizeBox);
		TrackSizeBox->AddChild(TrackScaleBox);
		TrackScaleBox->AddChild(TrackOverlay);
		TrackOverlay->AddChild(TrackDropShadow);
		TrackOverlay->AddChild(Track);

		Overlay->AddChild(HandleScaleBox);
		HandleScaleBox->AddChild(HandleOverlay);
		HandleOverlay->AddChild(HandleDropShadow);
		HandleOverlay->AddChild(Handle);

		if (LabelPlacement == ELabelPlacement::Right || LabelPlacement == ELabelPlacement::Hide) { HBox->AddChild(LabelItem); }

		// Additional Construction Parameters
		bCanOverrideBorderRadius = false;
		
		InitializeStyling();
	}
	return Widget;
}

int32 UWidgetStudioModernSwitch::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	if (bDisablePainting) { return LayerId; }
	
	/* Smoothly update size */
	UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(TrackSizeBox, GetDimensions().Y * 1.5f, GetDimensions().Y * .6f, AnimationTime);
	
	/* Smoothly lerp Track Color */
	UWidgetStudioFunctionLibrary::InterpImageColor(Track, IsChecked() == true ? HandleColor : TrackColor, AnimationTime);

	/* Smoothly lerp Handle Color */
	UWidgetStudioFunctionLibrary::InterpImageColor(Handle, IsChecked() == true ? TrackColor : HandleColor, AnimationTime); 
	
	/* Smoothly lerp Text Color to match track color */
	UWidgetStudioFunctionLibrary::InterpWidgetColor(LabelItem, LabelColor, AnimationTime);

	/* Smoothly lerp handle location based on value/max */
	const float TrackWidth = Track->GetPaintSpaceGeometry().GetLocalSize().X - HandleOverlay->GetPaintSpaceGeometry().GetLocalSize().X;
	const float HandleHoverOffset = IsHovered() ? GetDimensions().Y * .25f : GetDimensions().Y * .1f;
	const float NewHandleLoc = IsChecked() == true ? TrackWidth - HandleHoverOffset : HandleHoverOffset;
	UWidgetStudioFunctionLibrary::InterpWidgetTranslation(HandleOverlay, FVector2D(NewHandleLoc, 0), AnimationTime * 2);
	
	/* Smoothly lerp handle drop shadow opacity based on hover state */
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(HandleDropShadow, IsHovered() ? .35 : .1, AnimationTime);

	/* Smoothly lerp handle drop shadow opacity based on hover state */
	UWidgetStudioFunctionLibrary::InterpWidgetTranslation(HandleDropShadow, FVector2D(0, IsHovered() ? 5.f : 1.f), AnimationTime);
	
	return LayerId;
}

void UWidgetStudioModernSwitch::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioModernSwitch::InitializeStyling()
{
	Super::InitializeStyling();

	/* Setup Init */

	SetCheckable(true);
	bForceCheckable = true;
	const float Size = GetDimensions().Y * .6f;

	/* Setup HBox */
	if (HBox)
	{
		Cast<UOverlaySlot>(HBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(HBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	/* Setup Overlay */
	if (Overlay)
	{
		Cast<UHorizontalBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UHorizontalBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UHorizontalBoxSlot>(Overlay->Slot)->SetSize(ESlateSizeRule::Fill);
	}

	/* Setup Track Size Box */
	if (TrackSizeBox)
	{
		TrackSizeBox->SetWidthOverride(GetDimensions().Y * 1.5f);
		TrackSizeBox->SetHeightOverride(Size);
		Cast<UOverlaySlot>(TrackSizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(TrackSizeBox->Slot)->SetVerticalAlignment(VAlign_Center);
	}

	/* Setup Track Scale Box */
	if (TrackScaleBox)
	{
		TrackScaleBox->SetStretch(EStretch::ScaleToFit);
		Cast<USizeBoxSlot>(TrackScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(TrackScaleBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	/* Setup Track Overlay */
	if (TrackOverlay)
	{
		Cast<UScaleBoxSlot>(TrackOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UScaleBoxSlot>(TrackOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	/* Setup Track DropShadow */
	if (TrackDropShadow)
	{
		TrackDropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(TrackDropShadow, Size, Size);

		FSlateBrush NewBrush = TrackDropShadow->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		TrackDropShadow->SetBrush(NewBrush);

		TrackDropShadow->SetRenderTranslation(FVector2D(0.f, 3.f));
		TrackDropShadow->SetColorAndOpacity(FLinearColor::Black);
		TrackDropShadow->SetRenderOpacity(0.3f);
		TrackDropShadow->SetVisibility(ESlateVisibility::HitTestInvisible);

		Cast<UOverlaySlot>(TrackDropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(TrackDropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);
	}


	/* Setup Track */
	if (Track)
	{
		Track->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(Track, Size, Size);

		FSlateBrush NewBrush = Track->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		Track->SetBrush(NewBrush);

		Track->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(IsChecked() == true ? HandleColor : TrackColor));

		Cast<UOverlaySlot>(Track->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Track->Slot)->SetVerticalAlignment(VAlign_Fill);
	}


	/* Setup Handle Scale Box */
	if (HandleScaleBox)
	{
		HandleScaleBox->SetStretch(EStretch::ScaleToFit);
		Cast<UOverlaySlot>(HandleScaleBox->Slot)->SetHorizontalAlignment(HAlign_Left);
		Cast<UOverlaySlot>(HandleScaleBox->Slot)->SetVerticalAlignment(VAlign_Center);
	}

	/* Setup Handle Overlay */
	if (HandleOverlay)
	{
		Cast<UScaleBoxSlot>(HandleOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UScaleBoxSlot>(HandleOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	/* Setup Handle Drop Shadow */
	if (HandleDropShadow)
	{
		HandleDropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(HandleDropShadow, Size * 0.75f, Size * 0.75f);

		FSlateBrush NewBrush = HandleDropShadow->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		HandleDropShadow->SetBrush(NewBrush);

		HandleDropShadow->SetRenderTranslation(FVector2D(0.f, 3.f));
		HandleDropShadow->SetColorAndOpacity(FLinearColor::Black);
		HandleDropShadow->SetRenderOpacity(0.3f);
		Cast<UOverlaySlot>(HandleDropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(HandleDropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);
	}


	/* Setup Handle */
	if (Handle)
	{
		Handle->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(Handle, Size * 0.75f, Size * 0.75f);

		FSlateBrush NewBrush = Handle->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		Handle->SetBrush(NewBrush);

		Handle->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(IsChecked() == true ? TrackColor : HandleColor));
		Cast<UOverlaySlot>(Handle->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Handle->Slot)->SetVerticalAlignment(VAlign_Fill);
	}


	/* Setup Label Item */
	if (LabelItem)
	{
		LabelItem->bDisablePainting = true;
		LabelItem->SetColor(EPalette::White);
		LabelItem->SetType(EWSFontType::Caption);
		LabelItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(LabelColor));
		Cast<UHorizontalBoxSlot>(LabelItem->Slot)->SetHorizontalAlignment(LabelPlacement == ELabelPlacement::Left ? HAlign_Left : HAlign_Right);
		Cast<UHorizontalBoxSlot>(LabelItem->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UHorizontalBoxSlot>(LabelItem->Slot)->SetSize(ESlateSizeRule::Fill);
	}
}

void UWidgetStudioModernSwitch::UpdateStyling()
{
	Super::UpdateStyling();

	if (LabelItem)
	{
		LabelItem->SetText(Label);
	}

	if (HBox && LabelItem && Overlay)
	{
		if (LabelPlacement == ELabelPlacement::Left)
		{
			HBox->AddChild(LabelItem);
			HBox->AddChild(Overlay);
		}

		if (LabelPlacement == ELabelPlacement::Right || LabelPlacement == ELabelPlacement::Hide)
		{
			HBox->AddChild(Overlay);
			HBox->AddChild(LabelItem);
		}
		LabelItem->SetVisibility(LabelPlacement == ELabelPlacement::Hide ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
		Cast<UHorizontalBoxSlot>(LabelItem->Slot)->SetPadding(
			FMargin(
				LabelPlacement == ELabelPlacement::Right ? 15 : 0,
				0,
				LabelPlacement == ELabelPlacement::Left ? 15 : 0,
				0
			)
		);
	}
}

FText UWidgetStudioModernSwitch::GetLabel() const
{
	return Label;
}

bool UWidgetStudioModernSwitch::IsLabelVisible() const
{
	return LabelPlacement != ELabelPlacement::Hide;
}

EPalette UWidgetStudioModernSwitch::GetTrackColor() const
{
	return TrackColor;
}

EPalette UWidgetStudioModernSwitch::GetHandleColor() const
{
	return HandleColor;
}

EPalette UWidgetStudioModernSwitch::GetLabelColor() const
{
	return LabelColor;
}

void UWidgetStudioModernSwitch::SetLabelPlacement(const ELabelPlacement NewPlacement)
{
	LabelPlacement = NewPlacement;
	SynchronizeProperties();
}

void UWidgetStudioModernSwitch::SetLabel(const FText NewLabel)
{
	Label = NewLabel;
	SynchronizeProperties();
}

void UWidgetStudioModernSwitch::SetTrackColor(const EPalette NewColor)
{
	TrackColor = NewColor;
}

void UWidgetStudioModernSwitch::SetHandleColor(const EPalette NewColor)
{
	HandleColor = NewColor;
}

void UWidgetStudioModernSwitch::SetLabelColor(const EPalette NewColor)
{
	LabelColor = NewColor;
}

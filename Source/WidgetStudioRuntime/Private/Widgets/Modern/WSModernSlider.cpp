/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/Modern/WSModernSlider.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/Font.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Engine/Engine.h"

TSharedRef<SWidget> UWidgetStudioModernSlider::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{

		/*
		* RootWidget (InvalidationBox)
		* - Size Box
		* -- HBox
		* --- Overlay
		* ---- Track Scale Box
		* ----- Track Overlay
		* ------ Track Drop Shadow
		* ------ Track
		* ---- Fill Track Size Box
		* ----- Fill Track
		* ---- Handle Scale Box
		* ----- Handle Size Box
		* ------ Handle Overlay
		* ------- Handle Drop Shadow
		* ------- Handle
		* ---- SliderItem
		* --- Text Input Size Box
		* ---- Text Input
		*/
		
		/* Construct Widgets */
		LabelItem = WidgetTree->ConstructWidget<UWidgetStudioText>(UWidgetStudioText::StaticClass(), TEXT("Label Item"));
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Size Box"));
		HBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HBox"));
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		TrackScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Track Scale Box"));
		TrackOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Track Overlay"));
		TrackDropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Track DropShadow"));
		Track = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Track0"));
		FillTrackSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Fill Track Size Box"));
		FillTrack = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Fill Track"));
		HandleScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Handle Scale Box"));
		HandleSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Handle Size Box"));
		HandleOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Handle Overlay"));
		HandleDropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Handle DropShadow"));
		Handle = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Handle"));
		SliderItem = WidgetTree->ConstructWidget<USlider>(USlider::StaticClass(), TEXT("Slider Item"));
		TextInputSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Text Input Size Box"));
		TextInput = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("Text Input"));

		/* Construct Hierarchy */
		RootWidget->AddChild(LabelItem);
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(HBox);
		HBox->AddChild(Overlay);
		
		Overlay->AddChild(TrackScaleBox);
		TrackScaleBox->AddChild(TrackOverlay);
		TrackOverlay->AddChild(TrackDropShadow);
		TrackOverlay->AddChild(Track);

		Overlay->AddChild(FillTrackSizeBox);
		FillTrackSizeBox->AddChild(FillTrack);

		Overlay->AddChild(HandleScaleBox);
		HandleScaleBox->AddChild(HandleSizeBox);
		HandleSizeBox->AddChild(HandleOverlay);
		HandleOverlay->AddChild(HandleDropShadow);
		HandleOverlay->AddChild(Handle);

		Overlay->AddChild(SliderItem);

		HBox->AddChild(TextInputSizeBox);
		TextInputSizeBox->AddChild(TextInput);

		// Additional Construction Parameters
		MinimumDimensions = FVector2D(225, 13);
		bCanOverrideBorderRadius = false;

		InitializeStyling();

	}
	return Widget;
}

int32 UWidgetStudioModernSlider::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	if (bDisablePainting) { return LayerId; }
	
	// Smoothly lerp size box
	UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, GetDimensions().X, 0, AnimationTime);
	
	// Smoothly lerp Track Color
	UWidgetStudioFunctionLibrary::InterpImageColor(Track, TrackColor, AnimationTime);

	// Smoothly lerp Fill Track Color
	const float AdjustedPercent = UWidgetStudioFunctionLibrary::GetAdjustedPercent(Value, Min, Max);
	const EPalette NewFillColor = bColorBasedOnValue ? UWidgetStudioFunctionLibrary::GetPaletteBasedValue(AdjustedPercent, bReverseColorOrder) : HandleColor;
	UWidgetStudioFunctionLibrary::InterpImageColor(FillTrack, NewFillColor, AnimationTime);

	// Smoothly lerp Handle Color
	UWidgetStudioFunctionLibrary::InterpImageColor(Handle, NewFillColor, AnimationTime);
	
	/* Smoothly lerp Text Color to match track color */
	UWidgetStudioFunctionLibrary::InterpEditableTextBoxColor(TextInput, TextColor, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpWidgetColor(LabelItem, TextColor, AnimationTime);

	/* Smoothly lerp handle based on hover state */
	UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(HandleSizeBox, IsHovered() ? HandleSizeBox->GetHeightOverride() * 2.5f : HandleSizeBox->GetHeightOverride(), 0.f, AnimationTime * 3.f);

	/* Smoothly lerp handle location based on value/max */
	const float HandleHalfWidth = IsHandleVisible() ? HandleOverlay->GetPaintSpaceGeometry().GetLocalSize().X / 2.f : 0.f;
	const float NewHandleLoc = UKismetMathLibrary::MapRangeClamped(
		SliderItem->GetValue(),
		SliderItem->GetMinValue(),
		SliderItem->GetMaxValue(),
		0.f,
		Track->GetPaintSpaceGeometry().GetLocalSize().X - (bDisplayHandle ? HandleSizeBox->GetWidthOverride() : 0.f));


	UWidgetStudioFunctionLibrary::InterpWidgetTranslation(HandleOverlay, FVector2D(NewHandleLoc, 0), AnimationTime * 4);
	
	/* Smoothly lerp fill track location based on value/max */
	UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(FillTrackSizeBox, NewHandleLoc + HandleHalfWidth, 0, AnimationTime * 4, false);

	/* Smoothly lerp handle drop shadow opacity based on hover state */
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(HandleDropShadow, IsHovered() ? .35 : .1, AnimationTime);

	/* Smoothly lerp handle drop shadow opacity based on hover state */
	UWidgetStudioFunctionLibrary::InterpWidgetTranslation(HandleDropShadow, FVector2D(0, IsHovered() ? 5.f : 1.f), AnimationTime);
	
	return LayerId;
}

void UWidgetStudioModernSlider::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioModernSlider::InitializeStyling()
{
	Super::InitializeStyling();

	/* Apply Styling */
		const float Height = GetDimensions().Y * .25f;
		const float Size = GetDimensions().Y * .5f;

		/* Setup Size Box */
		if (SizeBox)
		{
			SizeBox->SetMinDesiredWidth(GetDimensions().X);
			Cast<UVerticalBoxSlot>(SizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
			Cast<UVerticalBoxSlot>(SizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		}
	
		// Setup Label
		if (LabelItem)
		{
			LabelItem->bDisablePainting = true;
			LabelItem->SetColor(EPalette::White);
			LabelItem->SetType(EWSFontType::Caption);
			Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetPadding(FMargin(0, 0, 0, 3));
		}
	
		/* Setup HBox */
		if (HBox)
		{
			Cast<USizeBoxSlot>(HBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
			Cast<USizeBoxSlot>(HBox->Slot)->SetVerticalAlignment(VAlign_Fill);
			Cast<USizeBoxSlot>(HBox->Slot)->SetPadding(FVector4(0, 5, 0, 5));
		}
	
		/* Setup Overlay */
		if (Overlay)
		{
			Cast<UHorizontalBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
			Cast<UHorizontalBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);
			Cast<UHorizontalBoxSlot>(Overlay->Slot)->SetSize(ESlateSizeRule::Fill);
		}
	
		/* Setup Track Scale Box */
		if (TrackScaleBox)
		{
			TrackScaleBox->SetStretch(EStretch::ScaleToFit);
			Cast<UOverlaySlot>(TrackScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
			Cast<UOverlaySlot>(TrackScaleBox->Slot)->SetVerticalAlignment(VAlign_Center);
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
			UWidgetStudioFunctionLibrary::SetBrushImageSize(TrackDropShadow, Height, Height);

			FSlateBrush NewBrush = TrackDropShadow->GetBrush();
			NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
			NewBrush.DrawAs = ESlateBrushDrawType::Box;
			TrackDropShadow->SetBrush(NewBrush);

			TrackDropShadow->SetRenderTranslation(FVector2D(0.f, 3.f));
			TrackDropShadow->SetColorAndOpacity(FLinearColor::Black);
			TrackDropShadow->SetRenderOpacity(0.3f);

			Cast<UOverlaySlot>(TrackDropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
			Cast<UOverlaySlot>(TrackDropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);
		}

	
		/* Setup Track */
		if (Track)
		{
			Track->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
			UWidgetStudioFunctionLibrary::SetBrushImageSize(Track, Height, Height);

			FSlateBrush NewBrush = Track->GetBrush();
			NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
			NewBrush.DrawAs = ESlateBrushDrawType::Box;
			Track->SetBrush(NewBrush);

			Cast<UOverlaySlot>(Track->Slot)->SetHorizontalAlignment(HAlign_Fill);
			Cast<UOverlaySlot>(Track->Slot)->SetVerticalAlignment(VAlign_Fill);
		}

	
		/* Setup Fill Track Size Box */
		if (FillTrackSizeBox)
		{
			Cast<UOverlaySlot>(FillTrackSizeBox->Slot)->SetHorizontalAlignment(HAlign_Left);
			Cast<UOverlaySlot>(FillTrackSizeBox->Slot)->SetVerticalAlignment(VAlign_Center);
		}
	
		/* Setup Fill Track */
		if (FillTrack)
		{
			FillTrack->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
			UWidgetStudioFunctionLibrary::SetBrushImageSize(FillTrack, Height, Height);

			FSlateBrush NewBrush = FillTrack->GetBrush();
			NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
			NewBrush.DrawAs = ESlateBrushDrawType::Box;
			FillTrack->SetBrush(NewBrush);

			Cast<USizeBoxSlot>(FillTrack->Slot)->SetHorizontalAlignment(HAlign_Fill);
			Cast<USizeBoxSlot>(FillTrack->Slot)->SetVerticalAlignment(VAlign_Fill);
		}

	
		/* Setup Handle Scale Box */
		if (HandleScaleBox)
		{
			HandleScaleBox->SetStretch(EStretch::ScaleToFit);
			Cast<UOverlaySlot>(HandleScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
			Cast<UOverlaySlot>(HandleScaleBox->Slot)->SetVerticalAlignment(VAlign_Center);
		}
	
		/* Setup Handle Size Box */
		if (HandleSizeBox)
		{
			HandleSizeBox->SetHeightOverride(Size);
			Cast<UScaleBoxSlot>(HandleSizeBox->Slot)->SetHorizontalAlignment(HAlign_Left);
			Cast<UScaleBoxSlot>(HandleSizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		}
	
		/* Setup Handle Overlay */
		if (HandleOverlay)
		{
			Cast<USizeBoxSlot>(HandleOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
			Cast<USizeBoxSlot>(HandleOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);
		}
	
		/* Setup Handle Drop Shadow */
		if (HandleDropShadow)
		{
			HandleDropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
			UWidgetStudioFunctionLibrary::SetBrushImageSize(HandleDropShadow, Size, Size);

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
			UWidgetStudioFunctionLibrary::SetBrushImageSize(Handle, Size, Size);

			FSlateBrush NewBrush = Handle->GetBrush();
			NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
			NewBrush.DrawAs = ESlateBrushDrawType::Box;
			Handle->SetBrush(NewBrush);

			Cast<UOverlaySlot>(Handle->Slot)->SetHorizontalAlignment(HAlign_Fill);
			Cast<UOverlaySlot>(Handle->Slot)->SetVerticalAlignment(VAlign_Fill);
		}

	
		/* Setup Slider Item */
		if (SliderItem)
		{
			SliderItem->SetRenderOpacity(0); // Hide the Slider as it's only used for interactivity and not display

			// Using the new API to set draw type for different states
			FSliderStyle SliderStyle = SliderItem->GetWidgetStyle();// Use getter to get current style
			SliderStyle.NormalBarImage.TintColor = FLinearColor::Transparent;
			SliderStyle.NormalBarImage.ImageSize = FVector2D::ZeroVector;
			SliderStyle.HoveredBarImage.TintColor = FLinearColor::Transparent;
			SliderStyle.HoveredBarImage.ImageSize = FVector2D::ZeroVector;
			SliderStyle.NormalThumbImage.TintColor = FLinearColor::Transparent;
			SliderStyle.NormalThumbImage.ImageSize = FVector2D::ZeroVector;
			SliderStyle.HoveredThumbImage.TintColor = FLinearColor::Transparent;
			SliderStyle.HoveredThumbImage.ImageSize = FVector2D::ZeroVector;
			SliderStyle.DisabledBarImage.TintColor = FLinearColor::Transparent;
			SliderStyle.DisabledBarImage.ImageSize = FVector2D::ZeroVector;
			SliderStyle.DisabledThumbImage.TintColor = FLinearColor::Transparent;
			SliderStyle.DisabledThumbImage.ImageSize = FVector2D::ZeroVector;

			SliderItem->SetWidgetStyle(SliderStyle); // Use setter to set modified style

			Cast<UOverlaySlot>(SliderItem->Slot)->SetHorizontalAlignment(HAlign_Fill);
			Cast<UOverlaySlot>(SliderItem->Slot)->SetVerticalAlignment(VAlign_Fill);
		}


	
		/* Setup Text Input Size Box */
		if (TextInputSizeBox)
		{
			TextInputSizeBox->SetWidthOverride(55);
			Cast<UHorizontalBoxSlot>(TextInputSizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
			Cast<UHorizontalBoxSlot>(TextInputSizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		}
	
		/* Setup Text Input Box */
		if (TextInput)
		{
			if (const UWidgetStudioTypography* Typography = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>()->GetTypography())
			{
				// ReSharper disable once CppUseStructuredBinding
				const FFontStyle FontStyle = UWidgetStudioFunctionLibrary::GetTypeScaleFromTypography(EWSFontType::Caption);

				FSlateFontInfo FontInfo;
				FontInfo.FontObject = Typography->Typeface;
				FontInfo.Size = FontStyle.Size;
				FontInfo.TypefaceFontName = UWidgetStudioFunctionLibrary::GetFontWeightName(FontStyle.Weight);
				FontInfo.LetterSpacing = FontStyle.LetterSpacing;

				TextInput->WidgetStyle.SetFont(FontInfo);
			}
		
			TextInput->SetJustification(ETextJustify::Right);
			TextInput->WidgetStyle.BackgroundImageNormal.DrawAs = ESlateBrushDrawType::NoDrawType;
			TextInput->WidgetStyle.BackgroundImageHovered.DrawAs = ESlateBrushDrawType::NoDrawType;
			TextInput->WidgetStyle.BackgroundImageFocused.DrawAs = ESlateBrushDrawType::NoDrawType;
			TextInput->WidgetStyle.BackgroundImageReadOnly.DrawAs = ESlateBrushDrawType::NoDrawType;
		}
	
		// Setup Colors

		const float AdjustedPercent = UWidgetStudioFunctionLibrary::GetAdjustedPercent(Value, Min, Max);
		const EPalette NewFillColor = bColorBasedOnValue ? UWidgetStudioFunctionLibrary::GetPaletteBasedValue(AdjustedPercent, bReverseColorOrder) : HandleColor;
		if (Track)
		{
			Track->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(TrackColor));
		}
		if (FillTrack)
		{
			FillTrack->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(NewFillColor));
		}
		if (Handle)
		{
			Handle->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(NewFillColor));
		}
		if (TextInput)
		{
			TextInput->WidgetStyle.SetForegroundColor(UWidgetStudioFunctionLibrary::GetColorFromPalette(TextColor));
		}
		if (LabelItem)
		{
			LabelItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(TextColor));
		}
		
		/* Setup Bindings */
		if (SliderItem && TextInput)
		{
			SliderItem->OnValueChanged.RemoveDynamic(this, &UWidgetStudioModernSlider::SyncSliderValue);
			TextInput->OnTextCommitted.RemoveDynamic(this, &UWidgetStudioModernSlider::OnTextCommitted);

			SliderItem->OnValueChanged.AddDynamic(this, &UWidgetStudioModernSlider::SyncSliderValue);
			TextInput->OnTextCommitted.AddDynamic(this, &UWidgetStudioModernSlider::OnTextCommitted);
		}
}

void UWidgetStudioModernSlider::UpdateStyling()
{
	Super::UpdateStyling();

	if (SliderItem)
	{
		SliderItem->SetMinValue(Min);
		SliderItem->SetMaxValue(Max);
		SliderItem->SetStepSize(StepSize);
		SliderItem->SetIsEnabled(bEnableInput);
		SliderItem->SetValue(FilterValue(Value));
	}

	if (TextInputSizeBox)
	{
		TextInputSizeBox->SetVisibility(bDisplayValue ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (TextInput)
	{
		TextInput->SetIsEnabled(bEnableInput);
		FNumberFormattingOptions NumberFormat;
		NumberFormat.MinimumIntegralDigits = 1;
		NumberFormat.MaximumIntegralDigits = 324;
		NumberFormat.MinimumFractionalDigits = 0;
		NumberFormat.MaximumFractionalDigits = 2;
		TextInput->SetText(FText::AsNumber(FilterValue(Value), &NumberFormat));
	}


	if (HandleScaleBox)
	{
		HandleScaleBox->SetVisibility(bDisplayHandle ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (LabelItem)
	{
		LabelItem->SetText(Label);
		LabelItem->SetVisibility(Label.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

		LabelItem->SizeModifier = SizeModifier;
	}
}

void UWidgetStudioModernSlider::OnTextCommitted(const FText& InText, const ETextCommit::Type InCommitMethod)
{
	if (InText.IsNumeric())
	{
		if (InCommitMethod == ETextCommit::OnEnter || InCommitMethod == ETextCommit::OnUserMovedFocus)
		{
			SetValue(FCString::Atof(*InText.ToString()));
		}
	}
	else
	{
		SetValue(Value);
	}
}

void UWidgetStudioModernSlider::SyncSliderValue(const float NewValue)
{
	SetValue(NewValue, true);
}

FText UWidgetStudioModernSlider::GetLabelText() const
{
	return Label;
}

void UWidgetStudioModernSlider::SetLabelText(const FText NewText)
{
	Label = NewText;
	SynchronizeProperties();
}

bool UWidgetStudioModernSlider::IsValueVisible() const
{
	return bDisplayValue;
}

bool UWidgetStudioModernSlider::IsHandleVisible() const
{
	return bDisplayHandle;
}

bool UWidgetStudioModernSlider::IsColorBasedOnValue() const
{
	return bColorBasedOnValue;
}

bool UWidgetStudioModernSlider::IsColorBasedOnValueReversed() const
{
	return bReverseColorOrder;
}

void UWidgetStudioModernSlider::SetReverseColorOrder(const bool bNewState)
{
	bReverseColorOrder = bNewState;
}

bool UWidgetStudioModernSlider::IsInputEnabled() const
{
	return bEnableInput;
}

void UWidgetStudioModernSlider::SetInputState(const bool NewState)
{
	bEnableInput = NewState;
	SynchronizeProperties();
	
}

void UWidgetStudioModernSlider::SetValueVisibility(const bool bNewState)
{
	bDisplayValue = bNewState;
	SynchronizeProperties();
}

void UWidgetStudioModernSlider::SetHandleVisibility(const bool bNewState)
{
	bDisplayHandle = bNewState;
	SynchronizeProperties();
}

void UWidgetStudioModernSlider::SetColorBasedOnValue(const bool bNewState)
{
	bColorBasedOnValue = bNewState;
	SynchronizeProperties();
}

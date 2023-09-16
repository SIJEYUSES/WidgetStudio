/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/Modern/WSModernTextField.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Components/VerticalBoxSlot.h"

TSharedRef<SWidget> UWidgetStudioModernTextField::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{		
		/* Construct Widgets */
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		VBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VBox"));
		LabelItem = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Label Item"));
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
		BackgroundScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Background Scale Box"));
		BackgroundOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Background Overlay"));
		DropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("DropDown"));
		Background = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background"));
		BackgroundOutline = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background Outline"));
		HBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HBox"));
		
		LeadingIconScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Leading Icon Scale Box"));
		LeadingIconItem = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Leading Icon"));
		TrailingIconScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Trailing Icon Scale Box"));
		TrailingIconItem = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Trailing Icon"));
		
		TextInput = WidgetTree->ConstructWidget<UWSEditableText>(UWSEditableText::StaticClass(), TEXT("Text Input"));
		TextInputMultiLine = WidgetTree->ConstructWidget<UCustomMultiLineTextBox>(UCustomMultiLineTextBox::StaticClass(), TEXT("Text Input Multi Line"));
			
		/* Construct Hierarchy */
		RootWidget->AddChild(VBox);
		VBox->AddChild(LabelItem);
		VBox->AddChild(SizeBox);

		SizeBox->AddChild(Overlay);
		Overlay->AddChild(BackgroundScaleBox);
		Overlay->AddChild(HBox);

		BackgroundScaleBox->AddChild(BackgroundOverlay);
		BackgroundOverlay->AddChild(DropShadow);
		BackgroundOverlay->AddChild(Background);
		BackgroundOverlay->AddChild(BackgroundOutline);

		HBox->AddChild(LeadingIconScaleBox);
		HBox->AddChild(TextInput);
		HBox->AddChild(TextInputMultiLine);
		HBox->AddChild(TrailingIconScaleBox);

		LeadingIconScaleBox->AddChild(LeadingIconItem);
		TrailingIconScaleBox->AddChild(TrailingIconItem);

		// Additional Construction Parameters
		bCanOverrideDimensions = true;
		
		InitializeStyling();
	}
	
	return Widget;
}

int32 UWidgetStudioModernTextField::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (bDisablePainting) { return LayerId; }
	
	/* Smoothly update size box */
	UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, GetDimensions().X, 0, AnimationTime);
	
	/* Smoothly update DropShadow opacity */
	const float NewDropShadowOpacity = TextInput->HasKeyboardFocus() || TextInputMultiLine->HasKeyboardFocus() || IsHovered() ? 0.5f : 0.35f;
	
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(DropShadow, NewDropShadowOpacity, AnimationTime);
	
	/* Smoothly update DropShadow location */
	const float NewDropShadowLoc = TextInput->HasKeyboardFocus() || TextInputMultiLine->HasKeyboardFocus() || IsHovered() ? 5.f : 1.f;
	UWidgetStudioFunctionLibrary::InterpWidgetTranslation(DropShadow, FVector2D(0, NewDropShadowLoc), AnimationTime);
	
	UWidgetStudioFunctionLibrary::InterpImageColor(Background, BackgroundColor, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpImageColor(BackgroundOutline, GetColorByState(), AnimationTime);
	UWidgetStudioFunctionLibrary::InterpImageColor(LeadingIconItem, ContentColor, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpImageColor(TrailingIconItem, ContentColor, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpTextColor(LabelItem, LabelColor, AnimationTime);
	
	return LayerId;
}

void UWidgetStudioModernTextField::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioModernTextField::InitializeStyling()
{
	Super::InitializeStyling();

	ScrollBarBrush = UWidgetStudioFunctionLibrary::GetRoundedSlateBrush();

	// Cache common parameters
	const float SizeY = GetDimensions().Y;
	const float RoundedSize = CornerStyle == ECornerStyle::Pill ?  SizeY : FMath::Clamp(static_cast<float>(GetBorderRadius()), 0.f, GetDimensions().Y);
	const FSlateFontInfo WSFont = TextStyle.Type == EWSFontType::Custom ?
		UWidgetStudioFunctionLibrary::ConstructFontInfoFromStyle(TextStyle.CustomStyle) : UWidgetStudioFunctionLibrary::ConstructFontInfoFromTypography(TextStyle.Type);

	// Initialize the SizeBox widget styling
	if (SizeBox)
	{
		SizeBox->SetMinDesiredHeight(GetDimensions().Y);
		SizeBox->SetMinDesiredWidth(GetDimensions().X);
		
		if (MaxDimensions.X > 0)
		{
			SizeBox->SetMaxDesiredWidth(MaxDimensions.X);
		}
		else
		{
			SizeBox->ClearMaxDesiredWidth();
		}

		if (MaxDimensions.Y > 0)
		{
			SizeBox->SetMaxDesiredHeight(MaxDimensions.Y);
		}
		else
		{
			SizeBox->ClearMaxDesiredHeight();
		}
		
		Cast<UVerticalBoxSlot>(SizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UVerticalBoxSlot>(SizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UVerticalBoxSlot>(SizeBox->Slot)->SetSize(bMultiLine ? ESlateSizeRule::Fill : ESlateSizeRule::Automatic);
	}

	// Initialize the VBox widget styling
	if (VBox)
	{
		Cast<UOverlaySlot>(VBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(VBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the LabelItem widget styling
	if (LabelItem)
	{
		LabelItem->SetFont(UWidgetStudioFunctionLibrary::ConstructFontInfoFromTypography(EWSFontType::Caption));
		LabelItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(LabelColor));
		
		Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetPadding(FMargin(0, 0, 0, 3));
	}
	
	// Initialize the Overlay widget styling
	if (Overlay)
	{
		Cast<USizeBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the BackgroundScaleBox widget styling
	if (BackgroundScaleBox)
	{
		BackgroundScaleBox->SetStretch(CornerStyle == ECornerStyle::Rounded || CornerStyle == ECornerStyle::Pill ? EStretch::Fill : EStretch::ScaleToFill);
		
		Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetPadding(0);
	}
	
	// Initialize the BackgroundOverlay widget styling
	if (BackgroundOverlay)
	{
		Cast<UScaleBoxSlot>(BackgroundOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UScaleBoxSlot>(BackgroundOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the DropShadow widget styling
	if (DropShadow)
	{
		DropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(DropShadow, RoundedSize, RoundedSize);
		DropShadow->SetColorAndOpacity(FLinearColor::Black);
		DropShadow->SetRenderOpacity(0.35f);

		FSlateBrush NewBrush = DropShadow->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		DropShadow->SetBrush(NewBrush);

		Cast<UOverlaySlot>(DropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(DropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	
	// Initialize the Background widget styling
	if (Background)
	{
		Background->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(Background, RoundedSize, RoundedSize);

		FSlateBrush NewBrush = Background->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		Background->SetBrush(NewBrush);

		Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(BackgroundColor));

		Cast<UOverlaySlot>(Background->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Background->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	
	// Initialize the BackgroundOutline widget styling
	if (BackgroundOutline)
	{
		BackgroundOutline->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedOutlineMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(BackgroundOutline, RoundedSize, RoundedSize);

		FSlateBrush NewBrush = BackgroundOutline->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		BackgroundOutline->SetBrush(NewBrush);

		BackgroundOutline->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(GetColorByState()));

		Cast<UOverlaySlot>(BackgroundOutline->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(BackgroundOutline->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	
	// Initialize the HBox widget styling
	if (HBox)
	{
		Cast<UOverlaySlot>(HBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(HBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UOverlaySlot>(HBox->Slot)->SetPadding(FMargin(15, 10, 15, 10));
	}
	
	// Initialize the LeadingIconScaleBox widget styling
	if (LeadingIconScaleBox)
	{
		Cast<UHorizontalBoxSlot>(LeadingIconScaleBox->Slot)->SetHorizontalAlignment(HAlign_Left);
		Cast<UHorizontalBoxSlot>(LeadingIconScaleBox->Slot)->SetVerticalAlignment(bMultiLine ? VAlign_Top : VAlign_Center);
		Cast<UHorizontalBoxSlot>(LeadingIconScaleBox->Slot)->SetPadding(FMargin(0, 2.5, 15, 2.5));
	}
	

	// Initialize the LeadingIconItem widget styling
	if (LeadingIconItem)
	{
		LeadingIconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
		
		Cast<UScaleBoxSlot>(LeadingIconItem->Slot)->SetHorizontalAlignment(HAlign_Center);
		Cast<UScaleBoxSlot>(LeadingIconItem->Slot)->SetVerticalAlignment(VAlign_Center);
	}
	
	// Initialize the TrailingIconScaleBox widget styling
	if (TrailingIconScaleBox)
	{
		Cast<UHorizontalBoxSlot>(TrailingIconScaleBox->Slot)->SetHorizontalAlignment(HAlign_Right);
		Cast<UHorizontalBoxSlot>(TrailingIconScaleBox->Slot)->SetVerticalAlignment(bMultiLine ? VAlign_Bottom : VAlign_Center);
		Cast<UHorizontalBoxSlot>(TrailingIconScaleBox->Slot)->SetPadding(FMargin(15, 2.5, 0, 2.5));
	}
	
	// Initialize the TrailingIconItem widget styling
	if (TrailingIconItem)
	{
		TrailingIconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
		
		Cast<UScaleBoxSlot>(TrailingIconItem->Slot)->SetHorizontalAlignment(HAlign_Center);
		Cast<UScaleBoxSlot>(TrailingIconItem->Slot)->SetVerticalAlignment(VAlign_Center);
	}
		
	// Initialize the TextInput widget styling
	if (TextInput)
	{
		
		TextInput->SetVisibility(bMultiLine ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		TextInput->SetFont(WSFont);
		
		Cast<UHorizontalBoxSlot>(TextInput->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UHorizontalBoxSlot>(TextInput->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UHorizontalBoxSlot>(TextInput->Slot)->SetSize(ESlateSizeRule::Fill);

		if (bMultiLine)
		{
			// Bind Single Line Text Change Events
			TextInput->OnTextChanged.RemoveAll(this);
			TextInput->OnTextCommitted.RemoveAll(this);
		}
		else
		{
			// Unbind Single Line Text Change Events
			TextInput->OnTextChanged.AddDynamic(this, &UWidgetStudioModernTextField::OnTextInputTextChanged);
			TextInput->OnTextCommitted.AddDynamic(this, &UWidgetStudioModernTextField::OnTextInputTextCommitted);
		}
	}

	// Initialize the TextInput widget styling
	if (TextInputMultiLine)
	{
		TextInputMultiLine->SetVisibility(bMultiLine ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		
		// Setup Style
		TextInputMultiLine->WidgetStyle.BackgroundImageNormal.DrawAs = ESlateBrushDrawType::NoDrawType;
		TextInputMultiLine->WidgetStyle.BackgroundImageHovered.DrawAs = ESlateBrushDrawType::NoDrawType;
		TextInputMultiLine->WidgetStyle.BackgroundImageFocused.DrawAs = ESlateBrushDrawType::NoDrawType;
		TextInputMultiLine->WidgetStyle.BackgroundImageReadOnly.DrawAs = ESlateBrushDrawType::NoDrawType;
		TextInputMultiLine->WidgetStyle.VScrollBarPadding = FMargin(5.f, 0.f, 5.f, 0.f);
		TextInputMultiLine->WidgetStyle.HScrollBarPadding = FMargin(0.f, 5.f, 0.f, 5.f);
		
		Cast<UHorizontalBoxSlot>(TextInputMultiLine->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UHorizontalBoxSlot>(TextInputMultiLine->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UHorizontalBoxSlot>(TextInputMultiLine->Slot)->SetSize(ESlateSizeRule::Fill);
		Cast<UHorizontalBoxSlot>(TextInputMultiLine->Slot)->SetPadding(FMargin(0, 0, 0, 0));

		if (bMultiLine)
		{
			// Bind Multi Line Text Change Events
			TextInputMultiLine->OnTextChanged.AddDynamic(this, &UWidgetStudioModernTextField::OnTextInputTextChanged);
			TextInputMultiLine->OnTextCommitted.AddDynamic(this, &UWidgetStudioModernTextField::OnTextInputTextCommitted);
		}
		else
		{
			// Unbind Multi Line Text Change Events
			TextInputMultiLine->OnTextChanged.RemoveAll(this);
			TextInputMultiLine->OnTextCommitted.RemoveAll(this);
		}
	}
}

void UWidgetStudioModernTextField::UpdateStyling()
{
	Super::UpdateStyling();

	const FSlateFontInfo WSFont = TextStyle.Type == EWSFontType::Custom ?
		UWidgetStudioFunctionLibrary::ConstructFontInfoFromStyle(TextStyle.CustomStyle) : UWidgetStudioFunctionLibrary::ConstructFontInfoFromTypography(TextStyle.Type);

	if (TextInput->HasKeyboardFocus() || TextInputMultiLine->HasKeyboardFocus())
	{
		if (CharacterLimit > 0)
		{
			const FText AdjustedText = FText::FromString(Text.ToString().Left(CharacterLimit));
			if (bMultiLine)
			{
				TextInputMultiLine->SetText(AdjustedText);
			}
			else
			{
				TextInput->SetText(AdjustedText);
			}
		}
		
		return;
	}


	// Update the SizeBox widget styling
	if (SizeBox)
	{
		SizeBox->SetMinDesiredHeight(GetDimensions().Y);
		SizeBox->SetMinDesiredWidth(GetDimensions().X);
	}

	if (TextInput)
	{
		TextInput->SetVisibility(bMultiLine ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		TextInput->SetText(Text);
		TextInput->SetHintText(PlaceholderText);
		TextInput->SetIsPassword(bIsPasswordField);
		TextInput->WidgetStyle.ColorAndOpacity = UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor);
	}



	if (LabelItem)
	{
		LabelItem->SetText(Label);
		LabelItem->SetVisibility(Label.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	if (LeadingIconItem)
	{
		LeadingIconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(LeadingIcon), false);
		UWidgetStudioFunctionLibrary::SetBrushImageSize(LeadingIconItem, GetIconSize(), GetIconSize());
	}

	if (TrailingIconItem)
	{
		TrailingIconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(TrailingIcon), false);
		UWidgetStudioFunctionLibrary::SetBrushImageSize(TrailingIconItem, GetIconSize(), GetIconSize());
	}

	if (LeadingIconItem && TrailingIconItem)
	{
		LeadingIconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(LeadingIcon), false);
		TrailingIconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(TrailingIcon), false);
		if (IconVisibility == ETextFieldIconVisibility::LeadingAndTrailing)
		{
			LeadingIconScaleBox->SetVisibility(ESlateVisibility::HitTestInvisible);
			TrailingIconScaleBox->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (IconVisibility == ETextFieldIconVisibility::Leading)
		{
			LeadingIconScaleBox->SetVisibility(ESlateVisibility::HitTestInvisible);
			TrailingIconScaleBox->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IconVisibility == ETextFieldIconVisibility::Trailing)
		{
			LeadingIconScaleBox->SetVisibility(ESlateVisibility::Collapsed);
			TrailingIconScaleBox->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	
		if (IconVisibility == ETextFieldIconVisibility::Hidden)
		{
			LeadingIconScaleBox->SetVisibility(ESlateVisibility::Collapsed);
			TrailingIconScaleBox->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

EPalette UWidgetStudioModernTextField::GetColorByState() const
{
	if (State == ETextFieldState::Normal) { return DefaultOutline; }
	if (State == ETextFieldState::Accepted) { return EPalette::Green; }
	if (State == ETextFieldState::Error) { return EPalette::Red; }
	if (State == ETextFieldState::Warning) { return EPalette::Yellow; }
	if (State == ETextFieldState::Focused) { return ContentColor; }

	return EPalette::Transparent;
}

bool UWidgetStudioModernTextField::IsMultiLineSupportEnabled() const
{
	return bMultiLine;
}

void UWidgetStudioModernTextField::SetMultiLineSupportEnabled(const bool bNewState)
{
	bMultiLine = bNewState;
	SynchronizeProperties();
}

ETextFieldIconVisibility UWidgetStudioModernTextField::GetIconVisibility() const
{
	return IconVisibility;
}

EIconItem UWidgetStudioModernTextField::GetLeadingIcon() const
{
	return LeadingIcon;
}

EIconItem UWidgetStudioModernTextField::GetTrailingIcon() const
{
	return TrailingIcon;
}

EPalette UWidgetStudioModernTextField::GetBackgroundColor() const
{
	return BackgroundColor;
}

EPalette UWidgetStudioModernTextField::GetContentColor() const
{
	return ContentColor;
}

float UWidgetStudioModernTextField::GetIconSize() const
{
	return UWidgetStudioFunctionLibrary::GetSizeByModifier(SizeModifier, IconSize);
}

void UWidgetStudioModernTextField::SetIconSize(const float NewSize)
{
	IconSize = NewSize;
	SynchronizeProperties();
}

void UWidgetStudioModernTextField::SetIconVisibility(const ETextFieldIconVisibility NewState)
{
	IconVisibility = NewState;
	SynchronizeProperties();
}

void UWidgetStudioModernTextField::SetLeadingIcon(const EIconItem NewIcon)
{
	LeadingIcon = NewIcon;
	SynchronizeProperties();
}

void UWidgetStudioModernTextField::SetTrailingIcon(const EIconItem NewIcon)
{
	TrailingIcon = NewIcon;
	SynchronizeProperties();
}

void UWidgetStudioModernTextField::SetBackgroundColor(const EPalette NewColor)
{
	BackgroundColor = NewColor;
}

void UWidgetStudioModernTextField::SetContentColor(const EPalette NewColor)
{
	ContentColor = NewColor;
}

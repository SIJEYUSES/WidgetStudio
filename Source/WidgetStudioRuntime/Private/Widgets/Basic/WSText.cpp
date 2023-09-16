/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/Basic/WSText.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

TSharedRef<SWidget> UWidgetStudioText::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;

	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{
		/* Construct Widgets */
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Size Box"));
		TextItem = WidgetTree->ConstructWidget<UWidgetStudioTextBlock>(UWidgetStudioTextBlock::StaticClass(), TEXT("Text Item"));
			
		/* Construct Hierarchy */
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(TextItem);

		// Additional Construction Parameters
		bCanOverrideDimensions = false;
		bCanOverrideBorderRadius = false;

		// Initialize TextStyle - Will eventually replace current variables with this
		TextStyle = FWSTextStyle(
			Type,
			CustomStyle,
			Case,
			Justification,
			AutoWrapText,
			WrapTextAt,
			WrappingPolicy
		);
		
		InitializeStyling();
		UpdateStyling();
				
	}
	return Widget;
}

int32 UWidgetStudioText::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	if (bDisablePainting) { return LayerId; }
	
	// Adjusts icon color
	UWidgetStudioFunctionLibrary::InterpTextColor(TextItem, Color, AnimationTime);
	
	return LayerId;
}

void UWidgetStudioText::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	// Update TextStyle - Will eventually replace current variables with the struct
	TextStyle = FWSTextStyle(
			Type,
			CustomStyle,
			Case,
			Justification,
			AutoWrapText,
			WrapTextAt,
			WrappingPolicy
		);
	UpdateStyling();
}

void UWidgetStudioText::InitializeStyling()
{
	Super::InitializeStyling();

	// Initialize the Size Box widget styling
	if (SizeBox)
	{
		Cast<UOverlaySlot>(SizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(SizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	// Initialize the TextItem widget styling
	if (TextItem)
	{
		TextItem->SetColorAndOpacity(FSlateColor(UWidgetStudioFunctionLibrary::GetColorFromPalette(Color)));
	}
}

void UWidgetStudioText::UpdateStyling()
{
	Super::UpdateStyling();

	// Update the Size Box widget styling
	if (SizeBox)
	{
		// Adjust the widget to match the WrapTextAt value.
		// The WrapTextAt is used for the width of the SizeBox due to the built in Unreal WrapTextAt function not working as intended.
		if (TextStyle.WrapTextAt > 0)
		{
			SizeBox->SetWidthOverride(TextStyle.WrapTextAt);
		}
	}

	// Update the TextItem widget styling
	if (TextItem)
	{
		FontInfo = TextStyle.Type ==
			EWSFontType::Custom ? UWidgetStudioFunctionLibrary::ConstructFontInfoFromStyle(TextStyle.CustomStyle) :
			UWidgetStudioFunctionLibrary::ConstructFontInfoFromTypography(TextStyle.Type);
		FontInfo.Size = UWidgetStudioFunctionLibrary::GetSizeByModifier(SizeModifier, FontInfo.Size);
		TextItem->SetFont(FontInfo);
		TextItem->SetJustification(TextStyle.Justification);
		TextItem->SetWrappingPolicy(TextStyle.WrappingPolicy);
		TextItem->SetWrapTextAt(TextStyle.WrapTextAt);
		TextItem->SetAutoWrapText(TextStyle.AutoWrapText);
		TextItem->SetText(
			TextStyle.Case == EFontCase::Uppercase ? Text.ToUpper() :
			TextStyle.Case == EFontCase::Lowercase ? Text.ToLower() :
			Text);
		
		if (bDisablePainting)
		{
			TextItem->SetColorAndOpacity(FSlateColor(UWidgetStudioFunctionLibrary::GetColorFromPalette(Color)));
		}
	}
}

FWSTextStyle UWidgetStudioText::GetTextOptions() const
{
	return TextStyle;
}

void UWidgetStudioText::SetTextStyle(const FWSTextStyle NewTextOptions)
{
	TextStyle = NewTextOptions;
	UpdateStyling();
}

FText UWidgetStudioText::GetText() const
{
	return Text;
}

bool UWidgetStudioText::IsTextEmpty() const
{
	return Text.IsEmpty();
}

ETextJustify::Type UWidgetStudioText::GetJustification() const
{
	return TextStyle.Justification;
}

int32 UWidgetStudioText::Length() const
{
	return Text.ToString().Len();
}

EWSFontType UWidgetStudioText::GetType() const
{
	return TextStyle.Type;
}

FFontStyle UWidgetStudioText::GetCustomStyle() const
{
	return TextStyle.CustomStyle;
}

void UWidgetStudioText::SetCustomStyle(const FFontStyle NewStyle)
{
	TextStyle.CustomStyle = NewStyle;
	UpdateStyling();
}

bool UWidgetStudioText::IsAutoWrappingText() const
{
	return TextStyle.AutoWrapText;
}

float UWidgetStudioText::GetWrapTextAt() const
{
	return TextStyle.WrapTextAt;
}

ETextWrappingPolicy UWidgetStudioText::GetWrapPolicy() const
{
	return TextStyle.WrappingPolicy;
}

EFontCase UWidgetStudioText::GetCase() const
{
	return TextStyle.Case;
}

void UWidgetStudioText::SetCase(const EFontCase NewCase)
{
	TextStyle.Case = NewCase;
	UpdateStyling();
}

void UWidgetStudioText::SetText(const FText NewText)
{
	Text = NewText;
	UpdateStyling();
}

void UWidgetStudioText::SetJustification(const ETextJustify::Type NewJustification)
{
	TextStyle.Justification = NewJustification;
	UpdateStyling();
}

void UWidgetStudioText::SetColor(const EPalette NewColor)
{
	Color = NewColor;
	UpdateStyling();
}

void UWidgetStudioText::SetType(const EWSFontType NewType)
{
	TextStyle.Type = NewType;
	UpdateStyling();
}

void UWidgetStudioText::SetAutoWrapText(const bool NewState)
{
	TextStyle.AutoWrapText = NewState;
	UpdateStyling();
}

void UWidgetStudioText::SetWrapTextAt(const float NewWrapAt)
{
	TextStyle.WrapTextAt = NewWrapAt;
	UpdateStyling();
}

void UWidgetStudioText::SetWrapPolicy(const ETextWrappingPolicy NewPolicy)
{
	TextStyle.WrappingPolicy = NewPolicy;
	UpdateStyling();
}

/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/Modern/WSModernCheckBox.h"

#include "Components/PanelWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"

TSharedRef<SWidget> UWidgetStudioModernCheckBox::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{

		// Construct Widgets
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
		BackgroundScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Background Scale Box"));
		BackgroundOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Background Overlay"));
		DropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("DropDown"));
		Background = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background"));
		BackgroundOutline = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background Outline"));
		CheckScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Check Scale Box"));
		HBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HBox"));
		CheckOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Check Overlay"));
		CheckBackground = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Check Background Item"));
		CheckOutline = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Check Outline Item"));
		IconItem = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Icon Item"));
		TextItem = WidgetTree->ConstructWidget<UWidgetStudioText>(UWidgetStudioText::StaticClass(), TEXT("Text Item"));
		SpacerItem = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass(), TEXT("Spacer Item"));
			
		// Construct Hierarchy
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(Overlay);
		
		Overlay->AddChild(BackgroundScaleBox);
		BackgroundScaleBox->AddChild(BackgroundOverlay);
		BackgroundOverlay->AddChild(DropShadow);
		BackgroundOverlay->AddChild(Background);
		BackgroundOverlay->AddChild(BackgroundOutline);
		
		Overlay->AddChild(HBox);
		HBox->AddChild(CheckScaleBox);
		CheckScaleBox->AddChild(CheckOverlay);
		CheckOverlay->AddChild(CheckBackground);
		CheckOverlay->AddChild(CheckOutline);
		CheckOverlay->AddChild(IconItem);
		HBox->AddChild(SpacerItem);
		HBox->AddChild(TextItem);

		// Additional Construction Parameters
		MinimumDimensions = FVector2D(65, 16);
		bForceCheckable = true;
		bCanOverrideBorderRadius = false;
		
		InitializeStyling();
		SetCheckable(true);
	}
	return Widget;
}

int32 UWidgetStudioModernCheckBox::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (bDisablePainting) { return LayerId; }
	
	/* Smoothly update size */
	const float SizeX = GetDimensions().X;
	const float SizeY = GetDimensions().Y;
	UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, SizeX, SizeY, 0);
	UWidgetStudioFunctionLibrary::InterpBrushImageSize(DropShadow, SizeY, SizeY, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpBrushImageSize(Background, SizeY, SizeY, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpBrushImageSize(BackgroundOutline, SizeY, SizeY, AnimationTime);
	
	UWidgetStudioFunctionLibrary::InterpBrushImageSize(CheckBackground, SizeY, SizeY, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpBrushImageSize(CheckOutline, SizeY, SizeY, AnimationTime);
	
	/* Smoothly update DropShadow opacity */
	const float NewDropShadowOpacity = IsHovered() ? 0.5f : 0.35f;
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(DropShadow, NewDropShadowOpacity, AnimationTime);
	
	/* Smoothly update DropShadow location */
	const float NewDropShadowLoc = IsHovered() ? 5.f : 1.f;
	UWidgetStudioFunctionLibrary::InterpWidgetTranslation(DropShadow, FVector2D(0, NewDropShadowLoc), AnimationTime);

	/* Smoothly update background color */
	UWidgetStudioFunctionLibrary::InterpImageColor(Background, BackgroundColor, AnimationTime);
	
	/* Smoothly update background opacity */
	const float NewBackgroundOpacity = Style == ECheckboxStyle::Modern ? 1.f : 0.f;
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(Background, NewBackgroundOpacity, AnimationTime);
		
	/* Smoothly update background outline color */
	UWidgetStudioFunctionLibrary::InterpImageColor(BackgroundOutline, CheckedColor, AnimationTime);

	/* Smoothly update background outline opacity */
	const float NewBackgroundOutlineOpacity = Clusivity == EClusivity::Exclusive && IsChecked() == true ? 1.0f : 0.0f;
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(BackgroundOutline, NewBackgroundOutlineOpacity, AnimationTime);
	
	/* Smoothly update check background color */
	UWidgetStudioFunctionLibrary::InterpImageColor(CheckBackground, IsChecked() ? CheckedColor : BackgroundColor, AnimationTime);

	/* Smoothly update check background opacity */
	const float NewCheckBackgroundOpacity = Style == ECheckboxStyle::Modern || IsChecked() ? 1.f : 0.f;
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(CheckBackground, NewCheckBackgroundOpacity, AnimationTime);
	
	/* Smoothly update check outline color */
	UWidgetStudioFunctionLibrary::InterpImageColor(CheckOutline, ContentColor, AnimationTime);
	
	/* Smoothly update check outline opacity */
	const float NewCheckOutlineOpacity = IsChecked() ? 0.0f : 0.35f;
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(CheckOutline, NewCheckOutlineOpacity, AnimationTime);
	
	/* Smoothly update check icon color */
	UWidgetStudioFunctionLibrary::InterpImageColor(IconItem, BackgroundColor, AnimationTime);
	
	/* Smoothly update check icon opacity */
	const float NewIconOpacity = IsChecked() ? 1.f : 0.f;
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(IconItem, NewIconOpacity, AnimationTime);
	
	/* Smoothly update text color */
	UWidgetStudioFunctionLibrary::InterpWidgetColor(TextItem, ContentColor, AnimationTime);
	
	/* Smoothly update check icon opacity */
	const float NewTextOpacity = IsChecked() ? 1.f : 0.85f;
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(TextItem, NewTextOpacity, AnimationTime);
	
	return LayerId;
}

void UWidgetStudioModernCheckBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioModernCheckBox::InitializeStyling()
{
	// Cache SizeY
	const float SizeY = GetDimensions().Y;

	// Get roundness value based on current styling
	const float CheckBackgroundRoundness = Clusivity == EClusivity::Exclusive || Style == ECheckboxStyle::Modern ? SizeY : 5;

	// Initialize the BackgroundScaleBox widget styling
	if (BackgroundScaleBox)
	{
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
	
	// 初始化 DropShadow 部件的样式
	if (DropShadow)
	{
		DropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(DropShadow, SizeY, SizeY);
		DropShadow->SetColorAndOpacity(FLinearColor::Black);
		DropShadow->SetRenderOpacity(0.35f);

		FSlateBrush NewBrush = DropShadow->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		DropShadow->SetBrush(NewBrush);

		DropShadow->SetVisibility(Style == ECheckboxStyle::Modern ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		Cast<UOverlaySlot>(DropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(DropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	// 初始化 Background 部件的样式
	if (Background)
	{
		Background->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(Background, SizeY, SizeY);

		FSlateBrush NewBrush = Background->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		Background->SetBrush(NewBrush);

		Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(BackgroundColor));

		Cast<UOverlaySlot>(Background->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Background->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	// 初始化 BackgroundOutline 部件的样式
	if (BackgroundOutline)
	{
		BackgroundOutline->SetOpacity(0.f);
		BackgroundOutline->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedOutlineMaterial());
		FSlateBrush NewBrush = BackgroundOutline->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		BackgroundOutline->SetBrush(NewBrush);
		UWidgetStudioFunctionLibrary::SetBrushImageSize(BackgroundOutline, SizeY, SizeY);
		BackgroundOutline->SetVisibility(Style == ECheckboxStyle::Modern ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		BackgroundOutline->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(CheckedColor));

		Cast<UOverlaySlot>(BackgroundOutline->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(BackgroundOutline->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	// 初始化 Overlay 部件的样式
	if (Overlay)
	{
		Cast<USizeBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	// 初始化 HBox 部件的样式
	if (HBox)
	{
		Cast<UOverlaySlot>(HBox->Slot)->SetHorizontalAlignment(HAlign_Center);
		Cast<UOverlaySlot>(HBox->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UOverlaySlot>(HBox->Slot)->SetPadding(FVector4(15, 5, 15, 5));
	}

	// 初始化 CheckScaleBox 部件的样式
	if (CheckScaleBox)
	{
		CheckScaleBox->SetStretch(EStretch::ScaleToFitY);
		CheckScaleBox->SetVisibility(Clusivity == EClusivity::Inclusive || Style == ECheckboxStyle::Classic ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		Cast<UHorizontalBoxSlot>(CheckScaleBox->Slot)->SetPadding(2.5);
		Cast<UHorizontalBoxSlot>(CheckScaleBox->Slot)->SetHorizontalAlignment(HAlign_Center);
		Cast<UHorizontalBoxSlot>(CheckScaleBox->Slot)->SetVerticalAlignment(VAlign_Center);
	}

	// 初始化 CheckBackground 部件的样式
	if (CheckBackground)
	{
		CheckBackground->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(CheckBackground, CheckBackgroundRoundness, CheckBackgroundRoundness);
		FSlateBrush NewBrush = CheckBackground->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		CheckBackground->SetBrush(NewBrush);
		CheckBackground->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(IsChecked() ? CheckedColor : BackgroundColor));

		Cast<UOverlaySlot>(CheckBackground->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(CheckBackground->Slot)->SetVerticalAlignment(VAlign_Fill);
	}

	// 初始化 CheckOutline 部件的样式
	if (CheckOutline)
	{
		CheckOutline->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedOutlineMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(CheckOutline, CheckBackgroundRoundness, CheckBackgroundRoundness);

		FSlateBrush NewBrush = CheckOutline->GetBrush();
		NewBrush.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		CheckOutline->SetBrush(NewBrush);

		CheckOutline->SetOpacity(IsChecked() ? 0.0f : 0.35f);
		CheckOutline->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));

		Cast<UOverlaySlot>(CheckOutline->Slot)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		Cast<UOverlaySlot>(CheckOutline->Slot)->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	}



	// 初始化 IconItem 部件的样式
	if (IconItem)
	{
		UWidgetStudioFunctionLibrary::SetBrushImageSize(IconItem, SizeY / 2, SizeY / 2);
		IconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(BackgroundColor));

		Cast<UOverlaySlot>(IconItem->Slot)->SetHorizontalAlignment(HAlign_Center);
		Cast<UOverlaySlot>(IconItem->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UOverlaySlot>(IconItem->Slot)->SetPadding(Clusivity == EClusivity::Inclusive ? 2.5 : 0);
	}

	// 初始化 SpacerItem 部件的样式
	if (SpacerItem)
	{
		SpacerItem->SetSize(FVector2D(15, 0));
		SpacerItem->SetVisibility(Clusivity == EClusivity::Inclusive ||
			Style == ECheckboxStyle::Classic ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed
		);
	}

	// 初始化 TextItem 部件的样式
	if (TextItem)
	{
		TextItem->bDisablePainting = true;
		TextItem->SetColor(EPalette::White);
		TextItem->SetType(EWSFontType::Caption);
		TextItem->SetJustification(Clusivity == EClusivity::Inclusive ? ETextJustify::Left : ETextJustify::Center);
		TextItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));

		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetHorizontalAlignment(HAlign_Center);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetVerticalAlignment(VAlign_Center);
	}

}

void UWidgetStudioModernCheckBox::UpdateStyling()
{
	// Update the SizeBox widget styling
	if (SizeBox)
	{
		SizeBox->SetMinDesiredWidth(GetDimensions().Y * 2.5);
		SizeBox->SetHeightOverride(GetDimensions().Y);
	}

	// Update the TextItem widget styling
	if (TextItem)
	{
		TextItem->SetText(Text);
		TextItem->SizeModifier = SizeModifier;
	}

	// Update the IconItem widget styling
	if (IconItem)
	{
		IconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(Icon), false);
	}
}

FText UWidgetStudioModernCheckBox::GetLabel() const
{
	return Text;
}

void UWidgetStudioModernCheckBox::SetText(const FText NewText)
{
	Text = NewText;
	SynchronizeProperties();
}

EIconItem UWidgetStudioModernCheckBox::GetIcon() const
{
	return Icon;
}

EPalette UWidgetStudioModernCheckBox::GetBackgroundColor() const
{
	return BackgroundColor;
}

EPalette UWidgetStudioModernCheckBox::GetContentColor() const
{
	return ContentColor;
}

EPalette UWidgetStudioModernCheckBox::GetCheckedColor() const
{
	return CheckedColor;
}

EClusivity UWidgetStudioModernCheckBox::GetSelectionMethod() const
{
	return Clusivity;
}

void UWidgetStudioModernCheckBox::SetSelectionMethod(const EClusivity NewState)
{
	Clusivity = NewState;
}

void UWidgetStudioModernCheckBox::SetIcon(const EIconItem NewIcon)
{
	Icon = NewIcon;
	SynchronizeProperties();
}

void UWidgetStudioModernCheckBox::SetBackgroundColor(const EPalette NewColor)
{
	BackgroundColor = NewColor;
}

void UWidgetStudioModernCheckBox::SetContentColor(const EPalette NewColor)
{
	ContentColor = NewColor;
}

void UWidgetStudioModernCheckBox::SetCheckedColor(const EPalette NewColor)
{
	CheckedColor = NewColor;
}



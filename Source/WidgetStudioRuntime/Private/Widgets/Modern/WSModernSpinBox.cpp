/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/Modern/WSModernSpinBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBoxSlot.h"
#include "Kismet/KismetInputLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "Framework/Application/SlateApplication.h"

TSharedRef<SWidget> UWidgetStudioModernSpinBox::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{		
		/* Construct Widgets */
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		BackgroundIndicator = WidgetTree->ConstructWidget<UWidgetStudioModernCard>(UWidgetStudioModernCard::StaticClass(), TEXT("Background Accent"));
		HorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HBox"));
		LabelItem = WidgetTree->ConstructWidget<UWidgetStudioText>(UWidgetStudioText::StaticClass(), TEXT("LabelItem"));
		BackgroundOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Background Overlay"));
		Background = WidgetTree->ConstructWidget<UWidgetStudioModernCard>(UWidgetStudioModernCard::StaticClass(), TEXT("Background"));
		BackgroundFill = WidgetTree->ConstructWidget<UWidgetStudioModernCard>(UWidgetStudioModernCard::StaticClass(), TEXT("Background Fill"));
		ContentHorizontalBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("Content HBox"));
		ArrowLeft = WidgetTree->ConstructWidget<UWidgetStudioIcon>(UWidgetStudioIcon::StaticClass(), TEXT("Arrow Left"));
		ArrowRight = WidgetTree->ConstructWidget<UWidgetStudioIcon>(UWidgetStudioIcon::StaticClass(), TEXT("Arrow Right"));
		ValueInput = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("Value input"));

		/* Construct Hierarchy */
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(Overlay);
		Overlay->AddChild(BackgroundIndicator);
		Overlay->AddChild(HorizontalBox);
		HorizontalBox->AddChild(LabelItem);
		HorizontalBox->AddChild(BackgroundOverlay);
		BackgroundOverlay->AddChild(Background);
		BackgroundOverlay->AddChild(BackgroundFill);
		BackgroundOverlay->AddChild(ContentHorizontalBox);
		ContentHorizontalBox->AddChild(ArrowLeft);
		ContentHorizontalBox->AddChild(ValueInput);
		ContentHorizontalBox->AddChild(ArrowRight);
		
		// Additional Construction Parameters
		bCanOverrideBorderRadius = true;
		bCanOverrideDimensions = true;
		MinimumDimensions = FVector2D(bEnableArrowButtons ? 125 : 100, 25);
		
		InitializeStyling();

	}
	return Widget;
}

int32 UWidgetStudioModernSpinBox::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (bDisablePainting) { return LayerId; } 
	
	/* Smoothly update size */
	UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(SizeBox, 0, GetDimensions().Y * .75f, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, GetDimensions().X, 0, AnimationTime);

	/*
	 * Smoothly Update Background Fill
	 * - Updates the background fill opacity based on the clamp method. If clamped, the fill is visible. Otherwise, hidden.
	 */
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(BackgroundFill, ClampMethod==EValueClamp::Clamped ? 0.5f : 0.f, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpWidgetScale(BackgroundFill,FVector2D(UKismetMathLibrary::MapRangeClamped(Value, Min, Max, 0, 1), 1), AnimationTime);

	/*
	 * Smoothly update the arrow icons
	 */
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(ArrowRight, IsHovered() ? .75f : .0f, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(ArrowLeft, IsHovered() ? .75f : .0f, AnimationTime);
	
	return LayerId;
}

FReply UWidgetStudioModernSpinBox::NativeOnMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// Only accept left mouse button presses
	if (!InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton"))) { return FReply::Unhandled(); }

	UE_LOG(LogWidgetStudio, Verbose, TEXT("Left Mouse Button Down"));
	
	// Reduce the value if the left mouse area was clicked.
	if(bEnableArrowButtons && ArrowLeft->IsHovered())
	{
		UE_LOG(LogWidgetStudio, Verbose, TEXT("Left Arrow Pressed"));
		Decrement();
		bIsLeftArrowPressed = true;
	}
	
	// Increase the value if the right mouse area was clicked.
	if(bEnableArrowButtons && ArrowRight->IsHovered())
	{
		
		UE_LOG(LogWidgetStudio, Verbose, TEXT("Right Arrow Pressed"));
		Increment();
		bIsRightArrowPressed = true;
	}
	
	bIsPressed = true;
	return FReply::Handled();
}

FReply UWidgetStudioModernSpinBox::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseWheel(InGeometry, InMouseEvent);

	if (!bEnableMouseWheelInput || !ValueInput->HasKeyboardFocus()) { return FReply::Unhandled(); }

	if (InMouseEvent.GetWheelDelta() > 0)
	{
		Increment();
	}
	else
	{
		Decrement();
	}
	
	return FReply::Handled();
}

void UWidgetStudioModernSpinBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioModernSpinBox::InitializeStyling()
{
	Super::InitializeStyling();

	// Setup SizeBox
	if (SizeBox)
	{
		SizeBox->SetMinDesiredWidth(GetDimensions().X);
		SizeBox->SetHeightOverride(GetDimensions().Y * .75f);
		Cast<UOverlaySlot>(SizeBox->Slot)->SetHorizontalAlignment(HAlign_Left);
		Cast<UOverlaySlot>(SizeBox->Slot)->SetVerticalAlignment(VAlign_Top);
	}
	
	// Setup Overlay
	if (Overlay)
	{
		Cast<USizeBoxSlot>(Overlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(Overlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Setup Background Indicator
	if (BackgroundIndicator)
	{
		BackgroundIndicator->SetColor(IndicatorColor);
		BackgroundIndicator->SetShadowStyle(EShadowStyle::Small);
		BackgroundIndicator->SetShadowHoverAnimationEnabled(true);
		Cast<UOverlaySlot>(BackgroundIndicator->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(BackgroundIndicator->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Setup Horizontal Box
	if (HorizontalBox)
	{
		Cast<UOverlaySlot>(HorizontalBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(HorizontalBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Setup Label
	if (LabelItem)
	{
		LabelItem->SetColor(ContentColor);
		LabelItem->SetType(EWSFontType::Caption);
		Cast<UHorizontalBoxSlot>(LabelItem->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UHorizontalBoxSlot>(LabelItem->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UHorizontalBoxSlot>(LabelItem->Slot)->SetPadding(FMargin(5, 0, 5, 0));
	}
	
	// Setup Background Overlay
	if (BackgroundOverlay)
	{
		Cast<UHorizontalBoxSlot>(BackgroundOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UHorizontalBoxSlot>(BackgroundOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UHorizontalBoxSlot>(BackgroundOverlay->Slot)->SetSize(ESlateSizeRule::Fill);
	}
	
	// Setup Background
	if (Background)
	{
		Background->SetShadowStyle(EShadowStyle::None);
		Background->SetColor(BackgroundColor);
		Cast<UOverlaySlot>(Background->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Background->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Setup Background Fill
	if (BackgroundFill)
	{
		BackgroundFill->SetShadowStyle(EShadowStyle::None);
		BackgroundFill->SetColor(IndicatorColor);
		BackgroundFill->SetRenderTransformPivot(FVector2D(0, .5));
		BackgroundFill->SetRenderScale(FVector2D(UKismetMathLibrary::MapRangeClamped(Value, Min, Max, 0, 1), 1));
		BackgroundFill->SetRenderOpacity(ClampMethod==EValueClamp::Clamped ? 1.f : 0.f);
		Cast<UOverlaySlot>(BackgroundFill->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(BackgroundFill->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Setup Content Horizontal Box
	if (ContentHorizontalBox)
	{
		Cast<UOverlaySlot>(ContentHorizontalBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(ContentHorizontalBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UOverlaySlot>(ContentHorizontalBox->Slot)->SetPadding(FMargin(5, 0, 5, 0));
	}
	
	// Setup Arrow Left
	if (ArrowLeft)
	{
		ArrowLeft->bDisablePainting = true;
		ArrowLeft->SetColor(ContentColor);
		ArrowLeft->SetIcon(EIconItem::ChevronLeft);
		ArrowLeft->SetSize(GetDimensions().Y * .3);
		ArrowLeft->SetRenderOpacity(.0f);
		ArrowLeft->SetVisibility(bEnableArrowButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		Cast<UHorizontalBoxSlot>(ArrowLeft->Slot)->SetHorizontalAlignment(HAlign_Left);
		Cast<UHorizontalBoxSlot>(ArrowLeft->Slot)->SetVerticalAlignment(VAlign_Center);
	}
	
	// Setup Arrow Right
	if (ArrowRight)
	{
		ArrowRight->bDisablePainting = true;
		ArrowRight->SetColor(ContentColor);
		ArrowRight->SetIcon(EIconItem::ChevronRight);
		ArrowRight->SetSize(GetDimensions().Y * .3);
		ArrowRight->SetRenderOpacity(.0f);
		ArrowRight->SetVisibility(bEnableArrowButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		Cast<UHorizontalBoxSlot>(ArrowRight->Slot)->SetHorizontalAlignment(HAlign_Right);
		Cast<UHorizontalBoxSlot>(ArrowRight->Slot)->SetVerticalAlignment(VAlign_Center);
	}
	
	// Setup Value Input
	if (ValueInput)
	{
		ValueInput->SetJustification(ETextJustify::Center);
		ValueInput->WidgetStyle.SetForegroundColor(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
		ValueInput->WidgetStyle.SetFont(UWidgetStudioFunctionLibrary::ConstructFontInfoFromTypography(EWSFontType::Caption));
		ValueInput->WidgetStyle.BackgroundImageNormal.DrawAs = ESlateBrushDrawType::NoDrawType;
		ValueInput->WidgetStyle.BackgroundImageHovered.DrawAs = ESlateBrushDrawType::NoDrawType;
		ValueInput->WidgetStyle.BackgroundImageFocused.DrawAs = ESlateBrushDrawType::NoDrawType;
		ValueInput->WidgetStyle.BackgroundImageReadOnly.DrawAs = ESlateBrushDrawType::NoDrawType;
		ValueInput->KeyboardType = EVirtualKeyboardType::Password;

		// Use setters for SelectAllTextWhenFocused and ClearKeyboardFocusOnCommit
		ValueInput->SetSelectAllTextWhenFocused(true);
		ValueInput->SetClearKeyboardFocusOnCommit(true);

		Cast<UHorizontalBoxSlot>(ValueInput->Slot)->SetSize(ESlateSizeRule::Fill);
		Cast<UHorizontalBoxSlot>(ValueInput->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UHorizontalBoxSlot>(ValueInput->Slot)->SetVerticalAlignment(VAlign_Fill);

		ValueInput->OnTextChanged.RemoveAll(this);
		ValueInput->OnTextCommitted.RemoveAll(this);
		ValueInput->OnTextCommitted.AddDynamic(this, &UWidgetStudioModernSpinBox::OnTextCommitted);
	}

}

void UWidgetStudioModernSpinBox::UpdateStyling()
{
	Super::UpdateStyling();

	// Updated minimum dimensions based on arrow button visibility
	MinimumDimensions = FVector2D(bEnableArrowButtons ? 125 : 100, 25);

	// Update Background Indicator
	if (BackgroundIndicator)
	{
		BackgroundIndicator->SetColor(IndicatorColor);
	}
	
	// Update Background
	if (Background)
	{
		Background->SetColor(BackgroundColor);
	}
	
	// Update Background Fill
	if (BackgroundFill)
	{
		BackgroundFill->SetColor(IndicatorColor);
	}
	
	// Update Arrows
	if (ArrowLeft)
	{
		ArrowLeft->SetColor(ContentColor);
		ArrowLeft->SetVisibility(bEnableArrowButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (ArrowRight)
	{
		ArrowRight->SetColor(ContentColor);
		ArrowRight->SetVisibility(bEnableArrowButtons ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	
	// Update Label
	if (LabelItem)
	{
		LabelItem->SetText(Label);
		LabelItem->SetColor(ContentColor);
	}
	
	// Update Value Input
	if (ValueInput)
	{
		ValueInput->SetText(GetValueAsText());
		ValueInput->WidgetStyle.SetForegroundColor(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
	}
}

void UWidgetStudioModernSpinBox::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Cancel tick functionality if widget isn't currently being pressed
	if (!bIsPressed) { return; }
	
	// Detect if the left mouse button has been un-pressed off widget
	if (!FSlateApplication::Get().GetPressedMouseButtons().Includes({FKey("LeftMouseButton")}))
	{
		bIsPressed = false;
        bIsLeftArrowPressed = false;
        bIsRightArrowPressed = false;
        PressTime = 0.f;
		LastMousePos = 0.f;
		return;
	}
	
	// Track tick time
	PressTime += InDeltaTime;

	// Wait until press has been held for .35f delta time
	if (PressTime < .35f) { return; }

	// Gate the tick timing
	if (InDeltaTime > .0025f)
	{
		if (bEnableArrowButtons && bIsLeftArrowPressed)
		{
			Decrement();
			return;
		}

		if (bEnableArrowButtons && bIsRightArrowPressed)
		{
			Increment();
			return;
		}

		UpdateValueOnMouseMovement();
	}
	
}

void UWidgetStudioModernSpinBox::OnTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (!InText.IsNumeric()) { SetValue(Value); }
	if (InCommitMethod == ETextCommit::OnEnter || InCommitMethod == ETextCommit::OnUserMovedFocus)
	{
			SetValue(FCString::Atof(*InText.ToString()));
	}
}

void UWidgetStudioModernSpinBox::UpdateValueOnMouseMovement()
{
	if (!bEnableDragInput) { return; }
	
	// Get the current X position of the mouse on the platform.
	const float CurrentMousePos = UWidgetLayoutLibrary::GetMousePositionOnPlatform().X;

	// Cache the initial mouse position.
	if (LastMousePos == 0.f)
	{
		LastMousePos = CurrentMousePos;
		return;
	}
		
	if (CurrentMousePos > LastMousePos)
	{
		SetValue(Value + abs(LastMousePos - CurrentMousePos));
	}
	else
	{
		SetValue(Value - abs(LastMousePos - CurrentMousePos));
	}

	// Cache the last mouse position.
	LastMousePos = CurrentMousePos;
}

FText UWidgetStudioModernSpinBox::GetLabel() const
{
	return Label;
}

bool UWidgetStudioModernSpinBox::AreArrowButtonsEnabled() const
{
	return bEnableArrowButtons;
}

void UWidgetStudioModernSpinBox::SetArrowButtonsEnabled(const bool bState)
{
	bEnableArrowButtons = bState;
	SynchronizeProperties();
}

bool UWidgetStudioModernSpinBox::IsMouseWheelInputEnabled() const
{
	return bEnableMouseWheelInput;
}

bool UWidgetStudioModernSpinBox::IsDragInputEnabled() const
{
	return bEnableDragInput;
}

void UWidgetStudioModernSpinBox::SetDragInputEnabled(const bool bState)
{
	bEnableDragInput = bState;
}

void UWidgetStudioModernSpinBox::SetMouseWheelInputEnabled(const bool bState)
{
	bEnableMouseWheelInput = bState;
}

EPalette UWidgetStudioModernSpinBox::GetBackgroundColor() const
{
	return BackgroundColor;
}

EPalette UWidgetStudioModernSpinBox::GetContentColor() const
{
	return ContentColor;
}

EPalette UWidgetStudioModernSpinBox::GetIndicatorColor() const
{
	return IndicatorColor;
}

void UWidgetStudioModernSpinBox::SetLabelText(const FText NewText)
{
	Label = NewText;
	SynchronizeProperties();
}

void UWidgetStudioModernSpinBox::SetBackgroundColor(const EPalette NewColor)
{
	BackgroundColor = NewColor;
	SynchronizeProperties();
}

void UWidgetStudioModernSpinBox::SetContentColor(const EPalette NewColor)
{
	ContentColor = NewColor;
	SynchronizeProperties();
}

void UWidgetStudioModernSpinBox::SetIndicatorColor(const EPalette NewColor)
{
	IndicatorColor = NewColor;
	SynchronizeProperties();
}

void UWidgetStudioModernSpinBox::SetValue(const float NewValue, const bool bBroadcast)
{
	Super::SetValue(NewValue, bBroadcast);
	ValueInput->SetText(GetValueAsText());
}

/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#pragma once

#include "CoreMinimal.h"

#include "Widgets/WSButtonBase.h"
#include "Widgets/Modern/WSModernCard.h"
#include "Widgets/Basic/WSIcon.h"
#include "Widgets/Basic/WSText.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/InvalidationBox.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Components/Spacer.h"
#include "Components/VerticalBox.h"
#include "WSModernButton.generated.h"

UENUM(BlueprintType, META=(
	Tooltip = "The location of the icon in conjunction to the widget.",
	DeprecatedNode,
	DeprecationMessage = "Use EWSPlacement instead.")
)
enum class EPlacement : uint8
{
	Left				UMETA(DisplayName="Left"),
	Right				UMETA(DisplayName="Right"),
	Top					UMETA(DisplayName="Top"),
	Bottom				UMETA(DisplayName="Bottom"),

	IconPlacement_Max	UMETA(Hidden),
};

UENUM(BlueprintType, META=(
	Tooltip = "The overall style of the button.",
	ScriptName="ButtonStyling")
)
enum class EButtonStyle : uint8
{
	Solid				UMETA(DisplayName="Solid"),
	Line				UMETA(DisplayName="Outline"),
	Content				UMETA(DisplayName="Content"),

	ButtonStyle_Max		UMETA(Hidden),
};

UENUM(BlueprintType, META=(
	Tooltip = "The style of the buttons content.")
)
enum class EButtonContentStyle : uint8
{
	IconWithLabel		UMETA(DisplayName="Text + Icon"),
	Label				UMETA(DisplayName="Text"),
	Icon				UMETA(DisplayName="Icon"),
	
	ButtonStyle_Max		UMETA(Hidden),
};

/**
 * Contains the data for a Widget Studio Modern Button.
 * This is usually in conjunction with a widget that automatically creates multiple buttons (e.g. WSModernTabBar, WSModernComboBox).
 */
USTRUCT(BlueprintType)
struct FButtonOptions
{
	GENERATED_BODY()

	// TODO: Re-add EditConditionHides meta properties back in 5.0 when the crash has been fixed. https://issues.unrealengine.com/issue/UE-117661
	// TODO: Change FButtonOptions to FWSButtonOptions to avoid naming conflicts with other plugins in the future.
	
	/** What content to display on the button. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio")
	EButtonContentStyle ContentStyle;

	/** The text that will display on the button. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio", Meta = (EditCondition = "ContentStyle != EButtonContentStyle::Icon"))
	FText Text;

	/** The icon to display from the Icon Set. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio", Meta = (EditCondition = "ContentStyle != EButtonContentStyle::Label"))
	EIconItem Icon;

	/**
	 * Use an icon not found in the icon library.
	 * Clear this when you want to use the icons from the icon library.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio", Meta = (EditCondition = "ContentStyle != EButtonContentStyle::Label"))
	UTexture2D* CustomIcon;
	
	/** The text that will displayed a mouse hovers over the button. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio", Meta = (MultiLine = true))
	FText ToolTip;
	
	/**
	* Locks the current checked state.
	* The user will not be able to change the state until this bool is set to false.
	* Requires the bIsCheckable state to be enabled to utilize.
	*/
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	bool bIsCheckedStateLocked;
	
	explicit FButtonOptions(
		const EButtonContentStyle InContentStyle = EButtonContentStyle::Label,
		const bool bInIsCheckedStateLocked = false,
		const FText InText = FText().FromString("New Option"),
		const EIconItem InIcon = EIconItem::Heart,
		UTexture2D* InCustomIcon = nullptr
		)
	{
		bIsCheckedStateLocked = bInIsCheckedStateLocked;
		ContentStyle = InContentStyle;
		Text = InText;
		Icon = InIcon;
		CustomIcon = InCustomIcon;
	}
};

/**
 * A fully styled functional button from the Widget Studio. Can be used for all button needs and purposes with minimal setup.
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernButton : public UWidgetStudioButtonBase
{
	GENERATED_BODY()

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void SynchronizeProperties() override;
	virtual void InitializeStyling() override;
	virtual void UpdateStyling() override;

	// Internal Functions
	void UpdateContentPositionsAndStyling() const;

	// Internal Properties
	const float BaseDropShadowOpacity = .15;

	UPROPERTY()
	float ShadowLength;

	// Widget Components

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UInvalidationBox* Retainer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* BackgroundOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* ContentOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* BackgroundScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* DropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Background = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* HorizontalContent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UVerticalBox* VerticalContent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioIcon* IconItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USpacer* SpacerItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* TextItem = nullptr;

private:

	// General
		
	/** What content to display on the button. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (InLineCategoryProperty))
	EButtonContentStyle ContentStyle = EButtonContentStyle::Label;

	/** The text that will display on the button. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (EditCondition="ContentStyle != EButtonContentStyle::Icon", EditConditionHides))
	FText Text = FText().FromString("Button");

	/** The displayed text options for the button. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (EditCondition="ContentStyle != EButtonContentStyle::Icon", EditConditionHides))
	FWSTextStyle TextStyle = FWSTextStyle(
		EWSFontType::Button,
		FFontStyle(),
		EFontCase::Sentence,
		ETextJustify::Center,
		false,
		0,
		ETextWrappingPolicy::DefaultWrapping);

	/** The icon that will display on the button. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (EditCondition="ContentStyle != EButtonContentStyle::Label", EditConditionHides))
	EIconItem Icon = EIconItem::Heart;
	
	/**
	 * Use an icon not found in the icon library.
	 * Clear this when you want to use the icons from the icon library.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (EditCondition="ContentStyle != EButtonContentStyle::Label", EditConditionHides))
	UTexture2D* CustomIcon;

	/**
	 * Where the icon should be placed on the button in conjunction with the text.
	 * If the text is disabled, this option will be ignored and the icon will be centered.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (EditCondition="ContentStyle == EButtonContentStyle::IconWithLabel", EditConditionHides))
	EWSPlacement IconPlacement = EWSPlacement::Left;

	/** The displayed icon options */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (EditCondition="ContentStyle != EButtonContentStyle::Label", EditConditionHides))
	FWSIconStyle IconStyle;

	/** How the content should be horizontally aligned. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (EditCondition="ContentStyle != EButtonContentStyle::Icon", EditConditionHides))
	TEnumAsByte<EHorizontalAlignment> ContentAlignment = HAlign_Center;
	
	// Colors

	/** The color of the background when the button is NOT in a checked state. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette StandardBackgroundColor = EPalette::TertiaryBackground;

	/** The color of the background when the button IS in a checked or pressed state. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette CheckedBackgroundColor = EPalette::PrimaryAccent;

	/** The color of the content (Text/Icon) when the button is NOT in a checked state. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette StandardContentColor = EPalette::PrimaryContent;

	/** The color of the content (Text/Icon) when the button IS in a checked or pressed state. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette CheckedContentColor = EPalette::TertiaryBackground;

	// Styling

	/** How the button will be displayed. The rounded border radius is controlled in the Project Settings under Widget Studio. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	EButtonStyle ButtonStyle = EButtonStyle::Solid;

	/** The amount of padding between the content and the edges of the button. If both the Text & Button are displayed, the padding on the left and right side will be tripled.  */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	float ContentPadding = 10;
	
	/** How the corners of the button will appear visually. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style", Meta = (EditCondition="ButtonStyle != EButtonStyle::Content", EditConditionHides))
	ECornerStyle CornerStyle = ECornerStyle::Rounded;
	
	/** How the buttons shadow will be displayed. Will be displayed upon mouseover. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style", Meta = (EditCondition="ButtonStyle != EButtonStyle::Content", EditConditionHides))
	EShadowStyle ShadowStyle = EShadowStyle::Small;

public:

	// Helpers

	/** Is the text valid? */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Text")
	bool IsTextValid() const;

	/** Returns the text displayed on the button */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Text")
	FText GetText() const;

	/** Returns the text style options */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Text")
	FWSTextStyle GetTextOptions() const;


	/** Is the icon valid? */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Icon")
	bool IsIconValid() const;

	/** Returns the icon displayed on the button. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Icon")
	EIconItem GetIcon() const;

	/** Returns the icon style options */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Icon")
	FWSIconStyle GetIconOptions() const;

	/** Returns true if the color is being colored by the theme's palette. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Icon")
	bool IsIconColoredByPalette() const;

	/** Returns true if a custom icon has been set and is being used. */
	UFUNCTION(BlueprintGetter, Category="Widget Studio|Helper")
	bool IsUsingCustomIcon() const;

	/** Returns the custom icon. */
	UFUNCTION(BlueprintGetter, Category="Widget Studio|Helper")
	UTexture2D* GetCustomIcon() const;
	

	/** Returns the standard background color of the button. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Color")
	EPalette GetStandardBackgroundColor() const;

	/** Returns the checked background color of the button. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Color")
	EPalette GetCheckedBackgroundColor() const;

	/** Returns the standard content (Icon/Text) color of the button. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Color")
	EPalette GetStandardContentColor() const;

	/** Returns the checked content (Icon/Text) color of the button. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Color")
	EPalette GetCheckedContentColor() const;

	/** Returns the placement of the icon within the button. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Style")
	EWSPlacement GetIconPlacement() const;

	/** Returns the corner style of the button. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Style")
	ECornerStyle GetCornerStyle() const;

	/** Returns the style of the button. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Style")
	EButtonStyle GetButtonStyle() const;

	/** Returns the content style of the button. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio|Helper|Style")
	EButtonContentStyle GetContentStyle() const;


	// Modifiers

	/**
	 * Set the text displayed.
	 * @param NewText The text to display on the button.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Text")
	void SetText(FText NewText);

	/**
	 * Set the text style options.
	 * @param NewTextOptions The text style options.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Text")
	void SetTextOptions(FWSTextStyle NewTextOptions);

	/**
	 * Set the placement of the content on the button.
	 * @param NewAlignment The new alignment to align the buttons contents to.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetAlignment(TEnumAsByte<EHorizontalAlignment> NewAlignment);

	
	/**
	 * Set the icon displayed.
	 * @param NewIcon The icon to display on the button. 
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Icon")
	void SetIcon(EIconItem NewIcon);

	/**
	 * Set the icon style options.
	 * @param NewIconOptions The icon style options.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Icon")
	void SetIconOptions(FWSIconStyle NewIconOptions);

	/**
	 * Set the placement of the icon on the button.
	 * @param NewIconPlacement Where to place the icon within the button.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Icon")
	void SetIconPlacement(EWSPlacement NewIconPlacement);

	/**
	 * Set the icon coloring method. If set to true, the icon will be colored by the palette. If set to false, the icon will use it's default color.
	 * @param NewState The state of using coloring the icon by the set color palette (true) or by using the icons own texture color (false).
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Icon")
	void SetColorIconByPalette(bool NewState);

	/**
	 * Sets the icon to use a custom icon.
	 * @param NewCustomIcon The texture2D to use as an icon.
	 */
	UFUNCTION(BlueprintSetter, Category="Widget Studio|Modifier")
	void SetCustomIcon(UTexture2D* NewCustomIcon);

	/** Clears the set custom icon and automatically switches the widget to use the icon library. */
	UFUNCTION(BlueprintSetter, Category="Widget Studio|Modifier")
	void ClearCustomIcon();
	

	/**
	 * Set the color of the background when the button is NOT in a checked state.
	 * @param NewColor The color to apply to the background of the button.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Color")
	void SetStandardBackgroundColor(EPalette NewColor);

	/**
	 * Set the color of the background when the button IS in a checked state.
	 * @param NewColor The color to apply to the checked background of the button.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Color")
	void SetCheckedBackgroundColor(EPalette NewColor);

	/**
	 * Set the color of the content (Text/Icon) when the button is NOT in a checked state.
	 * @param NewColor The color to apply to the content of the button.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Color")
	void SetStandardContentColor(EPalette NewColor);

	/**
	 * Set the color of the content (Text/Icon) when the button IS in a checked state.
	 * @param NewColor The color to apply to the checked content of the button.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Color")
	void SetCheckedContentColor(EPalette NewColor);


	/**
	 * Set how the corners of the button will appear visually.
	 * @param NewCornerStyle The style of the corners to update to.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Style")
	void SetCornerStyle(ECornerStyle NewCornerStyle);

	/**
	 * Set how the style of the button will appear visually.
	 * @param NewButtonStyle The style of the button to update to.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Style")
	void SetButtonStyle(EButtonStyle NewButtonStyle);

	/**
	 * Set how the content of the button will appear visually.
	 * @param NewContentStyle The style of the content to update to.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier|Style")
	void SetContentStyle(EButtonContentStyle NewContentStyle);

	/**
	 * Set the overall options of the button.
	 * @param NewOptions The options to update to.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetOptions(FButtonOptions NewOptions);
};

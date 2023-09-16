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
#include "WSEnums.generated.h"

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "Choose between different enumeration types."))
enum class EEnumeration : uint8
{
	Float		UMETA(DisplayName="Decimal (Float)"),
	Int			UMETA(DisplayName="Whole Number (Int)"),
	
	Enumeration_Max	UMETA(Hidden),
};

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "The modifier for widget resizing."))
enum class ESizeModifier : uint8
{
	Mini			UMETA(DisplayName="Mini"),
	Tiny			UMETA(DisplayName="Tiny"),
	Small			UMETA(DisplayName="Small"),
	Regular			UMETA(DisplayName="Regular"),
	Large			UMETA(DisplayName="Large"),
	Big				UMETA(DisplayName="Big"),
	Huge			UMETA(DisplayName="Huge"),
	Massive			UMETA(DisplayName="Massive"),

	SizeModifier_Max				UMETA(Hidden)
};

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "Access colors from both a basic color pool and from the Theme.."))
enum class EPalette : uint8
{
	// Accent Colors
	PrimaryAccent			UMETA(DisplayName="Primary Accent"),
	SecondaryAccent			UMETA(DisplayName="Secondary Accent"),

	// Content / Foreground Colors
	PrimaryContent			UMETA(DisplayName="Primary Content"),
	SecondaryContent		UMETA(DisplayName="Secondary Content"),
	TertiaryContent			UMETA(DisplayName="Tertiary Content"),

	// Background Colors
	PrimaryBackground		UMETA(DisplayName="Primary Background"),
	SecondaryBackground		UMETA(DisplayName="Secondary Background"),
	TertiaryBackground		UMETA(DisplayName="Tertiary Background"),

	//Basic Colors
	Red						UMETA(DisplayName="Red"),
	Orange					UMETA(DisplayName="Orange"),
	Yellow					UMETA(DisplayName="Yellow"),
	Olive					UMETA(DisplayName="Olive"),
	Green					UMETA(DisplayName="Green"),
	Teal					UMETA(DisplayName="Teal"),
	Blue					UMETA(DisplayName="Blue"),
	Violet					UMETA(DisplayName="Violet"),
	Purple					UMETA(DisplayName="Purple"),
	Pink					UMETA(DisplayName="Pink"),
	Brown					UMETA(DisplayName="Brown"),
	Grey					UMETA(DisplayName="Grey"),
	Black					UMETA(DisplayName="Black"),
	White					UMETA(DisplayName="White"),
	Transparent				UMETA(DisplayName="Transparent"),

	Palette_Max				UMETA(Hidden)
};

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "The weight of the font stroke."))
enum class EFontWeight : uint8
{
	Light			UMETA(DisplayName = "Light"),
	SemiLight		UMETA(DisplayName = "Semi-Light"),
	Regular			UMETA(DisplayName = "Regular"),
	SemiBold		UMETA(DisplayName = "Semi-Bold"),
	Bold			UMETA(DisplayName = "Bold"),
	Black			UMETA(DisplayName = "Black"),

	FontWeight_Max UMETA(Hidden)
};

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "The style type of the font."))
enum class EWSFontType : uint8
{
	H1				UMETA(DisplayName = "H1"),
	H2				UMETA(DisplayName = "H2"),
	H3				UMETA(DisplayName = "H3"),
	H4				UMETA(DisplayName = "H4"),
	H5				UMETA(DisplayName = "H5"),
	H6				UMETA(DisplayName = "H6"),
	Subtitle1		UMETA(DisplayName = "Subtitle 1"),
	Subtitle2		UMETA(DisplayName = "Subtitle 2"),
	Body1			UMETA(DisplayName = "Body 1"),
	Body2			UMETA(DisplayName = "Body 2"),
	Button			UMETA(DisplayName = "Button"),
	Caption			UMETA(DisplayName = "Caption"),
	Overline		UMETA(DisplayName = "Overline"),
	Custom			UMETA(DisplayName = "Custom"),
	
	FontType_Max	UMETA(Hidden)
};

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "The case of the font."))
enum class EFontCase : uint8
{
	Sentence		UMETA(DisplayName = "Sentence"),
	Uppercase		UMETA(DisplayName = "UPPERCASE"),
	Lowercase		UMETA(DisplayName = "lowercase"),
	
	FontCase_Max	UMETA(Hidden)
};

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "The icons available in the icon set."))
enum class EIconItem : uint8
{
	
	ChevronLeft				UMETA(DisplayName = "Chevron Left"),
	ChevronRight			UMETA(DisplayName = "Chevron Right"),
	ChevronUp				UMETA(DisplayName = "Chevron Up"),
	ChevronDown				UMETA(DisplayName = "Chevron Down"),
	Check					UMETA(DisplayName = "Check Mark"),
	ArrowLeft				UMETA(DisplayName = "Arrow Left"),
	ArrowRight				UMETA(DisplayName = "Arrow Right"),
	ArrowUp					UMETA(DisplayName = "Arrow Up"),
	ArrowDown				UMETA(DisplayName = "Arrow Down"),
	Close					UMETA(DisplayName = "Close"),
	Plus					UMETA(DisplayName = "Plus"),
	Minus					UMETA(DisplayName = "Minus"),
	Hamburger				UMETA(DisplayName = "Hamburger"),
	GridView				UMETA(DisplayName = "GridView"),
	Heart					UMETA(DisplayName = "Heart"),
	Envelope				UMETA(DisplayName = "Envelope"),
	Trash					UMETA(DisplayName = "Trash"),
	Photo					UMETA(DisplayName = "Photo"),
	Document				UMETA(DisplayName = "Document"),
	DocumentCopy			UMETA(DisplayName = "Document Copy"),
	LockClose				UMETA(DisplayName = "Lock Close"),
	LockOpen				UMETA(DisplayName = "Lock Open"),
	ArrowInUp				UMETA(DisplayName = "Arrow In Up"),
	ArrowInDown				UMETA(DisplayName = "Arrow In Down"),
	Download				UMETA(DisplayName = "Download"),
	Upload					UMETA(DisplayName = "Upload"),
	CloudDownload			UMETA(DisplayName = "Cloud Download"),
	CloudUpload				UMETA(DisplayName = "Cloud Upload"),
	Cloud					UMETA(DisplayName = "Cloud"),
	Home					UMETA(DisplayName = "Home"),
	Pen						UMETA(DisplayName = "Pen"),
	Power					UMETA(DisplayName = "Power"),
	Phone					UMETA(DisplayName = "Phone"),
	User					UMETA(DisplayName = "User"),
	Print					UMETA(DisplayName = "Print"),
	Video					UMETA(DisplayName = "Video"),
	Clipboard				UMETA(DisplayName = "Clipboard"),
	Magnifier				UMETA(DisplayName = "Magnifier"),
	MagnifierPlus			UMETA(DisplayName = "Magnifier Plus"),
	MagnifierMinus			UMETA(DisplayName = "Magnifier Minus"),
	Mobile					UMETA(DisplayName = "Mobile"),
	Desktop					UMETA(DisplayName = "Desktop"),
	Attachment				UMETA(DisplayName = "Attachment"),
	Tag						UMETA(DisplayName = "Tag"),
	Globe					UMETA(DisplayName = "Globe"),
	Link					UMETA(DisplayName = "Link"),
	Question				UMETA(DisplayName = "Question"),
	Exclamation				UMETA(DisplayName = "Exclamation"),
	Info					UMETA(DisplayName = "Info"),
	Share					UMETA(DisplayName = "Share"),
	Settings				UMETA(DisplayName = "Settings"),
	CogWheel				UMETA(DisplayName = "Cog Wheel"),
	Volume					UMETA(DisplayName = "Volume"),
	VolumePlus				UMETA(DisplayName = "Volume Plus"),
	VolumeMinus				UMETA(DisplayName = "Volume Minus"),
	VolumeMute				UMETA(DisplayName = "Volume Mute"),
	ControlPlay				UMETA(DisplayName = "Control Play"),
	ControlStop				UMETA(DisplayName = "Control Stop"),
	ControlPause			UMETA(DisplayName = "Control Pause"),
	ControlRewind			UMETA(DisplayName = "Control Rewind"),
	ControlFastForward		UMETA(DisplayName = "Control Fast Forward"),
	ControlStart			UMETA(DisplayName = "Control Start"),
	ControlEnd				UMETA(DisplayName = "Control End"),
	ControlFullscreen		UMETA(DisplayName = "Control Full Screen"),
	SpeechBubble			UMETA(DisplayName = "Speech Bubble"),
	Bookmark				UMETA(DisplayName = "Bookmark"),
	Wifi					UMETA(DisplayName = "Wifi"),
	Microphone				UMETA(DisplayName = "Microphone"),
	Calendar				UMETA(DisplayName = "Calender"),
	Users					UMETA(DisplayName = "Users"),
	Clock					UMETA(DisplayName = "Clock"),
	Refresh					UMETA(DisplayName = "Refresh"),
	Pin						UMETA(DisplayName = "Pin"),
	Map						UMETA(DisplayName = "Map"),
	Navigate				UMETA(DisplayName = "Navigate"),
	Compass					UMETA(DisplayName = "Compass"),
	Expand					UMETA(DisplayName = "Expand"),
	ArrowLeftRight			UMETA(DisplayName = "Arrow Left Right"),
	ShoppingCart			UMETA(DisplayName = "Shopping Cart"),
	Folder					UMETA(DisplayName = "Folder"),
	File					UMETA(DisplayName = "File"),
	FilePlus				UMETA(DisplayName = "File Plus"),
	FileMinus				UMETA(DisplayName = "File Minus"),
	StatisticPie			UMETA(DisplayName = "Statistic Pie"),
	StatisticGraph			UMETA(DisplayName = "Statistic Graph"),
	ListView				UMETA(DisplayName = "List View"),
	Sun						UMETA(DisplayName = "Sun"),
	Moon					UMETA(DisplayName = "Moon"),
};

UENUM(BlueprintType)
enum class EValueClamp : uint8
{
	NotClamped		UMETA(DisplayName="Not Clamped"),
	Clamped			UMETA(DisplayName="Clamped"),				

	ValueClamp_Max	UMETA(Hidden),
};

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "The location to place the widget in relevance of the parent widget."))
enum class EWSPlacement : uint8
{
	Left				UMETA(DisplayName="Left"),
	Right				UMETA(DisplayName="Right"),
	Top					UMETA(DisplayName="Top"),
	Bottom				UMETA(DisplayName="Bottom"),

	Placement_Max		UMETA(Hidden),
};

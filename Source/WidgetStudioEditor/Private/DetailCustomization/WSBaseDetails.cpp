// "// Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved."

#include "DetailCustomization/WSBaseDetails.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "Widgets/WSBase.h"

#define LOCTEXT_NAMESPACE "Base Details"

TSharedRef<IDetailCustomization> FModernButtonDetails::MakeInstance()
{
	return MakeShareable(new FModernButtonDetails);
}

void FModernButtonDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	// Bring the Widget Studio settings to the top of the widgets details panel
	IDetailCategoryBuilder& WSCategory = DetailBuilder.EditCategory("Widget Studio", LOCTEXT("CatName", "Widget Studio"), ECategoryPriority::Important);
	
	// Show/hide Override Border Radius
	bool bShowOverrideBorderRadius;
	const TSharedRef<IPropertyHandle> OverrideBorderRadius = DetailBuilder.GetProperty("bCanOverrideBorderRadius");
	OverrideBorderRadius->GetValue(bShowOverrideBorderRadius);
	
	if (!bShowOverrideBorderRadius)
	{
		DetailBuilder.HideProperty(OverrideBorderRadius);
	}

	// Show/hide Override Dimensions
	bool bShowOverrideDimensions;
	const TSharedRef<IPropertyHandle> OverrideDimensions = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UWidgetStudioBase, bCanOverrideDimensions));
	OverrideBorderRadius->GetValue(bShowOverrideDimensions);
	
	if (!bShowOverrideDimensions)
	{
		DetailBuilder.HideProperty(OverrideDimensions);
	}
}

#undef LOCTEXT_NAMESPACE


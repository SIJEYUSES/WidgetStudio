// "// Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved."

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

/**
 * Button details customization class
 */
class FModernButtonDetails : public IDetailCustomization
{
public:
	// IDetailCustomization Interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	static TSharedRef<IDetailCustomization> MakeInstance();
};
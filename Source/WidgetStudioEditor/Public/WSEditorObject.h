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
#include "UObject/Object.h"
#include "WSEditorObject.generated.h"

/**
 * Editor object that exists for the lifetime of the plugin.
 */
UCLASS()
class WIDGETSTUDIOEDITOR_API UWSEditorObject : public UObject
{
	GENERATED_BODY()

public:
	/**
	* UWSEditorObject main constructor - adds event delegates
	*/
	UWSEditorObject(const FObjectInitializer& ObjectInitializer);

	UWSEditorObject();

	/**
	* Handler for updating WS Settings in Editor module when setting(s) have been changed in Runtime module
	*/
	UFUNCTION()
	void HandleStyleChanged();

	/**
	* Binds Style change event -- called at OnPostEngineInit()
	*/
	UFUNCTION()
	void BindStyleChangedEvent();

private:
	friend class UWidgetStudioEditorTestClass;

	/**
	* Checks to see if one or more instances of "/WidgetStudio" exist in DirectoriesToAlwaysCook.  If so, it removes it / them.
	*/
	void RemoveWidgetStudioFromCookDirs();

};

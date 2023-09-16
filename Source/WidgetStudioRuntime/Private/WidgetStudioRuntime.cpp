/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#include "WidgetStudioRuntime.h"

#define LOCTEXT_NAMESPACE "FWidgetStudioRuntime"
DEFINE_LOG_CATEGORY(LogWidgetStudio);

void FWidgetStudioRuntime::StartupModule()
{
}

void FWidgetStudioRuntime::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWidgetStudioRuntime, WidgetStudioRuntime)

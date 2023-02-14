// Engine/Source/Runtime/WebBrowser/Private/CEF/CEFBrowserApp.cpp

#include "CEF/CEFInterfaceBrowserApp.h"

#if WITH_CEF3

FCEFInterfaceBrowserApp::FCEFInterfaceBrowserApp(bool bInGPU)
	: MessagePumpCountdown(0)
	, bGPU(bInGPU)
{
}

void FCEFInterfaceBrowserApp::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> CommandLine)
{
}

void FCEFInterfaceBrowserApp::OnBeforeCommandLineProcessing(const CefString& ProcessType, CefRefPtr< CefCommandLine > CommandLine)
{
	if (bGPU)
	{
		CommandLine->AppendSwitch("enable-gpu");
		CommandLine->AppendSwitch("enable-gpu-compositing");
	}
	else
	{
		CommandLine->AppendSwitch("disable-gpu");
		CommandLine->AppendSwitch("disable-gpu-compositing");
	}

	CommandLine->AppendSwitch("enable-begin-frame-scheduling");
}

void FCEFInterfaceBrowserApp::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> ExtraInfo)
{
	RenderProcessThreadCreatedDelegate.ExecuteIfBound(ExtraInfo);
}

#if !PLATFORM_LINUX
void FCEFInterfaceBrowserApp::OnScheduleMessagePumpWork(int64 delay_ms)
{
	FScopeLock Lock(&MessagePumpCountdownCS);

	// As per CEF documentation, if delay_ms is <= 0, then the call to CefDoMessageLoopWork should happen reasonably soon.  If delay_ms is > 0, then the call
	//  to CefDoMessageLoopWork should be scheduled to happen after the specified delay and any currently pending scheduled call should be canceled.
	if(delay_ms < 0)
	{
		delay_ms = 0;
	}
	MessagePumpCountdown = delay_ms;
}
#endif

void FCEFInterfaceBrowserApp::TickMessagePump(float DeltaTime, bool bForce)
{
#if PLATFORM_LINUX
	CefDoMessageLoopWork();
	return;
#endif

	bool bPump = false;
	{
		FScopeLock Lock(&MessagePumpCountdownCS);
		
		// count down in order to call message pump
		if (MessagePumpCountdown >= 0)
		{
			MessagePumpCountdown -= DeltaTime * 1000;
			if (MessagePumpCountdown <= 0)
			{
				bPump = true;
			}
			
			if (bPump || bForce)
			{
				// -1 indicates that no countdown is currently happening
				MessagePumpCountdown = -1;
			}
		}
	}
	
	if (bPump || bForce)
	{
		CefDoMessageLoopWork();
	}
}

#endif

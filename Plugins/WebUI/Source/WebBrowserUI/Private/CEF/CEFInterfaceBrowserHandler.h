// Engine/Source/Runtime/WebBrowser/Private/CEF/CEFBrowserHandler.h

#pragma once

#include "CoreMinimal.h"

#if WITH_CEF3

#if PLATFORM_WINDOWS
	#include "Windows/WindowsHWrapper.h"
	#include "Windows/AllowWindowsPlatformTypes.h"
	#include "Windows/AllowWindowsPlatformAtomics.h"
#endif

#pragma push_macro("OVERRIDE")
#undef OVERRIDE // cef headers provide their own OVERRIDE macro
THIRD_PARTY_INCLUDES_START
#if PLATFORM_APPLE
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
#endif
#include "include/cef_client.h"
#if PLATFORM_APPLE
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
#endif
THIRD_PARTY_INCLUDES_END
#pragma pop_macro("OVERRIDE")

#if PLATFORM_WINDOWS
	#include "Windows/HideWindowsPlatformAtomics.h"
	#include "Windows/HideWindowsPlatformTypes.h"
#endif

#include "IWebInterfaceBrowserWindow.h"

#endif

class IWebInterfaceBrowserWindow;
struct Rect;
class FCEFWebInterfaceBrowserWindow;
class FCEFInterfaceBrowserPopupFeatures;

#if WITH_CEF3

/**
 * Implements CEF Client and other Browser level interfaces.
 */
class FCEFInterfaceBrowserHandler
	: public CefClient
	, public CefDisplayHandler
	, public CefLifeSpanHandler
	, public CefLoadHandler
	, public CefRenderHandler
	, public CefRequestHandler
	, public CefKeyboardHandler
	, public CefJSDialogHandler
	, public CefContextMenuHandler
	, public CefDragHandler
{
public:

	/** Default constructor. */
	FCEFInterfaceBrowserHandler(bool InUseTransparency, const TArray<FString>& AltRetryDomains = TArray<FString>());

public:

	/**
	 * Pass in a pointer to our Browser Window so that events can be passed on.
	 *
	 * @param InBrowserWindow The browser window this will be handling.
	 */
	void SetBrowserWindow(TSharedPtr<FCEFWebInterfaceBrowserWindow> InBrowserWindow);
	
	/**
	 * Sets the browser window features and settings for popups which will be passed along when creating the new window.
	 *
	 * @param InPopupFeatures The popup features and settings for the window.
	 */
	void SetPopupFeatures(const TSharedPtr<FCEFInterfaceBrowserPopupFeatures>& InPopupFeatures)
	{
		BrowserPopupFeatures = InPopupFeatures;
	}

public:

	// CefClient Interface

	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override
	{
		return this;
	}

	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override
	{
		return this;
	}

	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override
	{
		return this;
	}

	virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override
	{
		return this;
	}

	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override
	{
		return this;
	}

	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override
	{
		return this;
	}

	virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override
	{
		return this;
	}

	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override
	{
		return this;
	}

	virtual CefRefPtr<CefDragHandler> GetDragHandler() override
	{
		return this;
	}


	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> Browser,
		CefProcessId SourceProcess,
		CefRefPtr<CefProcessMessage> Message) override;


public:

	// CefDisplayHandler Interface

	virtual void OnTitleChange(CefRefPtr<CefBrowser> Browser, const CefString& Title) override;
	virtual void OnAddressChange(CefRefPtr<CefBrowser> Browser, CefRefPtr<CefFrame> Frame, const CefString& Url) override;
	virtual bool OnTooltip(CefRefPtr<CefBrowser> Browser, CefString& Text) override;
	virtual bool OnConsoleMessage(
		CefRefPtr<CefBrowser> Browser, 
		const CefString& Message, 
		const CefString& Source, 
		int Line) override;

public:

	// CefLifeSpanHandler Interface

	virtual void OnAfterCreated(CefRefPtr<CefBrowser> Browser) override;
	virtual bool DoClose(CefRefPtr<CefBrowser> Browser) override;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> Browser) override;

	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> Browser,
		CefRefPtr<CefFrame> Frame,
		const CefString& Target_Url,
		const CefString& Target_Frame_Name,
		CefLifeSpanHandler::WindowOpenDisposition /* Target_Disposition */,
		bool /* User_Gesture */,
		const CefPopupFeatures& PopupFeatures,
		CefWindowInfo& WindowInfo,
		CefRefPtr<CefClient>& Client,
		CefBrowserSettings& Settings,
		bool* no_javascript_access) override 
	{
		return OnBeforePopup(Browser, Frame, Target_Url, Target_Frame_Name, PopupFeatures, WindowInfo, Client, Settings, no_javascript_access);
	}

	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> Browser,
		CefRefPtr<CefFrame> Frame, 
		const CefString& Target_Url, 
		const CefString& Target_Frame_Name,
		const CefPopupFeatures& PopupFeatures, 
		CefWindowInfo& WindowInfo,
		CefRefPtr<CefClient>& Client, 
		CefBrowserSettings& Settings,
		bool* no_javascript_access) ;

public:

	// CefLoadHandler Interface

	virtual void OnLoadError(CefRefPtr<CefBrowser> Browser,
		CefRefPtr<CefFrame> Frame,
		CefLoadHandler::ErrorCode InErrorCode,
		const CefString& ErrorText,
		const CefString& FailedUrl) override;

	virtual void OnLoadingStateChange(
		CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward) override;

#if PLATFORM_LINUX
	virtual void OnLoadStart(
		CefRefPtr<CefBrowser> Browser,
		CefRefPtr<CefFrame> Frame) override;
#else
	virtual void OnLoadStart(
		CefRefPtr<CefBrowser> Browser,
		CefRefPtr<CefFrame> Frame,
		TransitionType CefTransitionType) override;
#endif

public:

	// CefRenderHandler Interface
	virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> Browser, CefRect& Rect) override;
	virtual bool GetViewRect(CefRefPtr<CefBrowser> Browser, CefRect& Rect) override;
	virtual void OnPaint(CefRefPtr<CefBrowser> Browser,
		PaintElementType Type,
		const RectList& DirtyRects,
		const void* Buffer,
		int Width, int Height) override;
	virtual void OnCursorChange(CefRefPtr<CefBrowser> Browser,
		CefCursorHandle Cursor,
		CefRenderHandler::CursorType Type,
		const CefCursorInfo& CustomCursorInfo) override;
	virtual void OnPopupShow(CefRefPtr<CefBrowser> Browser, bool bShow) override;
	virtual void OnPopupSize(CefRefPtr<CefBrowser> Browser, const CefRect& Rect) override;
	virtual bool GetScreenInfo(CefRefPtr<CefBrowser> Browser, CefScreenInfo& ScreenInfo) override;
#if !PLATFORM_LINUX
	virtual void OnImeCompositionRangeChanged(
		CefRefPtr<CefBrowser> Browser,
		const CefRange& SelectionRange,
		const CefRenderHandler::RectList& CharacterBounds) override;
#endif

public:

	// CefRequestHandler Interface

	virtual ReturnValue OnBeforeResourceLoad(
		CefRefPtr<CefBrowser> Browser,
		CefRefPtr<CefFrame> Frame,
		CefRefPtr<CefRequest> Request,
		CefRefPtr<CefRequestCallback> Callback) override;
	virtual void OnResourceLoadComplete(CefRefPtr<CefBrowser> Browser,
		CefRefPtr<CefFrame> Frame,
		CefRefPtr<CefRequest> Request,
		CefRefPtr<CefResponse> Response,
		URLRequestStatus Status,
		int64 Received_content_length) override;
	virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> Browser, TerminationStatus Status) override;
	virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> Browser,
		CefRefPtr<CefFrame> Frame,
		CefRefPtr<CefRequest> Request,
		bool IsRedirect) override;
	virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
		CefRefPtr<CefBrowser> Browser,
		CefRefPtr<CefFrame> Frame,
		CefRefPtr<CefRequest> Request ) override;
	virtual bool OnCertificateError(
		CefRefPtr<CefBrowser> Browser,
		cef_errorcode_t CertError,
		const CefString& RequestUrl,
		CefRefPtr<CefSSLInfo> SslInfo,
		CefRefPtr<CefRequestCallback> Callback ) override;

public:
	// CefKeyboardHandler interface
	virtual bool OnKeyEvent(CefRefPtr<CefBrowser> Browser,
		const CefKeyEvent& Event,
		CefEventHandle OsEvent) override;

public:
	// CefJSDialogHandler interface

#if PLATFORM_LINUX
	virtual bool OnJSDialog(
		CefRefPtr<CefBrowser> Browser,
		const CefString& OriginUrl,
		const CefString& AcceptLang,
		JSDialogType DialogType,
		const CefString& MessageText,
		const CefString& DefaultPromptText,
		CefRefPtr<CefJSDialogCallback> Callback,
		bool& OutSuppressMessage) override;
#else
	virtual bool OnJSDialog(
		CefRefPtr<CefBrowser> Browser,
		const CefString& OriginUrl,
		JSDialogType DialogType,
		const CefString& MessageText,
		const CefString& DefaultPromptText,
		CefRefPtr<CefJSDialogCallback> Callback,
		bool& OutSuppressMessage) override;
#endif

	virtual bool OnBeforeUnloadDialog(CefRefPtr<CefBrowser> Browser, const CefString& MessageText, bool IsReload, CefRefPtr<CefJSDialogCallback> Callback) override;

	virtual void OnResetDialogState(CefRefPtr<CefBrowser> Browser) override;

public:
	// CefContextMenuHandler

	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> Browser,
		CefRefPtr<CefFrame> Frame,
		CefRefPtr<CefContextMenuParams> Params,
		CefRefPtr<CefMenuModel> Model) override;

public:
	// CefDragHandler interface

	virtual void OnDraggableRegionsChanged(
		CefRefPtr<CefBrowser> Browser,
		const std::vector<CefDraggableRegion>& Regions) override;

public:

	IWebInterfaceBrowserWindow::FOnBeforePopupDelegate& OnBeforePopup()
	{
		return BeforePopupDelegate;
	}

	IWebInterfaceBrowserWindow::FOnCreateWindow& OnCreateWindow()
	{
		return CreateWindowDelegate;
	}

	typedef TMap<FString, FString> FRequestHeaders;
	DECLARE_DELEGATE_ThreeParams(FOnBeforeResourceLoadDelegate, const CefString& /*URL*/, CefRequest::ResourceType /*Type*/, FRequestHeaders& /*AdditionalHeaders*/);
	FOnBeforeResourceLoadDelegate& OnBeforeResourceLoad()
	{
		return BeforeResourceLoadDelegate;
	}

	DECLARE_DELEGATE_FourParams(FOnResourceLoadCompleteDelegate, const CefString& /*URL*/, CefRequest::ResourceType /*Type*/, CefRequestHandler::URLRequestStatus /*Status*/, int64 /*ContentLength*/);
	FOnResourceLoadCompleteDelegate& OnResourceLoadComplete()
	{
		return ResourceLoadCompleteDelegate;
	}

	DECLARE_DELEGATE_FourParams(FOnConsoleMessageDelegate, CefRefPtr<CefBrowser> /*Browser*/, const CefString& /*Message*/, const CefString& /*Source*/, int /*Line*/);
	FOnConsoleMessageDelegate& OnConsoleMessage()
	{
		return ConsoleMessageDelegate;
	}

private:

	bool ShowDevTools(const CefRefPtr<CefBrowser>& Browser);

	bool bUseTransparency;

	TArray<FString> AltRetryDomains;
	uint32 AltRetryDomainIdx = 0;

	/** Delegate for notifying that a popup window is attempting to open. */
	IWebInterfaceBrowserWindow::FOnBeforePopupDelegate BeforePopupDelegate;
	
	/** Delegate for handling requests to create new windows. */
	IWebInterfaceBrowserWindow::FOnCreateWindow CreateWindowDelegate;

	/** Delegate for handling adding additional headers to requests */
	FOnBeforeResourceLoadDelegate BeforeResourceLoadDelegate;

	/** Delegate that allows response to the status of resource loads */
	FOnResourceLoadCompleteDelegate ResourceLoadCompleteDelegate;

	/** Delegate that allows for response to console logs.  Typically used to capture and mirror web logs in client application logs. */
	FOnConsoleMessageDelegate ConsoleMessageDelegate;

	/** Weak Pointer to our Web Browser window so that events can be passed on while it's valid.*/
	TWeakPtr<FCEFWebInterfaceBrowserWindow> BrowserWindowPtr;
	
	/** Pointer to the parent web browser handler */
	CefRefPtr<FCEFInterfaceBrowserHandler> ParentHandler;

	/** Stores popup window features and settings */
	TSharedPtr<FCEFInterfaceBrowserPopupFeatures> BrowserPopupFeatures;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(FCEFInterfaceBrowserHandler);
};

#endif

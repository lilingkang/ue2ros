// Copyright 2021 Tracer Interactive, LLC. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Engine/Texture.h"
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SWindow.h"

#if !UE_SERVER
#if PLATFORM_ANDROID || PLATFORM_IOS
#include "SWebBrowser.h"
#include "SWebBrowserView.h"
#include "IWebBrowserPopupFeatures.h"
#include "IWebBrowserWindow.h"
#else
#include "SWebInterfaceBrowserView.h"
#endif

#if PLATFORM_ANDROID || PLATFORM_IOS
typedef SWebBrowser                    SWebInterfaceBrowser;
typedef SWebBrowserView                SWebInterfaceBrowserView;
typedef IWebBrowserAdapter             IWebInterfaceBrowserAdapter;
typedef IWebBrowserDialog              IWebInterfaceBrowserDialog;
typedef IWebBrowserWindow              IWebInterfaceBrowserWindow;
typedef IWebBrowserPopupFeatures       IWebInterfaceBrowserPopupFeatures;
typedef EWebBrowserDialogEventResponse EWebInterfaceBrowserDialogEventResponse;
#else
class SWebInterfaceBrowserView;
class IWebInterfaceBrowserAdapter;
class IWebInterfaceBrowserDialog;
class IWebInterfaceBrowserWindow;
class IWebInterfaceBrowserPopupFeatures;
enum class EWebInterfaceBrowserDialogEventResponse;
#endif
struct FWebNavigationRequest;

class WEBUI_API SWebInterface : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_RetVal_TwoParams( bool, FOnBeforeBrowse, const FString& /*Url*/, const FWebNavigationRequest& /*Request*/ );
	DECLARE_DELEGATE_RetVal_ThreeParams( bool, FOnLoadUrl, const FString& /*Method*/, const FString& /*Url*/, FString& /* Response */ );
	DECLARE_DELEGATE_RetVal_OneParam( EWebInterfaceBrowserDialogEventResponse, FOnShowDialog, const TWeakPtr<IWebInterfaceBrowserDialog>& );

	DECLARE_DELEGATE_RetVal( bool, FOnSuppressContextMenu );

	SLATE_BEGIN_ARGS( SWebInterface )
		: _FrameRate( 60 )
		, _InitialURL( TEXT( "http://tracerinteractive.com" ) )
		, _BackgroundColor( 255, 255, 255, 255 )
		, _EnableMouseTransparency( false )
		, _EnableVirtualPointerTransparency( false )
		, _TransparencyDelay( 0.1f )
		, _TransparencyThreshold( 0.333f )
		, _TransparencyTick( 0.0f )
		, _ViewportSize( FVector2D::ZeroVector )
	{
		_Visibility = EVisibility::SelfHitTestInvisible;
	}
		SLATE_ARGUMENT( TSharedPtr<SWindow>, ParentWindow )
		SLATE_ARGUMENT( int32, FrameRate )
		SLATE_ARGUMENT( FString, InitialURL )
		SLATE_ARGUMENT( TOptional<FString>, ContentsToLoad )
		SLATE_ARGUMENT( FColor, BackgroundColor )
		SLATE_ARGUMENT( bool, NativeCursors )
		SLATE_ARGUMENT( bool, EnableMouseTransparency )
		SLATE_ARGUMENT( bool, EnableVirtualPointerTransparency )
		SLATE_ARGUMENT( float, TransparencyDelay )
		SLATE_ARGUMENT( float, TransparencyThreshold )
		SLATE_ARGUMENT( float, TransparencyTick )
		SLATE_ARGUMENT( TOptional<EPopupMethod>, PopupMenuMethod )

		SLATE_ATTRIBUTE( FVector2D, ViewportSize );

		SLATE_EVENT( FSimpleDelegate, OnLoadCompleted )
		SLATE_EVENT( FSimpleDelegate, OnLoadError )
		SLATE_EVENT( FSimpleDelegate, OnLoadStarted )
		SLATE_EVENT( FOnTextChanged, OnTitleChanged )
		SLATE_EVENT( FOnTextChanged, OnUrlChanged )
		SLATE_EVENT( FOnBeforePopupDelegate, OnBeforePopup )
		SLATE_EVENT( FOnCreateWindowDelegate, OnCreateWindow )
		SLATE_EVENT( FOnCloseWindowDelegate, OnCloseWindow )
		SLATE_EVENT( FOnBeforeBrowse, OnBeforeNavigation )
		SLATE_EVENT( FOnLoadUrl, OnLoadUrl )
		SLATE_EVENT( FOnShowDialog, OnShowDialog )
		SLATE_EVENT( FSimpleDelegate, OnDismissAllDialogs )
		SLATE_EVENT( FOnSuppressContextMenu, OnSuppressContextMenu );
	SLATE_END_ARGS()

	SWebInterface();
	~SWebInterface();

	void Construct( const FArguments& InArgs );

	virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;
	virtual bool SupportsKeyboardFocus() const override { return true; }

private:

	FLinearColor LastMousePixel;
	float        LastMouseTime;
	float        LastMouseTick;

	EVisibility GetViewportVisibility() const;

	bool HandleBeforePopup( FString URL, FString Frame );
	bool HandleSuppressContextMenu();

	bool HandleCreateWindow( const TWeakPtr<IWebInterfaceBrowserWindow>& NewBrowserWindow, const TWeakPtr<IWebInterfaceBrowserPopupFeatures>& PopupFeatures );
	bool HandleCloseWindow( const TWeakPtr<IWebInterfaceBrowserWindow>& BrowserWindowPtr );

protected:

	static bool bPAK;

	TSharedPtr<SWebInterfaceBrowserView>   BrowserView;
	TSharedPtr<IWebInterfaceBrowserWindow> BrowserWindow;

#if UE_BUILD_DEVELOPMENT || UE_BUILD_DEBUG
	TMap<TWeakPtr<IWebInterfaceBrowserWindow>, TWeakPtr<SWindow>> BrowserWindowWidgets;
#endif

	bool bMouseTransparency;
	bool bVirtualPointerTransparency;

	float TransparencyDelay;
	float TransparencyThreshold;
	float TransparencyTick;


	FSimpleDelegate OnLoadCompleted;
	FSimpleDelegate OnLoadError;
	FSimpleDelegate OnLoadStarted;

	FOnTextChanged OnTitleChanged;
	FOnTextChanged OnUrlChanged;

	FOnBeforePopupDelegate  OnBeforePopup;
	FOnCreateWindowDelegate OnCreateWindow;
	FOnCloseWindowDelegate  OnCloseWindow;

	FOnBeforeBrowse OnBeforeNavigation;
	FOnLoadUrl      OnLoadUrl;

	FOnShowDialog   OnShowDialog;
	FSimpleDelegate OnDismissAllDialogs;

public:

	bool HasMouseTransparency() const;
	bool HasVirtualPointerTransparency() const;

	float GetTransparencyDelay() const;
	float GetTransparencyThreshold() const;
	float GetTransparencyTick() const;

	int32 GetTextureWidth() const;
	int32 GetTextureHeight() const;

	FColor ReadTexturePixel( int32 X, int32 Y ) const;
	TArray<FColor> ReadTexturePixels( int32 X, int32 Y, int32 Width, int32 Height ) const;
	
	void LoadURL( FString NewURL );
	void LoadString( FString Contents, FString DummyURL );

	void Reload();
	void StopLoad();

	FString GetUrl() const;

	bool IsLoaded() const;
	bool IsLoading() const;

	void ExecuteJavascript( const FString& ScriptText );

	void BindUObject( const FString& Name, UObject* Object, bool bIsPermanent = true );
	void UnbindUObject( const FString& Name, UObject* Object, bool bIsPermanent = true );

	void BindAdapter( const TSharedRef<IWebInterfaceBrowserAdapter>& Adapter );
	void UnbindAdapter( const TSharedRef<IWebInterfaceBrowserAdapter>& Adapter );

	void BindInputMethodSystem( ITextInputMethodSystem* TextInputMethodSystem );
	void UnbindInputMethodSystem();
};
#endif

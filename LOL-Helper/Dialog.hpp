#pragma once

#include "Window.hpp"
#include "functional"
#include "Observer.hpp"

struct DialogSystemMessage
{
	HWND h_dlg;
	UINT message;
	WPARAM w_param;
	LPARAM l_param;
};

struct DialogCommonEvent
{
	HWND h_dlg;
	WORD notification_code;
	WORD control_id;
	LPARAM l_param;
};


struct DialogEvent
{
	enum
	{
		OnDialogMessage,
		OndialogCommonEvent
	};

	using ObserverTable = std::tuple<
		Observer<INT_PTR(const DialogSystemMessage&)>,
		Observer<INT_PTR(const DialogCommonEvent&)>
	>;
};


/**
 * \brief 
 * \param f 
 */
#define BIND_TO_DIALOG_EVENT(f) std::bind(&f, this, std::placeholders::_1)

/// <summary>
/// Generic dialog class
/// </summary>
class Dialog : public Window , public Observable<DialogEvent>
{
public:
    typedef INT_PTR( Dialog::*fnDlgProc )(const DialogSystemMessage&);

    typedef std::map<UINT, std::function<INT_PTR(const DialogSystemMessage&)>> mapMsgProc;
    typedef std::map<WORD, fnDlgProc> mapCtrlProc;

public:
    Dialog( int dialogID ,HWND parent = NULL)
        : _dialogID( dialogID ),
		dlgProc(&Dialog::DlgProc, this)
    { 
		_hwnd = CreateDialogW(GetModuleHandleA(0), MAKEINTRESOURCE(_dialogID), parent, dlgProc.GetThunk());
		Register<DialogEvent::OnDialogMessage>(BIND_TO_DIALOG_EVENT(Dialog::OnInit));
		Register<DialogEvent::OnDialogMessage>(BIND_TO_DIALOG_EVENT(Dialog::OnCommand));
		Register<DialogEvent::OnDialogMessage>(BIND_TO_DIALOG_EVENT(Dialog::OnClose));
    }

    /// <summary>
    /// Run dialog as modal window
    /// </summary>
    /// <param name="parent">Parent window.</param>
    /// <returns>TRUE on success, FALSE otherwise</returns>
    virtual INT_PTR RunModal( HWND parent = NULL )
    {
        // Stupid std::function and std::bind don't support __stdcall
        // Had to do this idiotic workaround
        Win32Thunk<DLGPROC, Dialog> dlgProc( &Dialog::DlgProc, this );
		int a = DialogBoxW(NULL, MAKEINTRESOURCEW(_dialogID), parent, dlgProc.GetThunk());
        return a;
    }

    /// <summary>
    /// Run dialog as modeless window
    /// </summary>
    /// <param name="parent">Parent window</param>
    /// <param name="accelID">Accelerator ID to use</param>
    /// <returns>TRUE on success, FALSE otherwise</returns>
    virtual INT_PTR RunModeless( HWND parent = NULL, int accelID = 0 )
    {
        MSG msg = { 0 };
        BOOL bRet = FALSE;
        HACCEL hAccel = LoadAcceleratorsW( NULL, MAKEINTRESOURCEW( accelID ) );
        _modeless = true;
        _hwnd = CreateDialogW( NULL, MAKEINTRESOURCE( _dialogID ), parent, dlgProc.GetThunk() );
        ShowWindow( _hwnd, SW_SHOW );

        while (IsWindow( _hwnd ) && PeekMessage( &msg, NULL, 0, 0,PM_REMOVE ) > 0)
        {
            if (TranslateAccelerator( _hwnd, hAccel, &msg ))
                continue;

            if (!IsDialogMessage( _hwnd, &msg ))
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            } 
        }

        return TRUE;
    }

	virtual INT_PTR CreateAndShow(HWND parent = NULL, int accelID = 0)
	{

		ShowWindow(_hwnd, SW_SHOW);
		UpdateWindow(_hwnd);
		return TRUE;
	}

protected:

    /// <summary>
    /// Close dialog properly
    /// </summary>
    /// <returns>status</returns>
    INT_PTR CloseDialog()
    {
        if (_modeless)
        {
            DestroyWindow( _hwnd );
            _hwnd = NULL;
        }
        else
            EndDialog( _hwnd, 0 );

        return TRUE;
    }

    /// <summary>
    /// Dialog message proc
    /// </summary>
    INT_PTR DlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
    {
		Notify<DialogEvent::OnDialogMessage>(DialogSystemMessage{ hDlg ,message ,wParam ,lParam });
		return 0;
    }

    /// <summary>
    /// WM_INITDIALOG handler
    /// </summary>
    virtual INT_PTR OnInit(const DialogSystemMessage& message )
    {
		if (message.message == WM_INITDIALOG) {
			_hwnd = message.h_dlg;
			return true;
		}
        return false;
    }

    /// <summary>
    /// WM_COMMAND handler
    /// </summary>
    virtual INT_PTR OnCommand(const DialogSystemMessage& dialog_system_message )
    {
		if (dialog_system_message.message == WM_COMMAND)
			Notify<DialogEvent::OndialogCommonEvent>(DialogCommonEvent{ dialog_system_message.h_dlg,
			HIWORD(dialog_system_message.w_param),
			LOWORD(dialog_system_message.w_param),
			dialog_system_message.l_param
				});
		else
			return false;
		return true;
    }

    /// <summary>
    /// WM_CLOSE handler
    /// </summary>
    virtual INT_PTR OnClose(const DialogSystemMessage& message)
    {
		if (message.message == WM_CLOSE)
			return CloseDialog();
		return false;
    }

protected:
    int _dialogID;              // Dialog resource ID
    bool _modeless = false;     // Modeless dialog
	Win32Thunk<DLGPROC, Dialog> dlgProc;
};


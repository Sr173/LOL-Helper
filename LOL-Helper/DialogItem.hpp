#pragma once

#include "Window.hpp"

namespace DialogItem {

	class DialogItemBase : public Window
	{
		friend class Dialog;
	public:
		DialogItemBase(HWND hdlg, int item_id)
			:item_id(item_id), h_dlg(hdlg), Window(GetDlgItem(hdlg, item_id))
		{

		}
	protected:
		HWND h_dlg;
		int item_id;
	};
}
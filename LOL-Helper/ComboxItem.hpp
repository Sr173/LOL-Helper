#include "DialogItem.hpp"
#include "windowsx.h"

namespace DialogItem
{
	class ComboBoxItem : public DialogItem::DialogItemBase
	{
	public:
		ComboBoxItem(HWND hdlg, int item_id)
			:DialogItemBase(hdlg, item_id)
		{

		}

		void add_string_item(std::wstring text)
		{
			ComboBox_AddString(hwnd(), text.data());
		}

		int get_select_index()
		{
			return (int)(DWORD)SNDMSG(hwnd(), CB_GETCURSEL, 0L, 0L);
		}

		void set_select_index(int index)
		{
			ComboBox_SetCurSel(hwnd(), index);
		}
	};
}

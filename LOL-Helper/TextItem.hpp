#pragma once

#include "DialogItem.hpp"

namespace DialogItem
{
	class TextItem : public DialogItem::DialogItemBase
	{
	public:
		TextItem (HWND h_dlg,int item_id)
			:DialogItemBase(h_dlg, item_id)
		{
			
		}
	};
}
#pragma once
#include "DialogItem.hpp"
#include "Observer.hpp"

namespace  DialogItem {
	struct ButtonEvent
	{
		enum { OnClicked };
		using ObserverTable = std::tuple<
			Observer<void()>
		>;
	};

	class ButtonItem : public DialogItem::DialogItemBase,public Observable<ButtonEvent>
	{
		friend class Dialog;
	public:
		ButtonItem(HWND hdlg, int item_id)
			:DialogItemBase(hdlg, item_id)
		{

		}
		void notify_clicked()
		{
			Notify<ButtonEvent::OnClicked>();
		}
	};
}
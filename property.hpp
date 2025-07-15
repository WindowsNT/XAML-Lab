#pragma once

class SERIALIZABLE
{
public:

	virtual void Ser(XML3::XMLElement& el) = 0;
	virtual void Unser(XML3::XMLElement& el) = 0;
};

class PROPERTY : public SERIALIZABLE
{

public:
	std::wstring g;
	std::wstring n;
	std::wstring bindv;
	int special_type = 0; // 1,2 Grid Row/Column, 3,4 Scale/Translate, 5 menubar menu, 6 dropdown menu, 7 NV page
	std::wstring xmln;
	std::wstring tip;
	bool S = 0;
	virtual void Ser(XML3::XMLElement& el) override
	{
		el.vv("g").SetValue(g);
		el.vv("n").SetValue(n);
		el.vv("x").SetValue(xmln);
		el.vv("s").SetValueInt(special_type);
		el.vv("tip").SetValue(tip);
		el.vv("bindv").SetValue(bindv);
	}

	virtual void Unser(XML3::XMLElement& el) override
	{
		g = el.vv("g").GetWideValue();
		n = el.vv("n").GetWideValue();
		xmln = el.vv("x").GetWideValue();
		special_type = el.vv("s").GetValueInt(0);
		tip = el.vv("tip").GetWideValue();
		bindv = el.vv("bindv").GetWideValue();
	}

};


enum PROPERTY_TYPE
{
	PT_STRING,
	PT_INT,
	PT_DOUBLE,
	PT_LIST,
	PT_HEADER,
	PT_COLOR,
	PT_GROUP,
	PT_BOOL,
	PT_FUNCTION,
};

struct FUNCTION_SIG : public SERIALIZABLE
{
	std::wstring name;
	std::vector<std::wstring> types = { L"IInspectable",L"IInspectable" };
	std::wstring return_type = L"void";
	std::wstring content;

	void Ser(XML3::XMLElement& el)
	{
		el.vv("function_name").SetValue(name);
		el.vv("function_return_type").SetValue(return_type);
		el.vv("function_content").SetValue(content);
		for (const auto& type : types)
		{
			el.AddElement("function_type").vv("value").SetValue(type);
		}
	}
	void Unser(XML3::XMLElement&  el)
	{
		name = el.vv("function_name").GetWideValue();
		return_type = el.vv("function_return_type").GetWideValue();
		if (return_type == L"")
			return_type = L"void"; // Default return type
		content = el.vv("function_content").GetWideValue();
		types.clear();
		for (const auto& typeEl : el.GetChildren())
		{
			if (typeEl->GetElementName() == "function_type")
			{
				types.push_back(typeEl->vv("value").GetWideValue());
			}
		}
	}

};

class FUNCTION_PROPERTY : public PROPERTY
{
public:
	FUNCTION_SIG f;
	std::wstring value;
	virtual void Ser(XML3::XMLElement& el) override
	{
		PROPERTY::Ser(el);
		f.Ser(el);
		el.vv("v").SetValue(value);
	}
	virtual void Unser(XML3::XMLElement& el) override
	{
		PROPERTY::Unser(el);
		f.Unser(el);
		value = el.vv("v").GetWideValue();
	}
};

class STRING_PROPERTY : public PROPERTY
{
public:
	std::wstring value;
	std::wstring def = L"";
	bool BindingAnyway = 0;
	virtual void Ser(XML3::XMLElement& el) override
	{
		PROPERTY::Ser(el);
		el.vv("v").SetValue(value);
		el.vv("def").SetValue(def);
		el.vv("ba").SetValueInt(BindingAnyway);
	}
	virtual void Unser(XML3::XMLElement& el) override
	{
		PROPERTY::Unser(el);
		value = el.vv("v").GetWideValue();
		def = el.vv("def").GetWideValue();
		BindingAnyway = el.vv("ba").GetValueInt(0) != 0;
	}
};

class LIST_PROPERTY : public PROPERTY
{
public:

	std::vector<std::wstring> Items;
	size_t SelectedIndex = 0;
	size_t DefaultIndex = 0;

	virtual void Ser(XML3::XMLElement& el) override
	{
		PROPERTY::Ser(el);
		// Serialize list items here if needed
		el.vv("s").SetValueULongLong(SelectedIndex);
		el.vv("d").SetValueULongLong(DefaultIndex);
	}


	virtual void Unser(XML3::XMLElement& el) override
	{
		PROPERTY::Unser(el);
		// Deserialize list items here if needed
		SelectedIndex = el.vv("s").GetValueULongLong(0);
		DefaultIndex = el.vv("d").GetValueULongLong(0);
	}
};

class BOOL_PROPERTY : public PROPERTY
{
public:

	size_t SelectedIndex = 0;
	size_t DefaultIndex = 0;

	virtual void Ser(XML3::XMLElement& el) override
	{
		PROPERTY::Ser(el);
		el.vv("s").SetValueULongLong(SelectedIndex);
		el.vv("d").SetValueULongLong(DefaultIndex);
	}


	virtual void Unser(XML3::XMLElement& el) override
	{
		PROPERTY::Unser(el);
		SelectedIndex = el.vv("s").GetValueULongLong(0);
		DefaultIndex = el.vv("d").GetValueULongLong(0);
	}
};


class COLOR_PROPERTY : public PROPERTY
{
	public:
	winrt::Windows::UI::Color value = winrt::Windows::UI::Colors::Black();
	winrt::Windows::UI::Color def = winrt::Windows::UI::Colors::Black();
	virtual void Ser(XML3::XMLElement& el) override
	{
		PROPERTY::Ser(el);
		el.vv("r").SetValueInt(value.R);
		el.vv("g").SetValueInt(value.G);
		el.vv("b").SetValueInt(value.B);
		el.vv("a").SetValueInt(value.A);
		el.vv("dr").SetValueInt(def.R);
		el.vv("dg").SetValueInt(def.G);
		el.vv("db").SetValueInt(def.B);
		el.vv("da").SetValueInt(def.A);
	}
	virtual void Unser(XML3::XMLElement& el) override
	{
		PROPERTY::Unser(el);
		value.R = (uint8_t)el.vv("r").GetValueInt(0);
		value.G = (uint8_t)el.vv("g").GetValueInt(0);
		value.B = (uint8_t)el.vv("b").GetValueInt(0);
		value.A = (uint8_t)el.vv("a").GetValueInt(0);
		def.R = (uint8_t)el.vv("dr").GetValueInt(0);
		def.G = (uint8_t)el.vv("dg").GetValueInt(0);
		def.B = (uint8_t)el.vv("db").GetValueInt(0);
		def.A = (uint8_t)el.vv("da").GetValueInt(0);
	}
};


class DOUBLE_PROPERTY : public PROPERTY
{
	public:
	double def = 0.0;
	double value = 0.0;
	double mmin = 0.0;
	double mmax = 1.0;
	double smallchange = 0.1;
	double largechange = 1.0;

	virtual void Ser(XML3::XMLElement& el) override
	{
		PROPERTY::Ser(el);
		el.vv("v").SetValueDouble(value);
		el.vv("def").SetValueDouble(def);
		el.vv("min").SetValueDouble(mmin);
		el.vv("max").SetValueDouble(mmax);
		el.vv("sc").SetValueDouble(smallchange);
		el.vv("lc").SetValueDouble(largechange);
	}
	virtual void Unser(XML3::XMLElement& el) override
	{
		PROPERTY::Unser(el);
		value = el.vv("v").GetValueDouble();
		def = el.vv("def").GetValueDouble(0.0);
		mmin = el.vv("min").GetValueDouble(0);
		mmax = el.vv("max").GetValueDouble(1);
		smallchange = el.vv("sc").GetValueDouble(0.1);
		largechange = el.vv("lc").GetValueDouble(1.0);
	}
};



class GROUP_PROPERTY : public PROPERTY
{
public:

	std::wstring GroupName;
	std::vector<std::shared_ptr<PROPERTY>> Items;

	virtual void Ser(XML3::XMLElement& el) override
	{
		PROPERTY::Ser(el);
		el.vv("group").SetValue(GroupName);
		// Serialize list items here if needed
		for (auto& item : Items)
		{
			XML3::XMLElement& itemEl = el.AddElement(XML3::XMLU(item->n.c_str()).bc());
			item->Ser(itemEl);
		}
	}


	virtual void Unser(XML3::XMLElement& el) override
	{
		PROPERTY::Unser(el);
		GroupName = el.vv("group").GetWideValue();
		// Deserialize list items here if needed
		for (auto& itemEl : el)
		{
			std::shared_ptr<PROPERTY> item;
			if (itemEl.GetElementName() == "STRING_PROPERTY")
				item = std::make_shared<STRING_PROPERTY>();
			if (itemEl.GetElementName() == "FUNCTION_PROPERTY")
				item = std::make_shared<FUNCTION_PROPERTY>();
			else if (itemEl.GetElementName() == "LIST_PROPERTY")
				item = std::make_shared<LIST_PROPERTY>();
			else if (itemEl.GetElementName() == "COLOR_PROPERTY")
				item = std::make_shared<COLOR_PROPERTY>();
			else if (itemEl.GetElementName() == "DOUBLE_PROPERTY")
				item = std::make_shared<DOUBLE_PROPERTY>();
			else
				continue; // Unknown type
			item->Unser(itemEl);
			Items.push_back(item);
		}
	}
};

//void ApplyPropertiesFor(winrt::Microsoft::UI::Xaml::UIElement e, std::vector <std::shared_ptr<PROPERTY>> props);
//void ApplyPropertiesFor(winrt::Microsoft::UI::Xaml::FrameworkElement e, std::vector <std::shared_ptr<PROPERTY>> props);
//void ApplyPropertiesFor(winrt::Microsoft::UI::Xaml::Controls::Control e, std::vector <std::shared_ptr<PROPERTY>> props);
//void ApplyPropertiesFor(winrt::Microsoft::UI::Xaml::Controls::Panel e, std::vector <std::shared_ptr<PROPERTY>> props);

using namespace winrt::Microsoft::UI::Xaml::Media;
using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media;
using namespace Windows::UI;

#include "templateproperties.h"

std::vector<std::shared_ptr<PROPERTY>> CreatePropertiesFor(winrt::Microsoft::UI::Xaml::FrameworkElement e);
std::vector<std::shared_ptr<PROPERTY>> CreatePropertiesFor(winrt::Microsoft::UI::Xaml::Controls::Panel e);

class XITEM;
void XMLPropertiesFor(XML3::XMLElement& ee, XITEM* xit, std::vector <std::shared_ptr<PROPERTY>> props);
void LoadXMLPropertiesfor(XML3::XMLElement& ee, std::vector <std::shared_ptr<PROPERTY>> props);


class XITEM : public SERIALIZABLE
{
public:

	std::vector<FUNCTION_SIG> CallbackFunctions;
	winrt::Windows::Foundation::IInspectable X;
	bool Selected = false;

	std::wstring ElementName; // Say, "StackPanel"
	std::vector<std::shared_ptr<PROPERTY>> properties;
	std::vector<std::shared_ptr<XITEM>> children;
	XITEM* the_par = 0;
	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int,XITEM* par)
	{
		par;
		return nullptr;
	}
	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el)
	{
		el;
		return {};
	}
	virtual void ApplyProperties()
	{
		CallbackFunctions.clear();
	}

	virtual std::optional<std::wstring> GetCodeForProperty([[maybe_unused]] PROPERTY* p,[[maybe_unused]] int Type /* 0 idl 1 h 2 cpp */)
	{
		return {};
	}
	template <typename T>
	void AllTap(T X)
	{
		auto b = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Control>();
		if (b)
		{
			b.Tapped([](winrt::Windows::Foundation::IInspectable t, winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs  teh)
				{
					GenericTap(t);
					teh.Handled(true);
				});
			b.RightTapped([](winrt::Windows::Foundation::IInspectable t, winrt::Microsoft::UI::Xaml::Input::RightTappedRoutedEventArgs  teh)
				{
					GenericTap(t);
					teh.Handled(true);
				});
		}
	}

/*
	template <typename T>
	size_t AddPropertySet()
	{
		auto uip = CreatePropertiesFor(X.as<T>());
		std::sort(uip.begin(), uip.end(), [](std::shared_ptr<PROPERTY> left, std::shared_ptr<PROPERTY> right) -> bool
			{
				if (left->n < right->n)
					return true;
				return false;
			});

		for (auto& p : uip)
		{
			properties.push_back(p);
		}

		return uip.size();
	}
	*/

	void SaveRecursive(XML3::XMLElement* ee)
	{
		ee->SetElementName(XML3::XMLU(ElementName.c_str()).bc());
		XMLPropertiesFor(*ee, this, properties);


		bool ISXItemNavigationView(XITEM * xit);
		if (ISXItemNavigationView(this))
			return;

		for (auto& ch : children)
		{
			XML3::XMLElement& eee = ee->AddElement(XML3::XMLU(ch->ElementName.c_str()).bc());
			ch->SaveRecursive(&eee);
		}
	}

	virtual XML3::XMLElement SaveEl()
	{
		XML3::XMLElement ee;
		SaveRecursive(&ee);
		return ee;
	}

	virtual void Ser(XML3::XMLElement& el) override
	{
		el.SetElementName(ElementName.c_str());
		XMLPropertiesFor(el, this,properties);
		for (auto& ch : children)
		{
			XML3::XMLElement& eee = el.AddElement(XML3::XMLU(ch->ElementName.c_str()).bc());
			ch->Ser(eee);
		}
	}
	virtual void Unser(XML3::XMLElement& el) override;

	virtual 	void Select()
	{
	}

	virtual 	void Unselect()
	{
	}

};

void GenericTap(winrt::Windows::Foundation::IInspectable);






bool ISXItemStackPanel(XITEM* xit);
bool ISXItemSwapChainPanel(XITEM* xit);
bool ISXItemRelativePanel(XITEM* xit);
bool ISXItemCanvas(XITEM* xit);
bool ISXItemGrid(XITEM* xit);
bool ISXItemNavigationView(XITEM* xit);
std::shared_ptr<XITEM> CreateXItemStackPanel();
std::shared_ptr<XITEM> CreateXItemSwapChainPanel();
std::shared_ptr<XITEM> CreateXItemRelativePanel();
std::shared_ptr<XITEM> CreateXItemCanvas();
std::shared_ptr<XITEM> CreateXItemGrid();
std::shared_ptr<XITEM> CreateXItemButton();
std::shared_ptr<XITEM> CreateXItemExpander();
std::shared_ptr<XITEM> CreateXItemHLButton();
std::shared_ptr<XITEM> CreateXItemDropDownButton();
std::shared_ptr<XITEM> CreateXItemAppBarButton();
std::shared_ptr<XITEM> CreateXItemToggleSwitch();
std::shared_ptr<XITEM> CreateXItemRepeatButton();
std::shared_ptr<XITEM> CreateXItemCheckBox();
std::shared_ptr<XITEM> CreateXItemAnimatedVisualPlayer();
std::shared_ptr<XITEM> CreateXItemRadioButton();
std::shared_ptr<XITEM> CreateXItemColorPicker();
std::shared_ptr<XITEM> CreateXItemMapControl();
std::shared_ptr<XITEM> CreateXItemTextBlock();
std::shared_ptr<XITEM> CreateXItemTextBox();
std::shared_ptr<XITEM> CreateXItemRichEditBox();
std::shared_ptr<XITEM> CreateXItemRichTextBlock();
std::shared_ptr<XITEM> CreateXItemNumberBox();
std::shared_ptr<XITEM> CreateXItemPasswordBox();
std::shared_ptr<XITEM> CreateXItemBreadcrumbBar();
std::shared_ptr<XITEM> CreateXItemNavigationView();
std::shared_ptr<XITEM> CreateXItemNavigationViewItem();
std::shared_ptr<XITEM> CreateXItemPivot();
std::shared_ptr<XITEM> CreateXItemMenuBar();
std::shared_ptr<XITEM> CreateXItemCommandBar();
std::shared_ptr<XITEM> CreateXItemRatingControl();
std::shared_ptr<XITEM> CreateXItemSlider();
std::shared_ptr<XITEM> CreateXItemWebView2();
std::shared_ptr<XITEM> CreateXItemImage();
std::shared_ptr<XITEM> CreateXItemPersonPicture();
std::shared_ptr<XITEM> CreateXItemMediaPlayerElement();
std::shared_ptr<XITEM> CreateXItemInfoBar();
std::shared_ptr<XITEM> CreateXItemInfoBadge();
std::shared_ptr<XITEM> CreateXItemProgressBar();
std::shared_ptr<XITEM> CreateXItemProgressRing();
std::shared_ptr<XITEM> CreateXItemFlipView();
std::shared_ptr<XITEM> CreateXItemGridView();
std::shared_ptr<XITEM> CreateXItemListView();
std::shared_ptr<XITEM> CreateXItemListBox();
std::shared_ptr<XITEM> CreateXItemComboBox();
std::shared_ptr<XITEM> CreateXItemTreeView();
std::shared_ptr<XITEM> CreateXItemCalendarDatePicker();
std::shared_ptr<XITEM> CreateXItemDatePicker();
std::shared_ptr<XITEM> CreateXItemTimePicker();
std::shared_ptr<XITEM> CreateXItemCalendarView();


class XITEM_UIElement: public XITEM
{
	public:
		void ApplyProperties();
		std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el);
};

class XITEM_FrameworkElement : public XITEM_UIElement
{
	public:
		void ApplyProperties();
		std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el);
};

class XITEM_Control : public XITEM_FrameworkElement
{
	public:
		void ApplyProperties();
		std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el);
		void Select();
		void Unselect();
};

class XITEM_Panel : public XITEM_FrameworkElement
{
public:
	void ApplyProperties();
	std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el);
	void Select();
	void Unselect();
};

class XITEM_ContentControl : public XITEM_Control
{
public:
	void ApplyProperties();
	std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el);
};

class XITEM_ItemsControl : public XITEM_Control
{
public:
	void ApplyProperties();
	std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el);
};

class XITEM_Selector : public XITEM_ItemsControl
{
public:
	void ApplyProperties();
	std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el);
};

class XITEM_ButtonBase : public XITEM_ContentControl
{
public:
	void ApplyProperties();
	std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el);
};


class XITEM_ToggleButton : public XITEM_ButtonBase
{
public:
	void ApplyProperties();
	std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el);
};

class XITEM_ListViewBase : public XITEM_Selector
{
public:
	void ApplyProperties();
	std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el);
};

inline bool IsSomeParentGrid(XITEM* xit,bool = false)
{
	if (!xit)
		return false;
	if (ISXItemGrid(xit))
		return true;
	if (xit->the_par)
		return IsSomeParentGrid(xit->the_par,1);
	return false;
}

inline bool IsSomeParentCanvas(XITEM* xit, bool = false)
{
	if (!xit)
		return false;
	if (ISXItemCanvas(xit))
		return true;
	if (xit->the_par)
		return IsSomeParentCanvas(xit->the_par, 1);
	return false;
}

inline void XITEM::Unser(XML3::XMLElement& el)
{
	children.clear();
	CreateProperties(&el);
	LoadXMLPropertiesfor(el, properties);
	for (auto& e : el)
	{
		auto el2 = e.GetElementName();
		std::shared_ptr<XITEM> ch;
		if (el2 == "StackPanel")	ch = CreateXItemStackPanel();
		if (el2 == "SwapChainPanel")	ch = CreateXItemSwapChainPanel();
		if (el2 == "RelativePanel")	ch = CreateXItemRelativePanel();
		if (el2 == "Canvas")	ch = CreateXItemCanvas();
		if (el2 == "Grid")	ch = CreateXItemGrid();
		if (el2 == "Button")	ch = CreateXItemButton();
		if (el2 == "Expander")	ch = CreateXItemExpander();
		if (el2 == "HyperlinkButton")	ch = CreateXItemHLButton();
		if (el2 == "DropDownButton")	ch = CreateXItemDropDownButton();
		if (el2 == "AppBarButton")	ch = CreateXItemAppBarButton();
		if (el2 == "ToggleSwitch")	ch = CreateXItemToggleSwitch();
		if (el2 == "RepeatButton")	ch = CreateXItemRepeatButton();
		if (el2 == "CheckBox")	ch = CreateXItemCheckBox();
		if (el2 == "RadioButton")	ch = CreateXItemRadioButton();
		if (el2 == "MapControl")	ch = CreateXItemMapControl();
		if (el2 == "AnimatedVisualPlayer")	ch = CreateXItemAnimatedVisualPlayer();
		if (el2 == "RadioButton")	ch = CreateXItemRadioButton();
		if (el2 == "CalendarDatePicker")	ch = CreateXItemCalendarDatePicker();
		if (el2 == "CalendarView")	ch = CreateXItemCalendarView();
		if (el2 == "DatePicker")	ch = CreateXItemDatePicker();
		if (el2 == "TimePicker")	ch = CreateXItemTimePicker();
		if (el2 == "TextBlock")	ch = CreateXItemTextBlock();
		if (el2 == "TextBox")	ch = CreateXItemTextBox();
		if (el2 == "RichEditBox")	ch = CreateXItemRichEditBox();
		if (el2 == "ColorPicker")	ch = CreateXItemColorPicker();
		if (el2 == "NumberBox")	ch = CreateXItemNumberBox();
		if (el2 == "PasswordBox")	ch = CreateXItemPasswordBox();
		if (el2 == "BreadcrumbBar")	ch = CreateXItemBreadcrumbBar();
		if (el2 == "NavigationView")	ch = CreateXItemNavigationView();
		if (el2 == "NavigationViewItem")	ch = CreateXItemNavigationViewItem();
		if (el2 == "Pivot")	ch = CreateXItemPivot();
		if (el2 == "MenuBar")	ch = CreateXItemMenuBar();
		if (el2 == "CommandBar")	ch = CreateXItemCommandBar();
		if (el2 == "RatingControl")	ch = CreateXItemRatingControl();
		if (el2 == "Slider")	ch = CreateXItemSlider();
		if (el2 == "WebView2")	ch = CreateXItemWebView2();
		if (el2 == "Image")	ch = CreateXItemImage();
		if (el2 == "PersonPicture")	ch = CreateXItemPersonPicture();
		if (el2 == "MediaPlayerElement")	ch = CreateXItemMediaPlayerElement();
		if (el2 == "InfoBar")	ch = CreateXItemInfoBar();
		if (el2 == "InfoBadge")	ch = CreateXItemInfoBadge();
		if (el2 == "ProgressBar")	ch = CreateXItemProgressBar();
		if (el2 == "ProgressRing")	ch = CreateXItemProgressRing();
		if (el2 == "FlipView")	ch = CreateXItemFlipView();
		if (el2 == "GridView")	ch = CreateXItemGridView();
		if (el2 == "ListView")	ch = CreateXItemListView();
		if (el2 == "ListBox")	ch = CreateXItemListBox();
		if (el2 == "ComboBox")	ch = CreateXItemComboBox();
		if (el2 == "TreeView")	ch = CreateXItemTreeView();
		if (!ch)
			continue;
		ch->Unser(e);
		children.push_back(ch);
	}
}

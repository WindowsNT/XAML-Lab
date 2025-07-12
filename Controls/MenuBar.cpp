#include "pch.h"
#include "property.hpp"
extern std::map<HWND, winrt::Windows::Foundation::IInspectable> windows;
void LoadMenuNames(std::vector<std::wstring>& names, XML3::XMLElement&);

class ITEM_MenuBar : public XITEM_Control
{
public:

	ITEM_MenuBar()
	{
		ElementName = L"MenuBar";
		X = MenuBar();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{
			auto e = X.as<MenuBar>();
			for (auto& p : properties)
			{
				if (p->n == L"Menu")
				{
					auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
					if (op)
					{
						if (op->SelectedIndex < 0 || op->SelectedIndex >= op->Items.size())
							continue;
						std::wstring menu_name = op->Items[op->SelectedIndex];
						if (menu_name == s(48))
						{
							e.Items().Clear();
						}
						else
						{
							std::vector<std::wstring> names;
							XML3::XMLElement menuEl;
							LoadMenuNames(names, menuEl);
							for(size_t i = 0 ; i < names.size() ; i++)
							{
								if (names[i] == menu_name)
								{
									auto& item = menuEl.GetChildren()[i];
									item;
									void ProjectXMLElementToMenuBar(winrt::Microsoft::UI::Xaml::Controls::MenuBar mb,XML3::XMLElement & e);
									ProjectXMLElementToMenuBar(e, *item);
									break;
								}
							}
						}
					}
				}
			}
		}
		catch (...)
		{

		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		el;
		auto e = X.as<MenuBar>();
		if (!properties.empty())
			return properties;

		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->S = 1;
			op->g = L"MenuBar";
			op->n = L"Menu";
			op->tip = s(74);
			op->special_type = 5; // MenuBar Menu	a
			

			std::vector<std::wstring> names;
			XML3::XMLElement menuEl;
			LoadMenuNames(names,menuEl);
			op->Items.push_back(s(48));
			for(auto& n : names)
			{
				op->Items.push_back(n);
			}	
			op->DefaultIndex = 0;
			if (el)
			{
				auto vn = el->vv("Menu").GetWideValue();
				for (size_t i = 0; i < op->Items.size(); i++)
				{
					if (op->Items[i] == vn)
					{
						op->SelectedIndex = (int)i;
						break;
					}
				}
			}
			properties.push_back(op);

		}


		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = MenuBar();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<MenuBar>();
		b.Tag(box_value((long long)this));
		b.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Left);
		b.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Top);
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemMenuBar()
{
	return std::make_shared< ITEM_MenuBar>();
}

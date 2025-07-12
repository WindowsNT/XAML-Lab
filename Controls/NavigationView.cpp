#include "pch.h"
#include "property.hpp"
#include "blankwindow.g.h"
#include "MainPage.g.h"
#include "..\\project.h"
void LoadPageNames(std::vector<std::wstring>& names, XML3::XMLElement&);

class ITEM_NavigationView : public XITEM_ContentControl
{
public:

	ITEM_NavigationView()
	{
		ElementName = L"NavigationView";
		X = NavigationView();
	}


	virtual void ApplyProperties()
	{
		XITEM_ContentControl::ApplyProperties();
		auto b = X.try_as<NavigationView>();
		if (!b)
			return;
		for (auto& p : properties)
		{
			if (p->n == L"PaneDisplayMode")
			{
				auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
				if (op)
				{
					b.PaneDisplayMode((winrt::Microsoft::UI::Xaml::Controls::NavigationViewPaneDisplayMode)op->SelectedIndex);
				}
			}
			if (p->n == L"IsSettingsVisible")
			{
				auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
				if (op)
				{
					b.IsSettingsVisible(op->SelectedIndex);
				}
			}
			if (p->n == L"IsPaneOpen")
			{
				auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
				if (op)
				{
					b.IsPaneOpen(op->SelectedIndex);
				}
			}
			if (p->n == L"IsBackButtonVisible")
			{
				auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
				if (op)
				{
					b.IsBackButtonVisible((winrt::Microsoft::UI::Xaml::Controls::NavigationViewBackButtonVisible)op->SelectedIndex);
				}
			}
		}


		for (auto& p : properties)
		{
			static bool InBuild = 0;
			if (p->n == L"Page" && InBuild == 0)
			{
			
				auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
				if (op)
				{
					if (op->SelectedIndex < 0 || op->SelectedIndex >= op->Items.size())
						continue;
					std::wstring menu_name = op->Items[op->SelectedIndex];
					if (menu_name == s(48))
					{
						b.Content(nullptr);
					}
					else
					{
						try
						{
							auto co = b.Content();
							if (co)
							{
								auto page = co.try_as<winrt::Microsoft::UI::Xaml::Controls::Page>();
								if (page)
								{
									auto pc = page.Content();
									if (pc)
									{
										auto tag = page.Tag();
										if (tag)
										{
											auto tag_value = winrt::unbox_value<winrt::hstring>(tag);
											if (tag_value == menu_name)
											{
												return; // Already loaded
											}
										}
									}
								}
							}
						}
						catch (...)
						{

						}
						std::vector<std::wstring> names;
						XML3::XMLElement menuEl;
						LoadPageNames(names, menuEl);
						for (size_t i = 0; i < names.size(); i++)
						{
							if (names[i] == menu_name)
							{
								auto& item = menuEl.GetChildren()[i];
								item;

								winrt::Microsoft::UI::Xaml::Controls::Page Page;
								PROJECT* prj = new PROJECT;
								XML3::XMLElement elx = *item->GetChildren()[0]; // make a copy
								prj->Unser(elx);
								InBuild = 1;
								UIElement BuildThisProject(PROJECT & prj);
								auto u = BuildThisProject(*prj);
								InBuild = 0;
								Page.Tag(box_value(menu_name.c_str()));
								Page.Content(u);

								b.Content(Page);
								break;
							}
						}
					}
				}
			}
		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;

		auto b = X.try_as<NavigationView>();

		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->S = 1;
			op->g = L"NavigationView";
			op->n = L"PaneDisplayMode";
			op->Items = { L"Auto", L"Left", L"Top", L"LeftCompact", L"LeftMinimal" };
			op->SelectedIndex = (int)b.PaneDisplayMode();
			properties.push_back(op);
		}
		if (1)
		{
			// IsSettingsVisible
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"NavigationView";
			op->n = L"IsSettingsVisible";
			op->SelectedIndex = b.IsSettingsVisible();
			properties.push_back(op);
		}
		if (1)
		{
			// IsPaneOpen
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"NavigationView";
			op->n = L"IsPaneOpen";
			op->SelectedIndex = b.IsPaneOpen();
			properties.push_back(op);
		}
		if (1)
		{
			// IsBackButtonVisible
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"NavigationView";
			op->n = L"IsBackButtonVisible";
			op->Items = { L"Collapsed", L"Visible", L"Auto" };
			op->SelectedIndex = (int)b.IsBackButtonVisible();
			properties.push_back(op);

		}

		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"NavigationView";
			op->n = L"Page";
			op->tip = s(73);
			op->special_type = 7; // Page

			std::vector<std::wstring> names;
			XML3::XMLElement menuEl;
			LoadPageNames(names, menuEl);
			op->Items.push_back(s(48));
			for (auto& n : names)
			{
				op->Items.push_back(n);
			}
			op->DefaultIndex = 0;
			if (el)
			{
				auto vn = el->vv("Page").GetWideValue();
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

		auto p2 = XITEM_ContentControl::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = NavigationView();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}
		auto b = X.as<NavigationView>();
		b.Tag(box_value((long long)this));
		b.Content(winrt::box_value(L"NavigationView"));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemNavigationView()
{
	return std::make_shared< ITEM_NavigationView>();
}

bool IsNavigationView(std::shared_ptr<XITEM> xit)
{
	return std::dynamic_pointer_cast<ITEM_NavigationView>(xit) != nullptr;
}


bool ISXItemNavigationView(XITEM* xit)
{
	if (!xit)
		return false;
	return dynamic_cast<ITEM_NavigationView*>(xit) != nullptr;
}


void ApplyNVProperties(NavigationView nv, std::vector<std::shared_ptr<PROPERTY>> ps)
{
	ITEM_NavigationView panel;
	panel.properties = ps;
	panel.X = nv;
	panel.ApplyProperties();

}


#include "BlankWindow.g.h"
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Windows::Foundation;
#pragma warning(disable: 4100)
void BuildNavigationView(winrt::VisualWinUI3::BlankWindow topbw, UIElement iroot, std::shared_ptr<XITEM> root, winrt::Windows::Foundation::IInspectable menu_root, int ForWhat, std::shared_ptr<XITEM> parentroot)
{
	auto cb = std::dynamic_pointer_cast<ITEM_NavigationView>(root);
	if (!cb)
		return;
	auto b = iroot.try_as<NavigationView>();
	if (!b)
		return;

	b.MenuItems().Clear();
	b.FooterMenuItems().Clear();
	for (auto& r : root->children)
	{
		auto item = r->Create(ForWhat, root.get());
		if (item)
		{
			r->ApplyProperties();

			bool IsFooter = 0;
			// Check properties
			for (auto& p : r->properties)
			{
				if (p->n == L"Footer")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						IsFooter = (op->SelectedIndex != 0);
					}
				}
			}

			auto cmd = item.try_as<winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem>();
			if (cmd)
			{
				cmd.Tag(box_value((long long)r.get()));
				if (IsFooter)
					b.FooterMenuItems().Append(cmd);
				else
					b.MenuItems().Append(cmd);
			}
		}
	}

}
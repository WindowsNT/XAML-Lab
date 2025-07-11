#include "pch.h"
#include "property.hpp"

class ITEM_CommandBar : public XITEM_ContentControl
{
public:

	ITEM_CommandBar()
	{
		ElementName = L"CommandBar";
		X = CommandBar();
	}


	virtual void ApplyProperties()
	{
		XITEM_ContentControl::ApplyProperties();
		for(auto& p : properties)
		{
			if (p->n == L"Sticky")
			{
				auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
				if (op)
				{
					auto b = X.try_as<CommandBar>();
					if (b)
					{
						b.IsSticky(op->SelectedIndex != 0);
					}
				}
			}
			if (p->n == L"DefaultLabelPosition")
			{
				auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
				if (op)
				{
					auto b = X.try_as<CommandBar>();
					if (b)
					{
						b.DefaultLabelPosition((winrt::Microsoft::UI::Xaml::Controls::CommandBarDefaultLabelPosition)op->SelectedIndex);
					}
				}
			}
		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;

		auto b = X.try_as<CommandBar>();
		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"CommandBar";
			op->S = 1;
			op->n = L"DefaultLabelPosition";
			op->Items = { L"Bottom",L"Right",L"Collapsed"};
			op->DefaultIndex = 0;
			op->SelectedIndex = (int)b.DefaultLabelPosition();
			properties.push_back(op);

		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"CommandBar";
			op->n = L"Sticky";
			op->DefaultIndex = 0;
			op->SelectedIndex = b.IsSticky() ? 1 : 0;
			properties.push_back(op);
		}

		auto p2 = XITEM_ContentControl::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = CommandBar();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}
		auto b = X.as<CommandBar>();
		b.Tag(box_value((long long)this));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemCommandBar()
{
	return std::make_shared< ITEM_CommandBar>();
}


bool IsCommandBar(std::shared_ptr<XITEM> xit)
{
	return std::dynamic_pointer_cast<ITEM_CommandBar>(xit) != nullptr;
}

#include "BlankWindow.g.h"
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Windows::Foundation;
#pragma warning(disable: 4100)
void BuildCommandBar(winrt::VisualWinUI3::BlankWindow topbw, UIElement iroot, std::shared_ptr<XITEM> root, winrt::Windows::Foundation::IInspectable menu_root, int ForWhat, std::shared_ptr<XITEM> parentroot)
{
	auto cb = std::dynamic_pointer_cast<ITEM_CommandBar>(root);	
	if (!cb)
		return;
	auto b = iroot.try_as<CommandBar>();
	if (!b)
		return;
	b.PrimaryCommands().Clear();
	for (auto& r : root->children)
	{
		auto item = r->Create(ForWhat, root.get());
		if (item)
		{
			r->ApplyProperties();

			bool IsSecondary = 0;
			// Check properties
			for (auto& p : r->properties)
			{
				if (p->n == L"Secondary")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						IsSecondary = (op->SelectedIndex != 0);
					}
				}
			}

			auto cmd = item.try_as<winrt::Microsoft::UI::Xaml::Controls::AppBarButton>();
			if (cmd)
			{
				cmd.Tag(box_value((long long)r.get()));
				if (IsSecondary)
					b.SecondaryCommands().Append(cmd);
				else
					b.PrimaryCommands().Append(cmd);
			}
			else
			{
				auto cmd2 = item.try_as<winrt::Microsoft::UI::Xaml::Controls::AppBarToggleButton>();
				if (cmd2)
				{
					cmd2.Tag(box_value((long long)r.get()));
					if (IsSecondary)
						b.SecondaryCommands().Append(cmd2);
					else
						b.PrimaryCommands().Append(cmd2);
				}
				else
				{
					auto cmd3 = item.try_as<winrt::Microsoft::UI::Xaml::Controls::AppBarSeparator>();
					if (cmd3)
					{
						if (IsSecondary)
							b.SecondaryCommands().Append(cmd3);
						else
							b.PrimaryCommands().Append(cmd3);
					}
				}
			}
		}
	}
}
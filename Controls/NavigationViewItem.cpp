#include "pch.h"
#include "property.hpp"

class ITEM_NavigationViewItem : public XITEM_ContentControl
{
public:

	ITEM_NavigationViewItem()
	{
		ElementName = L"NavigationViewItem";
		X = NavigationViewItem();
	}


	virtual void ApplyProperties()
	{
		XITEM_ContentControl::ApplyProperties();
		auto e = X.as<NavigationViewItem>();
		ApplyPropertiesForIcon(e, properties);
		for(auto& p : properties)
		{
			p;
		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;

		auto b = X.try_as<NavigationViewItem>();
		
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->S = 1;
			op->g = L"NavigationViewItem";
			op->n = L"Footer";
			op->DefaultIndex = 0;
			op->SelectedIndex = 0;
			properties.push_back(op);
		}
		if (1)
		{
			auto uip = CreatePropertiesForIcon<NavigationViewItem>(b);
			for (auto& p : uip)
			{
				p->g = L"NavigationViewItem";
				properties.push_back(p);
			}
		}

		auto p2 = XITEM_ContentControl::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = NavigationViewItem();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}
		auto b = X.as<NavigationViewItem>();
		b.Tag(box_value((long long)this));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemNavigationViewItem()
{
	return std::make_shared< ITEM_NavigationViewItem>();
}


bool IsNavigationViewItem(std::shared_ptr<XITEM> xit)
{
	return std::dynamic_pointer_cast<ITEM_NavigationViewItem>(xit) != nullptr;
}

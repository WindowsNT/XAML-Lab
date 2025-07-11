#include "pch.h"
#include "property.hpp"

class ITEM_Expander : public XITEM_ContentControl
{
public:

	ITEM_Expander()
	{
		ElementName = L"Expander";
		X = Expander();
	}


	virtual void ApplyProperties()
	{
		XITEM_ContentControl::ApplyProperties();
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;
		auto p2 = XITEM_ContentControl::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = Expander();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}
		auto b = X.as<Expander>();
		b.Tag(box_value((long long)this));
		b.Content(winrt::box_value(L"Expander"));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemExpander()
{
	return std::make_shared< ITEM_Expander>();
}

#include "pch.h"
#include "property.hpp"

class ITEM_CHECKBOX : public XITEM_ToggleButton
{
public:

	ITEM_CHECKBOX()
	{
		ElementName = L"CheckBox";
		X = CheckBox();
	}


	virtual void ApplyProperties()
	{
		XITEM_ToggleButton::ApplyProperties();
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		el;
		auto e = X.as<CheckBox>();
		if (!properties.empty())
			return properties;
		auto p2 = XITEM_ToggleButton::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = CheckBox();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<CheckBox>();
		b.Tag(box_value((long long)this));
		b.Content(winrt::box_value(L"CheckBox"));
		return b;
	}




};


std::shared_ptr<XITEM> CreateXItemCheckBox()
{
	return std::make_shared< ITEM_CHECKBOX>();
}

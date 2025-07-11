#include "pch.h"
#include "property.hpp"

class ITEM_RadioButton : public XITEM_ToggleButton
{
public:

	ITEM_RadioButton()
	{
		ElementName = L"RadioButton";
		X = RadioButton();
	}


	virtual void ApplyProperties()
	{
		XITEM_ToggleButton::ApplyProperties();
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		el;
		auto e = X.as<RadioButton>();
		if (!properties.empty())
			return properties;
		auto p2 = XITEM_ToggleButton::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = RadioButton();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<RadioButton>();
		b.Tag(box_value((long long)this));
		b.Content(winrt::box_value(L"RadioButton"));
		return b;
	}




};


std::shared_ptr<XITEM> CreateXItemRadioButton()
{
	return std::make_shared< ITEM_RadioButton>();
}

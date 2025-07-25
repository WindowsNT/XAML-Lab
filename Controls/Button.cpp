#include "pch.h"
#include "property.hpp"

class XITEM_Button : public XITEM_ButtonBase
{
public:

	XITEM_Button()
	{
		ElementName = L"Button";
		X = Button();
	}


	virtual void ApplyProperties()
	{
		XITEM_ButtonBase::ApplyProperties();
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;
		auto p2 = XITEM_ButtonBase::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = Button();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}
		auto b = X.as<Button>();
		b.Tag(box_value((long long)this));
		b.Content(winrt::box_value(L"Button"));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemButton()
{
	return std::make_shared< XITEM_Button>();
}

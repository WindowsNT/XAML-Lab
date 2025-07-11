#include "pch.h"
#include "property.hpp"

using namespace Microsoft::UI::Xaml::Controls::Primitives;

class ITEM_RepeatButton : public XITEM_ButtonBase
{
public:

	ITEM_RepeatButton()
	{
		ElementName = L"RepeatButton";
		X = RepeatButton();
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
		X = RepeatButton();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}
		auto b = X.as<RepeatButton>();
		b.Tag(box_value((long long)this));
		b.Content(winrt::box_value(L"RepeatButton"));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemRepeatButton()
{
	return std::make_shared< ITEM_RepeatButton>();
}

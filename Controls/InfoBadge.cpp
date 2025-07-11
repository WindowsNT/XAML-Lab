#include "pch.h"
#include "property.hpp"

class ITEM_InfoBadge : public XITEM_Control
{
public:

	ITEM_InfoBadge()
	{
		ElementName = L"InfoBadge";
		X = InfoBadge();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{

			auto e = X.as<InfoBadge>();
			for (auto& p : properties)
			{
				p;
			}
		}
		catch (...)
		{

		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		el;
		auto e = X.as<InfoBadge>();
		if (!properties.empty())
			return properties;

		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = InfoBadge();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<InfoBadge>();
		b.Tag(box_value((long long)this));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemInfoBadge()
{
	return std::make_shared< ITEM_InfoBadge>();
}

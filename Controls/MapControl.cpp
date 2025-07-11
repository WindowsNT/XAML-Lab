#include "pch.h"
#include "property.hpp"

class ITEM_MapControl : public XITEM_Control
{
public:

	ITEM_MapControl()
	{
		ElementName = L"MapControl";
		X = MapControl();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{
			auto e = X.as<MapControl>();
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
		auto e = X.as<MapControl>();
		if (!properties.empty())
			return properties;
		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = MapControl();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<MapControl>();
		b.MinWidth(200);
		b.MinHeight(200);
		b.Tag(box_value((long long)this));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemMapControl()
{
	return std::make_shared< ITEM_MapControl>();
}

#include "pch.h"
#include "property.hpp"

class ITEM_NumberBox : public XITEM_Control
{
public:
	ITEM_NumberBox()
	{
		ElementName = L"NumberBox";
		X = NumberBox();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		ApplyPropertiesForText(X.as<NumberBox>(), properties);
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;
		if (1)
		{
			auto uip = CreatePropertiesForText<NumberBox>(X.as<NumberBox>(), L"NumberBox");
			for (auto& p : uip)
			{
				p->g = L"NumberBox";
				properties.push_back(p);
			}
		}
		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = NumberBox();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}
		auto b = X.as<NumberBox>();
		b.Tag(box_value((long long)this));
		if (ForWhat == 0)
			b.Text(L"Hello");
		return b;
	}




};


std::shared_ptr<XITEM> CreateXItemNumberBox()
{
	return std::make_shared< ITEM_NumberBox>();
}

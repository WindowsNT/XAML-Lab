#include "pch.h"
#include "property.hpp"

class ITEM_TEXTBOX : public XITEM_Control
{
public:
	ITEM_TEXTBOX()
	{
		ElementName = L"TextBox";
		X = TextBox();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		ApplyPropertiesForText(X.as<TextBox>(), properties);
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		properties.clear();
		if (1)
		{
			auto uip = CreatePropertiesForText<TextBox>(X.as<TextBox>(), L"TextBox");
			for (auto& p : uip)
			{
				p->g = L"TextBox";
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
		X = TextBox();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}
		auto b = X.as<TextBox>();
		b.Tag(box_value((long long)this));
		if (ForWhat == 0)
			b.Text(L"Hello");
		return b;
	}




};


std::shared_ptr<XITEM> CreateXItemTextBox()
{
	return std::make_shared< ITEM_TEXTBOX>();
}

#include "pch.h"
#include "property.hpp"

class ITEM_TEXTBLOCK : public XITEM_FrameworkElement
{
public:

	ITEM_TEXTBLOCK()
	{
		ElementName = L"TextBlock";
		X = TextBlock();
	}


	virtual void ApplyProperties()
	{
		XITEM_FrameworkElement::ApplyProperties();
		ApplyPropertiesForForeground(X.as<TextBlock>(), properties);
		ApplyPropertiesForFont(X.as<TextBlock>(), properties);
		ApplyPropertiesForText(X.as<TextBlock>(), properties);
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;
		if (1)
		{
			auto uip = CreatePropertiesForText<TextBlock>(X.as<TextBlock>(), L"TextBlock");
			for (auto& p : uip)
			{
				p->g = L"TextBlock";
				p->S = 1;
				properties.push_back(p);
			}


			if (1)
			{
				auto uip1 = CreatePropertiesForFont(X.as<TextBlock>());
				for (auto& p : uip1)
				{
					p->g = L"TextBlock";
					properties.push_back(p);
				}
			}
			if (1)
			{
				auto uip1 = CreatePropertiesForForeground(X.as<TextBlock>());
				for (auto& p : uip1)
				{
					p->g = L"TextBlock";
					properties.push_back(p);
				}
			}


		}
		auto p2 = XITEM_FrameworkElement::CreateProperties(el);
		for (auto& p : p2)
			properties.push_back(p);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = TextBlock();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}
		auto b = X.as<TextBlock>();
		b.Tag(box_value((long long)this));
		if (ForWhat == 0)
			b.Text(L"Hello");
		return b;
	}

};


std::shared_ptr<XITEM> CreateXItemTextBlock()
{
	return std::make_shared< ITEM_TEXTBLOCK>();
}

#include "pch.h"
#include "property.hpp"

class ITEM_RichTextBlock : public XITEM_FrameworkElement
{
public:

	ITEM_RichTextBlock()
	{
		ElementName = L"RichTextBlock";
		X = RichTextBlock();
	}


	virtual void ApplyProperties()
	{
		XITEM_FrameworkElement::ApplyProperties();
		ApplyPropertiesForForeground(X.as<RichTextBlock>(), properties);
		ApplyPropertiesForFont(X.as<RichTextBlock>(), properties);
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;
		if (1)
		{
		
			if (1)
			{
				auto uip1 = CreatePropertiesForFont(X.as<RichTextBlock>());
				for (auto& p : uip1)
				{
					p->g = L"RichTextBlock";
					properties.push_back(p);
				}
			}
			if (1)
			{
				auto uip1 = CreatePropertiesForForeground(X.as<RichTextBlock>());
				for (auto& p : uip1)
				{
					p->g = L"RichTextBlock";
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
		X = RichTextBlock();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}
		auto b = X.as<RichTextBlock>();
		b.Tag(box_value((long long)this));
		return b;
	}

};


std::shared_ptr<XITEM> CreateXItemRichTextBlock()
{
	return std::make_shared< ITEM_RichTextBlock>();
}

#include "pch.h"
#include "property.hpp"

class ITEM_HLBUTTON : public XITEM_ButtonBase
{
public:

	ITEM_HLBUTTON()
	{
		ElementName = L"HyperlinkButton";
		X = HyperlinkButton();
	}


	virtual void ApplyProperties()
	{
		XITEM_ButtonBase::ApplyProperties();
		try
		{
			auto e = X.try_as<HyperlinkButton>();
			for (auto& p : properties)
			{
				if (p->n == L"NavigateUri")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						e.NavigateUri(winrt::Windows::Foundation::Uri(op->value));
					}
				}
			}

		}
		catch (...)
		{

		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;
		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->S = 1;
			op->g = L"HyperlinkButton";
			op->n = L"NavigateUri";
			op->def = L"https://www.turbo-play.com";
			op->value = L"https://www.turbo-play.com";
			properties.push_back(op);

		}
		auto p2 = XITEM_ButtonBase::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = HyperlinkButton();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<HyperlinkButton>();
		b.Tag(box_value((long long)this));
		b.Content(winrt::box_value(L"HyperlinkButton"));
		return b;
	}


};


std::shared_ptr<XITEM> CreateXItemHLButton()
{
	return std::make_shared< ITEM_HLBUTTON>();
}

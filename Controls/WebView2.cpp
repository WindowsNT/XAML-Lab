#include "pch.h"
#include "property.hpp"

class ITEM_WebView2 : public XITEM_FrameworkElement
{
public:

	ITEM_WebView2()
	{
		ElementName = L"WebView2";
		X = WebView2();
	}


	virtual void ApplyProperties()
	{
		XITEM_FrameworkElement::ApplyProperties();
		try
		{
			auto e = X.try_as<WebView2>();
			for (auto& p : properties)
			{
				if (p->n == L"Source")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						if (op->value.length())
							e.Source(winrt::Windows::Foundation::Uri(op->value));
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
		properties.clear();
		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"WebView2";
			op->n = L"Source";
			properties.push_back(op);

		}
		auto p2 = XITEM_FrameworkElement::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = WebView2();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<WebView2>();
		b.MinWidth(200);
		b.MinHeight(200);
		b.Tag(box_value((long long)this));
		b.Source(winrt::Windows::Foundation::Uri(L"https://www.turbo-play.com"));
		return b;
	}


};


std::shared_ptr<XITEM> CreateXItemWebView2()
{
	return std::make_shared< ITEM_WebView2>();
}

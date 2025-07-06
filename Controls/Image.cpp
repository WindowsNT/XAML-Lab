#include "pch.h"
#include "property.hpp"

class ITEM_Image : public XITEM_FrameworkElement
{
public:

	ITEM_Image()
	{
		ElementName = L"Image";
		X = Image();
	}


	virtual void ApplyProperties()
	{
		XITEM_FrameworkElement::ApplyProperties();
		try
		{
			auto e = X.try_as<Image>();
			for (auto& p : properties)
			{
				if (p->n == L"Source")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						if (op->value.length())
						{
							winrt::Windows::Foundation::Uri uri{ op->value };
							winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage bitmap{};
							bitmap.UriSource(uri);
							e.Source(bitmap);
						}
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
			op->g = L"Image";
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
		X = Image();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<Image>();
		b.MinWidth(200);
		b.MinHeight(200);
		b.Tag(box_value((long long)this));

		winrt::Windows::Foundation::Uri uri{ L"https://www.turbo-play.com/logo128.png" }; // your image URL here
		winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage bitmap{};
		bitmap.UriSource(uri);
		b.Source(bitmap);
		return b;
	}


};


std::shared_ptr<XITEM> CreateXItemImage()
{
	return std::make_shared< ITEM_Image>();
}

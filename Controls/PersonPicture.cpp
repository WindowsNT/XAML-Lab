#include "pch.h"
#include "property.hpp"

class ITEM_PersonPicture : public XITEM_FrameworkElement
{
public:

	ITEM_PersonPicture()
	{
		ElementName = L"PersonPicture";
		X = PersonPicture();
	}


	virtual void ApplyProperties()
	{
		XITEM_FrameworkElement::ApplyProperties();
		try
		{
			auto e = X.try_as<PersonPicture>();
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
							e.ProfilePicture(bitmap);
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
		if (!properties.empty())
			return properties;
		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->S = 1;
			op->g = L"PersonPicture";
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
		X = PersonPicture();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<PersonPicture>();
		b.MinWidth(200);
		b.MinHeight(200);
		b.Tag(box_value((long long)this));

		winrt::Windows::Foundation::Uri uri{ L"https://www.turbo-play.com/logo128.png" }; // your PersonPicture URL here
		winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage bitmap{};
		bitmap.UriSource(uri);
		b.ProfilePicture(bitmap);
		return b;
	}


};


std::shared_ptr<XITEM> CreateXItemPersonPicture()
{
	return std::make_shared< ITEM_PersonPicture>();
}

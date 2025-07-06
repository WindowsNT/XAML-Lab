#include "pch.h"
#include "property.hpp"

class ITEM_ProgressBar : public XITEM_Control
{
public:

	ITEM_ProgressBar()
	{
		ElementName = L"ProgressBar";
		X = ProgressBar();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{
			auto e = X.as<ProgressBar>();
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
		auto e = X.as<ProgressBar>();
		properties.clear();


		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = ProgressBar();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<ProgressBar>();
		b.Tag(box_value((long long)this));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemProgressBar()
{
	return std::make_shared< ITEM_ProgressBar>();
}

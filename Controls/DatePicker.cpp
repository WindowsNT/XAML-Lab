#include "pch.h"
#include "property.hpp"

class ITEM_DATEPICKER : public XITEM_Control
{
public:

	ITEM_DATEPICKER()
	{
		ElementName = L"DatePicker";
		X = DatePicker();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{
			auto e = X.as<DatePicker>();
			// set date to 2021-10-05

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
		properties.clear();
		auto e = X.as<DatePicker>();

		auto p2 = XITEM_Control::CreateProperties(el);
		for(auto p : p2)
			properties.push_back(p);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = DatePicker();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<DatePicker>();
		b.Tag(box_value((long long)this));
		return b;
	}


};


std::shared_ptr<XITEM> CreateXItemDatePicker()
{
	return std::make_shared< ITEM_DATEPICKER>();
}

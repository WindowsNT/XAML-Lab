#include "pch.h"
#include "property.hpp"

class ITEM_TimePICKER : public XITEM_Control
{
public:

	ITEM_TimePICKER()
	{
		ElementName = L"TimePicker";
		X = TimePicker();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{
			auto e = X.as<TimePicker>();
			// set Time to 2021-10-05

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
		auto e = X.as<TimePicker>();

		auto p2 = XITEM_Control::CreateProperties(el);
		for(auto p : p2)
			properties.push_back(p);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = TimePicker();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<TimePicker>();
		b.Tag(box_value((long long)this));
		return b;
	}


};


std::shared_ptr<XITEM> CreateXItemTimePicker()
{
	return std::make_shared< ITEM_TimePICKER>();
}

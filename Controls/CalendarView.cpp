#include "pch.h"
#include "property.hpp"

class ITEM_CALENDARView : public XITEM_Control
{
public:

	ITEM_CALENDARView()
	{
		ElementName = L"CalendarView";
		X = CalendarView();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{
			auto e = X.as<CalendarView>();

			for (auto& p : properties)
			{
				if (p->n == L"DisplayMode")
				{
					auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
					if (op)
					{
						e.DisplayMode((winrt::Microsoft::UI::Xaml::Controls::CalendarViewDisplayMode)op->SelectedIndex);
					}
				}
				if (p->n == L"FirstDayOfWeek")
				{
					auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
					if (op)
					{
						e.FirstDayOfWeek((winrt::Windows::Globalization::DayOfWeek)op->SelectedIndex);
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
		auto e = X.as<CalendarView>();

		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"CalendarView";
			op->n = L"DisplayMode";
			op->Items = { L"Month", L"Year",L"Decade"};
			op->DefaultIndex = 0;
			op->SelectedIndex = (size_t)e.DisplayMode();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"CalendarView";
			op->n = L"FirstDayOfWeek";
			op->Items = { L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday" };
			op->DefaultIndex = 0;
			op->SelectedIndex = (size_t)e.FirstDayOfWeek();
			properties.push_back(op);
		}

		auto p2 = XITEM_Control::CreateProperties(el);
		for(auto p : p2)
			properties.push_back(p);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = CalendarView();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<CalendarView>();
		b.Tag(box_value((long long)this));
		return b;
	}


};


std::shared_ptr<XITEM> CreateXItemCalendarView()
{
	return std::make_shared< ITEM_CALENDARView>();
}

#include "pch.h"
#include "property.hpp"

class ITEM_CALENDARDATEPICKER : public XITEM_Control
{
public:

	ITEM_CALENDARDATEPICKER()
	{
		ElementName = L"CalendarDatePicker";
		X = CalendarDatePicker();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{
			auto e = X.as<CalendarDatePicker>();
			// set date to 2021-10-05

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
				if (p->n == L"IsCalendarOpen")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.IsCalendarOpen(op->SelectedIndex);
					}
				}


				if (p->n == L"DateFormat")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
						e.DateFormat(op->value);
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
		auto e = X.as<CalendarDatePicker>();

		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"CalendarDatePicker";
			op->n = L"DateFormat";
			op->value = e.DateFormat();
			op->def = e.DateFormat();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"CalendarDatePicker";
			op->n = L"DisplayMode";
			op->Items = { L"Month", L"Year",L"Decade"};
			op->DefaultIndex = 0;
			op->SelectedIndex = (size_t)e.DisplayMode();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"CalendarDatePicker";
			op->n = L"FirstDayOfWeek";
			op->Items = { L"Sunday", L"Monday", L"Tuesday", L"Wednesday", L"Thursday", L"Friday", L"Saturday" };
			op->DefaultIndex = 0;
			op->SelectedIndex = (size_t)e.FirstDayOfWeek();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"CalendarDatePicker";
			op->n = L"IsCalendarOpen";
			op->DefaultIndex = 0;
			op->SelectedIndex = e.IsCalendarOpen() ? 1 : 0;
			properties.push_back(op);
		}

		auto p2 = XITEM_Control::CreateProperties(el);
		for(auto p : p2)
			properties.push_back(p);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = CalendarDatePicker();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<CalendarDatePicker>();
		b.Tag(box_value((long long)this));
		return b;
	}


};


std::shared_ptr<XITEM> CreateXItemCalendarDatePicker()
{
	return std::make_shared< ITEM_CALENDARDATEPICKER>();
}

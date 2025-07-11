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


	winrt::Windows::Foundation::TimeSpan CreateTimeSpan(int hours, int minutes)
	{
		using namespace std::chrono;

		// filetime_period = 100 nanoseconds
		constexpr int64_t ticks_per_hour = 3600LL * 10'000'000LL; // 3600s * 10M ticks/s
		constexpr int64_t ticks_per_minute = 60LL * 10'000'000LL;

		int64_t totalTicks = hours * ticks_per_hour + minutes * ticks_per_minute;

		return winrt::Windows::Foundation::TimeSpan{ totalTicks };
	}

	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{
			auto e = X.as<TimePicker>();

			for (auto& p : properties)
			{
				if (p->n == L"ClockIdentifier")
				{
					auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
					if (op)
					{
						if (op->SelectedIndex == 0)
							e.ClockIdentifier(L"12HourClock");
						else
							e.ClockIdentifier(L"24HourClock");
					}
				}
				if (p->n == L"MinuteIncrement")
				{
					auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (op)
					{
						e.MinuteIncrement(static_cast<int>(op->value));
					}
				}
				if (p->n == L"SelectedTime")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						if (op->value.length())
						{
							auto time = winrt::Windows::Foundation::TimeSpan(0);
							auto parts = split(op->value, L':');
							if (parts.size() == 2)
							{
								int hours = std::stoi(parts[0]);
								int minutes = std::stoi(parts[1]);
								e.SelectedTime(CreateTimeSpan(hours,minutes));
							}
							else
							{
								e.SelectedTime(winrt::Windows::Foundation::TimeSpan{ 0 });
							}
						}
						else
						{
							e.SelectedTime(winrt::Windows::Foundation::TimeSpan{ 0 });
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
		auto e = X.as<TimePicker>();

		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"TimePicker";
			op->S = 1;
			op->n = L"ClockIdentifier";
			op->Items = { L"12HourClock", L"24HourClock" };
			op->DefaultIndex = 0;
			op->SelectedIndex = e.ClockIdentifier() == L"12HourClock" ? 0 : 1;
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"TimePicker";
			op->n = L"SelectedTime";
			op->def = L"00:00";
			try
			{
				op->value = L"00:00";
				auto selectedTime = e.SelectedTime();
				auto ct2 = selectedTime.try_as<winrt::Windows::Foundation::TimeSpan>();
				if (ct2 && ct2.has_value())
				{
					// Get duration
					auto ticks = ct2.value().count();
					int hours = static_cast<int>(ticks / 36000000000LL); // 1 hour = 36,000,000,000 ticks
					int minutes = static_cast<int>((ticks % 36000000000LL) / 600000000LL); // 1 minute = 600,000,000 ticks
					op->value = std::to_wstring(hours) + L":" + (minutes < 10 ? L"0" : L"") + std::to_wstring(minutes);
				}
			}
			catch (...)
			{
				op->value = L"00:00";
			}
			properties.push_back(op);

		}

		if (1)
		{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->g = L"TimePicker";
			op->n = L"MinuteIncrement";
			op->mmin = 1;
			op->mmax = 60;
			op->smallchange = 1;
			op->def = 15;
			try
			{
				op->value = 15;
				op->value = e.MinuteIncrement();
			}
			catch (...)
			{
			}
			properties.push_back(op);

		}

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
		auto b = X.as<TimePicker>();

		
		if (ForWhat == 0)
		{
			AllTap(X);
		}

		b.Tag(box_value((long long)this));
		return b;
	}


};


std::shared_ptr<XITEM> CreateXItemTimePicker()
{
	return std::make_shared< ITEM_TimePICKER>();
}

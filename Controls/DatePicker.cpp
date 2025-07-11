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


	winrt::Windows::Foundation::DateTime DateFromYear(int Year)
	{
		std::tm tm = {};
		tm.tm_year = Year - 1900; // years since 1900
		tm.tm_mon = 0;
		tm.tm_mday = 0;

		// Convert to time_t (UTC)
		std::time_t time = _mkgmtime(&tm); // use _mkgmtime for UTC; mktime for local time

		// Convert to chrono time_point
		using namespace std::chrono;
		auto sysTime = system_clock::from_time_t(time);

		// Convert to Windows clock (1601 epoch)
		auto windowsEpoch = sysTime.time_since_epoch() + 11644473600s;
		auto dt = winrt::Windows::Foundation::DateTime{ duration_cast<winrt::Windows::Foundation::TimeSpan>(windowsEpoch) };
		return dt;
	}

	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{
			auto e = X.as<DatePicker>();
			for (auto& p : properties)
			{
				if (p->n == L"Date")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						// convert YYYY-MM-DD to time_point
						std::wstring dateStr = op->value;
						if (dateStr.length() == 10) // YYYY-MM-DD
						{
							int Year = _wtoi(dateStr.substr(0, 4).c_str());
							int Month = _wtoi(dateStr.substr(5, 2).c_str());
							int Day = _wtoi(dateStr.substr(8, 2).c_str());

							if (Year >= 1970)
							{
								std::tm tm = {};
								tm.tm_year = Year - 1900; // years since 1900
								tm.tm_mon = Month - 1;    // months since January [0,11]
								tm.tm_mday = Day;

								// Convert to time_t (UTC)
								std::time_t time = _mkgmtime(&tm); // use _mkgmtime for UTC; mktime for local time

								// Convert to chrono time_point
								using namespace std::chrono;
								auto sysTime = system_clock::from_time_t(time);

								// Convert to Windows clock (1601 epoch)
								auto windowsEpoch = sysTime.time_since_epoch() + 11644473600s;
								auto dt = winrt::Windows::Foundation::DateTime{ duration_cast<winrt::Windows::Foundation::TimeSpan>(windowsEpoch) };
								e.Date(dt);
							}
						}
					}
				}
				if (p->n == L"DayVisible")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.DayVisible(op->SelectedIndex);
					}
				}
				if (p->n == L"MonthVisible")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.MonthVisible(op->SelectedIndex);
					}
				}
				if (p->n == L"YearVisible")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.YearVisible(op->SelectedIndex);
					}
				}
				if (p->n == L"Header")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						e.Header(winrt::box_value(op->value));
					}
				}
				if (p->n == L"MaxYear")
				{
					auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (op && !std::isnan(op->value))
					{
						int Year = (int)op->value;
						if (Year >= 1970)
						{
							e.MaxYear(DateFromYear(Year));
						}
					}
				}
				if (p->n == L"MinYear")
				{
					auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (op && !std::isnan(op->value))
					{
						int Year = (int)op->value;
						if (Year >= 1970)
						{
							e.MinYear(DateFromYear(Year));
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
		auto e = X.as<DatePicker>();

		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"DatePicker";
			op->S = 1;
			op->n = L"Date";
			try
			{
				auto cont = e.Date();
				// convert this to YYYY-MM-DD format
				auto ct = cont.time_since_epoch().count();
				// ct is duration in seconds since epoch
				int Year = (int)(ct / (365 * 24 * 60 * 60)) + 1970; // rough estimate
				int Month = (int)((ct % (365 * 24 * 60 * 60)) / (30 * 24 * 60 * 60)) + 1; // rough estimate
				int Day = (int)((ct % (30 * 24 * 60 * 60)) / (24 * 60 * 60)) + 1; // rough estimate
				wchar_t txt[100] = {};
				swprintf(txt, 100,L"%04d-%02d-%02d", Year, Month, Day);
				op->value = txt;

			}
			catch (...)
			{
			}
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->S = 1;
			op->g = L"DatePicker";
			op->n = L"DayVisible";
			op->DefaultIndex = 1;
			op->SelectedIndex = e.DayVisible();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"DatePicker";
			op->n = L"MonthVisible";
			op->DefaultIndex = 1;
			op->SelectedIndex = e.MonthVisible();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"DatePicker";
			op->n = L"YearVisible";
			op->DefaultIndex = 1;
			op->SelectedIndex = e.YearVisible();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"DatePicker";
			op->n = L"Header";
			try
			{
				op->value = winrt::unbox_value<winrt::hstring>(e.Header());
			}
			catch (...)
			{

			}
			properties.push_back(op);
		}
		if (1)
			{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->g = L"DatePicker";
			op->n = L"MinYear";
			op->mmin = 1970;
			op->mmax = 3000;
			op->value = std::nan("");
			try
			{
				auto maxYear = e.MinYear();
				if (maxYear.time_since_epoch().count() > 0)
				{
					int Year = (int)(maxYear.time_since_epoch().count() / (365 * 24 * 60 * 60)) + 1970; // rough estimate
					op->value = Year;
				}
			}
			catch (...)
			{
			}
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->g = L"DatePicker";
			op->n = L"MaxYear";
			op->mmin = 1970;
			op->mmax = 3000;
			op->value = std::nan("");
			try
			{
				auto maxYear = e.MaxYear();
				if (maxYear.time_since_epoch().count() > 0)
				{
					int Year = (int)(maxYear.time_since_epoch().count() / (365 * 24 * 60 * 60)) + 1970; // rough estimate
					op->value = Year;
				}
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

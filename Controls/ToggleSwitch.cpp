#include "pch.h"
#include "property.hpp"

class ITEM_ToggleSwitch : public XITEM_Control
{
public:

	ITEM_ToggleSwitch()
	{
		ElementName = L"ToggleSwitch";
		X = ToggleSwitch();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{

			auto e = X.as<ToggleSwitch>();
			for (auto& p : properties)
			{
				if (p->n == L"OffContent")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						e.OffContent(winrt::box_value(op->value));
					}
				}
				if (p->n == L"OnContent")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						e.OnContent(winrt::box_value(op->value));
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
				if (p->n == L"IsOn")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.IsOn(op->SelectedIndex != 0);
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
		auto e = X.as<ToggleSwitch>();

		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->S = 1;
			op->n = L"OffContent";
			op->g = L"ToggleSwitch";
			try
			{
				op->value = e.OffContent().as<winrt::hstring>().c_str();
			}
			catch (...)
			{

			}
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->n = L"OnContent";
			op->g = L"ToggleSwitch";
			try
			{
				op->value = e.OnContent().as<winrt::hstring>().c_str();
			}
			catch (...)
			{
			}
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"ToggleSwitch";
			op->n = L"Header";
			try
			{
				op->value = e.Header().as<winrt::hstring>().c_str();
			}
			catch (...)
			{
			}
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"ToggleSwitch";
			op->n = L"IsOn";
			op->DefaultIndex = 0;
			try
			{
				op->SelectedIndex = e.IsOn();
			}
			catch (...)
			{
			}
			properties.push_back(op);
		}


		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = ToggleSwitch();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<ToggleSwitch>();
		b.Tag(box_value((long long)this));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemToggleSwitch()
{
	return std::make_shared< ITEM_ToggleSwitch>();
}

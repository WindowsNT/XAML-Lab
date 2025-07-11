#include "pch.h"
#include "property.hpp"

using namespace winrt::Microsoft::UI::Xaml::Controls::Primitives;
void XITEM_ToggleButton::ApplyProperties()
{
	XITEM_ButtonBase::ApplyProperties();
	try
	{
		auto e = X.try_as<ToggleButton>();
		for (auto& p : properties)
		{
			if (p->n == L"IsChecked")
			{
				auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
				if (op)
				{
					e.IsChecked(op->SelectedIndex);
				}
			}
			if (p->n == L"IsThreeState")
			{
				auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
				if (op)
				{
					e.IsThreeState(op->SelectedIndex);
				}
			}
		}

	}
	catch (...)
	{

	}
}


std::vector<std::shared_ptr<PROPERTY>> XITEM_ToggleButton::CreateProperties(XML3::XMLElement* el) 
	{
	if (!properties.empty())
		return properties;
		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"ToggleButton";
			op->n = L"IsChecked";
			op->Items = { L"False", L"True" };
			op->DefaultIndex = 0;
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"ToggleButton";
			op->n = L"IsThreeState";
			op->Items = { L"False", L"True" };
			op->DefaultIndex = 0;
			properties.push_back(op);
		}

		auto p2 = XITEM_ButtonBase::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}


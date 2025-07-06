#include "pch.h"
#include "property.hpp"

using namespace winrt::Microsoft::UI::Xaml::Controls::Primitives;
class ITEM_TOGGLEBUTTON : public XITEM_ButtonBase
{
public:

	ITEM_TOGGLEBUTTON()
	{
		ElementName = L"ToggleButton";
		X = winrt::Microsoft::UI::Xaml::Controls::Primitives::ToggleButton();
	}


	virtual void ApplyProperties()
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

			ApplyPropertiesForContent(X.as<winrt::Microsoft::UI::Xaml::Controls::Primitives::ToggleButton>(), properties);
		}
		catch (...)
		{

		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		properties.clear();
		if (1)
		{
			auto uip = CreatePropertiesForContent(X.as<winrt::Microsoft::UI::Xaml::Controls::Primitives::ToggleButton>(), L"ToggleButton");
			for (auto& p : uip)
			{
				p->g = L"ToggleButton";
				properties.push_back(p);
			}
		}
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

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = winrt::Microsoft::UI::Xaml::Controls::Primitives::ToggleButton();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<winrt::Microsoft::UI::Xaml::Controls::Primitives::ToggleButton>();
		b.Tag(box_value((long long)this));
		b.Content(winrt::box_value(L"ToggleButton"));
		return b;
	}



};


std::shared_ptr<XITEM> CreateXItemToggleButton()
{
	return std::make_shared< ITEM_TOGGLEBUTTON>();
}

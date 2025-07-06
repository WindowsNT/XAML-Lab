#include "pch.h"
#include "property.hpp"

class ITEM_CHECKBOX : public XITEM_ButtonBase
{
public:

	ITEM_CHECKBOX()
	{
		ElementName = L"CheckBox";
		X = CheckBox();
	}


	virtual void ApplyProperties()
	{
		XITEM_ButtonBase::ApplyProperties();
		try
		{
			auto e = X.as<CheckBox>();
			for (auto& p : properties)
			{
				if (p->n == L"IsChecked")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.IsChecked(op->SelectedIndex);
					}
				}
				if (p->n == L"IsThreeState")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
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


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		el;
		auto e = X.as<CheckBox>();
		properties.clear();
		if (1)
		{
			auto uip = CreatePropertiesForContent(X.as<CheckBox>(), L"CheckBox");
			for (auto& p : uip)
			{
				p->g = L"CheckBox";
				properties.push_back(p);
			}
		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"CheckBox";
			op->n = L"IsChecked";
			op->DefaultIndex = 0;
			op->SelectedIndex = (e.IsChecked()) ? 1 : 0;
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"CheckBox";
			op->n = L"IsThreeState";
			op->DefaultIndex = 0;
			op->SelectedIndex = (int)e.IsThreeState();
			properties.push_back(op);
		}

		auto p2 = XITEM_ButtonBase::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = CheckBox();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<CheckBox>();
		b.Tag(box_value((long long)this));
		b.Content(winrt::box_value(L"CheckBox"));
		return b;
	}




};


std::shared_ptr<XITEM> CreateXItemCheckBox()
{
	return std::make_shared< ITEM_CHECKBOX>();
}

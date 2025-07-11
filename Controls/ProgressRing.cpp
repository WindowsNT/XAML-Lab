#include "pch.h"
#include "property.hpp"

class ITEM_ProgressRing : public XITEM_Control
{
public:

	ITEM_ProgressRing()
	{
		ElementName = L"ProgressRing";
		X = ProgressRing();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{

			auto e = X.as<ProgressRing>();
			for (auto& p : properties)
			{
				if (p->n == L"IsActive")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.IsActive(op->SelectedIndex);
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
		auto e = X.as<ProgressRing>();
		if (!properties.empty())
			return properties;

		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->S = 1;
			op->g = L"ProgressRing";
			op->n = L"IsActive";
			op->DefaultIndex = 1;
			properties.push_back(op);

		}


		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = ProgressRing();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<ProgressRing>();
		b.Tag(box_value((long long)this));
		b.IsActive(true); // Default to active
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemProgressRing()
{
	return std::make_shared< ITEM_ProgressRing>();
}

#include "pch.h"
#include "property.hpp"

class ITEM_Slider : public XITEM_Control
{
public:

	ITEM_Slider()
	{
		ElementName = L"Slider";
		X = Slider();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{

			auto e = X.as<Slider>();
			for (auto& p : properties)
			{
				if (p->n == L"Minimum")
				{
					auto pp = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (pp)
					{
						e.Minimum(pp->value);
					}
				}
				else if (p->n == L"Maximum")
				{
					auto pp = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (pp)
					{
						e.Maximum(pp->value);
					}
				}
				else if (p->n == L"StepFrequency")
				{
					auto pp = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (pp)
					{
						e.StepFrequency(pp->value);
					}
				}
				else if (p->n == L"SmallChange")
				{
					auto pp = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (pp)
					{
						e.SmallChange(pp->value);
					}
				}
				else if (p->n == L"TickFrequency")
				{
					auto pp = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (pp)
					{
						e.TickFrequency(pp->value);
					}
				}
				else if (p->n == L"TickPlacement")
				{
					auto pp = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
					if (pp)
					{
						e.TickPlacement((winrt::Microsoft::UI::Xaml::Controls::Primitives::TickPlacement)pp->SelectedIndex);
					}
				}
				else if (p->n == L"SnapsTo")
				{
					auto pp = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
					if (pp)
					{
						e.SnapsTo((winrt::Microsoft::UI::Xaml::Controls::Primitives::SliderSnapsTo)pp->SelectedIndex);
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
		auto e = X.as<Slider>();
		if (!properties.empty())
			return properties;
		if (1)
		{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->g = L"Slider";
			op->S = 1;
			op->n = L"Minimum";
			op->mmin = 1;
			op->mmax = 100;
			op->smallchange = 1;
			op->largechange = 10;
			op->value = e.Minimum();
			op->def = 5;
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->g = L"Slider";
			op->n = L"Maximum";
			op->mmin = 1;
			op->mmax = 100;
			op->smallchange = 1;
			op->largechange = 10;
			op->value = e.Maximum();
			op->def = 5;
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->g = L"Slider";
			op->n = L"StepFrequency";
			op->mmin = 1;
			op->mmax = 100;
			op->smallchange = 1;
			op->largechange = 10;
			op->value = e.StepFrequency();
			op->def = 5;
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->g = L"Slider";
			op->n = L"SmallChange";
			op->mmin = 1;
			op->mmax = 100;
			op->smallchange = 1;
			op->largechange = 10;
			op->value = e.SmallChange();
			op->def = 5;
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->g = L"Slider";
			op->n = L"TickFrequency";
			op->mmin = 1;
			op->mmax = 100;
			op->smallchange = 1;
			op->largechange = 10;
			op->value = e.TickFrequency();
			op->def = 5;
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"Slider";
			op->n = L"TickPlacement";
			op->Items = { L"None",L"TopLeft",L"BottomRight",L"Outside",L"Inside" };
			op->DefaultIndex = 0;
			op->SelectedIndex = (int)e.TickPlacement();
			properties.push_back(op);
		}
		if (1)
		{ 
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"Slider";
			op->n = L"SnapsTo";
			op->Items = { L"StepValues",L"Ticks"};
			op->DefaultIndex = 0;
			op->SelectedIndex = (int)e.SnapsTo();
			properties.push_back(op);

		}


		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = Slider();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<Slider>();
		b.Tag(box_value((long long)this));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemSlider()
{
	return std::make_shared< ITEM_Slider>();
}

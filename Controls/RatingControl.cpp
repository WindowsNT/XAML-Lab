#include "pch.h"
#include "property.hpp"

class ITEM_RATINGCONTROL : public XITEM_Control
{
public:

	ITEM_RATINGCONTROL()
	{
		ElementName = L"RatingControl";
		X = RatingControl();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{

			auto e = X.as<RatingControl>();
			for (auto& p : properties)
			{
				if (p->n == L"MaxRating")
				{
					auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (op)
					{
						e.MaxRating((int)op->value);
					}
				}
				if (p->n == L"Value")
				{
					auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (op)
					{
						e.Value(op->value);
					}
				}
				if (p->n == L"IsClearEnabled")
				{
					auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
					if (op)
					{
						e.IsClearEnabled(op->SelectedIndex);
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
		auto e = X.as<RatingControl>();
		if (!properties.empty())
			return properties;
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"RatingControl";
			op->S = 1;
			op->n = L"IsClearEnabled";
			op->DefaultIndex = 0;
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->g = L"RatingControl";
			op->n = L"MaxRating";
			op->mmin = 1;
			op->mmax = 100;
			op->smallchange = 1;
			op->largechange = 2;
			op->value = e.MaxRating();
			op->def = 5;
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->g = L"RatingControl";
			op->n = L"Value";
			op->mmin = 1;
			op->mmax = 100;
			op->smallchange = 0.5;
			op->largechange = 0.5;
			op->value = e.Value();
			op->def = e.Value();
			properties.push_back(op);
		}

		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = RatingControl();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<RatingControl>();
		b.Tag(box_value((long long)this));
		b.IsReadOnly(false);
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemRatingControl()
{
	return std::make_shared< ITEM_RATINGCONTROL>();
}

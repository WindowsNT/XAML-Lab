#include "pch.h"
#include "property.hpp"

class ITEM_ProgressBar : public XITEM_Control
{
public:

	ITEM_ProgressBar()
	{
		ElementName = L"ProgressBar";
		X = ProgressBar();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{
			auto e = X.as<ProgressBar>();
			for (auto& p : properties)
			{
				if (p->n == L"ShowPaused")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.ShowPaused(op->SelectedIndex);
					}
				}
				if (p->n == L"ShowError")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.ShowError(op->SelectedIndex);
					}
				}
				if (p->n == L"IsIndeterminate")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.IsIndeterminate(op->SelectedIndex);
					}
				}
				if (p->n == L"Minimum")
				{
					auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (op)
					{
						e.Minimum(op->value);
					}
				}
				if (p->n == L"Maximum")
				{
					auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (op)
					{
						e.Maximum(op->value);
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
			}
		}
		catch (...)
		{

		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		el;
		auto e = X.as<ProgressBar>();
		if (!properties.empty())
			return properties;

		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->S = 1;
			op->g = L"ProgressBar";
			op->n = L"IsIndeterminate";
			op->DefaultIndex = 0;
			properties.push_back(op);

		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"ProgressBar";
			op->n = L"ShowPaused";
			op->DefaultIndex = 0;
			properties.push_back(op);

		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"ProgressBar";
			op->n = L"ShowError";
			op->DefaultIndex = 0;
			properties.push_back(op);

		}

		if (1)
		{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->g = L"ProgressBar";
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
			op->g = L"ProgressBar";
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
			op->g = L"ProgressBar";
			op->n = L"Value";
			op->mmin = 0;
			op->mmax = 100;
			op->smallchange = 1;
			op->largechange = 10;
			op->value = e.Value();
			op->def = 5;
			properties.push_back(op);
		}


		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = ProgressBar();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<ProgressBar>();
		b.Maximum(100);
		b.Minimum(0);
		b.Tag(box_value((long long)this));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemProgressBar()
{
	return std::make_shared< ITEM_ProgressBar>();
}

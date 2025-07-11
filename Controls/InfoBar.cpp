#include "pch.h"
#include "property.hpp"

class ITEM_InfoBar : public XITEM_Control
{
public:

	ITEM_InfoBar()
	{
		ElementName = L"InfoBar";
		X = InfoBar();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{

			auto e = X.as<InfoBar>();
			for (auto& p : properties)
			{
				if (p->n == L"IsOpen")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.IsOpen(op->SelectedIndex);
					}
				}
				if (p->n == L"Title")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						e.Title(op->value);
					}
				}
				if (p->n == L"Message")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						e.Message(op->value);
					}
				}
				if (p->n == L"Severity")
				{
					auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
					if (op)
					{
						e.Severity((InfoBarSeverity)op->SelectedIndex);
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
		auto e = X.as<InfoBar>();
		if (!properties.empty())
			return properties;

		if (1)
		{
			// IsOpen
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->S = 1;
			op->g = L"InfoBar";
			op->n = L"IsOpen";
			op->DefaultIndex = 1;
			op->SelectedIndex = 1;
			properties.push_back(op);
		}
		if (1)
		{
			// Title
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"InfoBar";
			op->n = L"Title";
			op->value = e.Title();
			op->def = ttitle;
			properties.push_back(op);
		}
		if (1)
		{
			// Message
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"InfoBar";
			op->n = L"Message";
			op->value = e.Message();
			op->def = s(29);
			properties.push_back(op);
		}
		if (1)
		{
			// Severity
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"InfoBar";
			op->n = L"Severity";
			op->Items.push_back(L"Informational");
			op->Items.push_back(L"Success");
			op->Items.push_back(L"Warning");
			op->Items.push_back(L"Error");
			op->SelectedIndex = (int)e.Severity();
			op->DefaultIndex = 0;
			properties.push_back(op);
		}

		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = InfoBar();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<InfoBar>();
		b.IsOpen(true);
		b.Title(ttitle);
		b.Message(s(29));
		b.Tag(box_value((long long)this));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemInfoBar()
{
	return std::make_shared< ITEM_InfoBar>();
}

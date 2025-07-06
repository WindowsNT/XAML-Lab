#include "pch.h"
#include "property.hpp"

void XITEM_FrameworkElement::ApplyProperties()
{
	XITEM_UIElement::ApplyProperties();
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::FrameworkElement>();
	if (!e)
		return;
	for (auto& p : properties)
	{
		if (IsSomeParentGrid(this))
		{
			if (p->n == L"Grid.Column")
			{
				auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
				if (op)
				{
					Grid::SetColumn(e, (int)op->value);
				}
			}
			if (p->n == L"Grid.Row")
			{
				auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
				if (op)
				{
					Grid::SetRow(e, (int)op->value);
				}
			}
		}
		if (p->n == L"Margin")
		{
			auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
			if (op)
			{
				try
				{
					// get comma separated values from value
					std::vector<std::wstring> values;
					std::wstring value = op->value;
					size_t pos = 0;
					while ((pos = value.find(L',')) != std::wstring::npos)
					{
						values.push_back(value.substr(0, pos));
						value.erase(0, pos + 1);
					}
					if (value.length())
						values.push_back(value);
					if (values.size() == 4)
					{
						double left = std::stod(values[0]);
						double top = std::stod(values[1]);
						double right = std::stod(values[2]);
						double bottom = std::stod(values[3]);
						Microsoft::UI::Xaml::Thickness th = { left, top, right, bottom };
						e.Margin(th);
					}
					else
					{
					}
				}
				catch (...)
				{

				}
			}
		}
		if (p->n == L"Name")
		{
			auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
			if (op && op->value.length())
			{
				e.Name(op->value);
			}
		}
		if (p->n == L"MinWidth")
		{
			auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
			if (op && !std::isnan(op->value))
			{
				e.MinWidth(op->value);
			}
		}
		if (p->n == L"MinHeight")
		{
			auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
			if (op && !std::isnan(op->value))
			{
				e.MinHeight(op->value);
			}
		}
		if (p->n == L"MaxWidth")
		{
			auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
			if (op && !std::isnan(op->value))
			{
				e.MaxWidth(op->value);
			}
		}
		if (p->n == L"MaxHeight")
		{
			auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
			if (op && !std::isnan(op->value))
			{
				e.MaxHeight(op->value);
			}
		}
		if (p->n == L"HorizontalAligment")
		{
			auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
			if (op)
			{
				e.HorizontalAlignment((winrt::Microsoft::UI::Xaml::HorizontalAlignment)op->SelectedIndex);
			}
		}
		if (p->n == L"VerticalAligment")
		{
			auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
			if (op)
			{
				e.VerticalAlignment((winrt::Microsoft::UI::Xaml::VerticalAlignment)op->SelectedIndex);
			}
		}
	}
}


std::vector<std::shared_ptr<PROPERTY>> XITEM_FrameworkElement::CreateProperties(XML3::XMLElement* el)
{
	el;
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::FrameworkElement>();
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;	
	if (1)
	{
		std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
		op->g = L"FrameworkElement";
		op->n = L"Margin";
		op->value = e.Name();
		op->def = L"";
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->g = L"FrameworkElement";
		op->n = L"MaxHeight";
		op->mmin = 0.0;
		op->mmax = 10000;
		op->value = std::nan("");
		op->def = std::nan("");
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->g = L"FrameworkElement";
		op->n = L"MaxWidth";
		op->mmin = 0.0;
		op->mmax = 10000;
		op->value = std::nan("");
		op->def = std::nan("");
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->g = L"FrameworkElement";
		op->n = L"MinHeight";
		op->mmin = 0.0;
		op->mmax = 10000;
		op->value = std::nan("");
		op->def = std::nan("");
		if (X.try_as<winrt::Microsoft::UI::Xaml::Controls::WebView2>())
		{
			op->value = 200.0;
			op->def = 200.0;
		}
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->g = L"FrameworkElement";
		op->n = L"MinWidth";
		op->mmin = 0.0;
		op->mmax = 10000;
		op->value = std::nan("");
		op->def = std::nan("");
		if (X.try_as<winrt::Microsoft::UI::Xaml::Controls::WebView2>())
		{
			op->value = 200.0;
			op->def = 200.0;
		}
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
		op->g = L"FrameworkElement";
		op->n = L"Name";
		op->value = e.Name();
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
		op->g = L"FrameworkElement";
		op->n = L"HorizontalAligment";
		op->Items = { L"Left", L"Center", L"Right", L"Stretch" };
		op->DefaultIndex = 3;
		op->SelectedIndex = (size_t)e.HorizontalAlignment();	
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
		op->g = L"FrameworkElement";
		op->n = L"VerticalAligment";
		op->Items = { L"Top",L"Center",L"Bottom",L"Stretch"};
		op->DefaultIndex = 3;
		op->SelectedIndex = (size_t)e.VerticalAlignment();
		p.push_back(op);
	}

	// Grid
	auto uip = CreatePropertiesForParentGrid(X);
	for (auto& p1 : uip)
	{
		p1->g = L"Parent Grid";
		properties.push_back(p1);
	}



	// Add properties from UIElement
	auto p2 = XITEM_UIElement::CreateProperties(el);
	for (auto& pp : p2)
		p.push_back(pp);
	return p;
}

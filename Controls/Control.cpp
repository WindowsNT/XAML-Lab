#include "pch.h"
#include "property.hpp"


void XITEM_Control::ApplyProperties()
{
	XITEM_FrameworkElement::ApplyProperties();
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Control>();
	if (!e)
		return;
	try
	{
		ApplyPropertiesForFont< winrt::Microsoft::UI::Xaml::Controls::Control>(e, properties);
		ApplyPropertiesForBackground< winrt::Microsoft::UI::Xaml::Controls::Control>(e, properties);
		ApplyPropertiesForForeground< winrt::Microsoft::UI::Xaml::Controls::Control>(e, properties);
		for (auto& p : properties)
		{
			if (p->n == L"IsEnabled")
			{
				auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
				if (op)
				{
					e.IsEnabled((bool)op->SelectedIndex);
				}
			}
		}
	}
	catch (...)
	{

	}

}


void XITEM_Control::Select()
{
	auto b = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Control>();
	if (!b)
		return;
	try
	{
		b.BorderBrush(SolidColorBrush(Colors::Red()));
		b.BorderThickness(ThicknessHelper::FromLengths(2.0, 2.0, 2.0, 2.0));
	}
	catch(...)
	{
	}
}

void XITEM_Control::Unselect()
{
	auto b = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Control>();
	if (!b)
		return;
	try
	{
		b.BorderBrush(nullptr);
		b.BorderThickness(ThicknessHelper::FromLengths(0, 0, 0, 0));
	}
	catch (...)
	{
		
	}
}

std::vector<std::shared_ptr<PROPERTY>> XITEM_Control::CreateProperties(XML3::XMLElement* el)
{
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Control>();
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;

	auto bc = X.try_as<winrt::Microsoft::UI::Xaml::Controls::BreadcrumbBar>();


	if (!bc)
	{
		auto p1 = CreatePropertiesForFont< winrt::Microsoft::UI::Xaml::Controls::Control>(e);
		for (auto& pp : p1)
			p.push_back(pp);
	}

	if (!bc)
	{
		auto p1 = CreatePropertiesForBackground< winrt::Microsoft::UI::Xaml::Controls::Control>(e);
		for (auto& pp : p1)
		{
			pp->g = L"Control";
			p.push_back(pp);
		}
	}
	if (!bc)
	{
		auto p1 = CreatePropertiesForForeground< winrt::Microsoft::UI::Xaml::Controls::Control>(e);
		for (auto& pp : p1)
		{
			pp->g = L"Control";
			p.push_back(pp);
		}
	}	

	if (1)
	{
		std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
		op->g = L"Control";
		op->n = L"IsEnabled";
		op->Items = { L"False", L"True" };
		op->DefaultIndex = 1;
		op->SelectedIndex = 1;
		p.push_back(op);

	}


	// Add properties from FrameworkElement
	auto p2 = XITEM_FrameworkElement::CreateProperties(el);
	for (auto& pp : p2)
		p.push_back(pp);

	return p;
}

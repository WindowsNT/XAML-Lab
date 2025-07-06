#include "pch.h"
#include "property.hpp"

void XITEM_UIElement::ApplyProperties()
{
	XITEM::ApplyProperties();
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::UIElement>();
	if (!e)
		return;
	for (auto& p : properties)
	{
		if (p->n == L"Opacity")
		{
			auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
			if (op)
			{
				e.Opacity(op->value);
			}
		}
		if (p->n == L"Rotation")
		{
			// Exclude if RatingControl
			if (e.try_as<winrt::Microsoft::UI::Xaml::Controls::RatingControl>())
				continue;
			auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
			if (op)
			{
				e.Rotation((float)op->value);
			}
		}
	}
}



std::vector<std::shared_ptr<PROPERTY>> XITEM_UIElement::CreateProperties(XML3::XMLElement* el)
{
	el;
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::UIElement>();
	if (!e)
		return {};
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->g = L"UIElement";
		op->n = L"Opacity";
		op->mmin = 0.0;
		op->mmax = 1.0;
		op->smallchange = 0.1;
		op->largechange = 0.1;
		op->value = e.Opacity();
		op->def = 1.0;
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->g = L"UIElement";
		op->n = L"Rotation";
		op->mmin = 0.0;
		op->mmax = 360.0;
		op->smallchange = 1;
		op->largechange = 10;
		op->value = e.Rotation();
		op->def = 0.0;
		p.push_back(op);
	}


	return p;
}

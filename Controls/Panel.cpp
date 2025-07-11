#include "pch.h"
#include "property.hpp"

void XITEM_Panel::ApplyProperties()
{
	XITEM_FrameworkElement::ApplyProperties();
	auto e = X.try_as<Panel>();
	if (!e)
		return;
	ApplyPropertiesForBackground<Panel>(e, properties);
	for (auto& p : properties)
	{
		p;
	}
}


std::vector<std::shared_ptr<PROPERTY>> XITEM_Panel::CreateProperties(XML3::XMLElement* el)
{
	auto e = X.try_as<Panel>();
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;

	if (1)
	{
		auto p1 = CreatePropertiesForBackground<Panel>(e);
		for (auto& pp : p1)
		{
			pp->g = L"Panel";
			p.push_back(pp);
		}
	}

	auto p2 = XITEM_FrameworkElement::CreateProperties(el);
	for (auto& pp : p2)
		p.push_back(pp);
	return p;
}


void XITEM_Panel::Select()
{
	auto e = X.try_as<Panel>();
	if (!e)
		return;
	e.Background(SolidColorBrush(Colors::Red()));
}

void XITEM_Panel::Unselect()
{
	auto e = X.try_as<Panel>();
	if (!e)
		return;
	e.Background(SolidColorBrush(Colors::Transparent()));
}



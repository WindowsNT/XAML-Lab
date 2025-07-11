#include "pch.h"
#include "property.hpp"


void XITEM_Selector::ApplyProperties()
{
	XITEM_ItemsControl::ApplyProperties();
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Primitives::Selector>();
	if (!e)
		return;
	try
	{
	}
	catch (...)
	{

	}

}


std::vector<std::shared_ptr<PROPERTY>> XITEM_Selector::CreateProperties(XML3::XMLElement* el)
{
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Primitives::Selector>();
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;
	auto p2 = XITEM_ItemsControl::CreateProperties(el);
	for (auto& pp : p2)
		p.push_back(pp);

	return p;
}

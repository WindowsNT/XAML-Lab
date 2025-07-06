#include "pch.h"
#include "property.hpp"


void XITEM_ListViewBase::ApplyProperties()
{
	XITEM_Selector::ApplyProperties();
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::ListViewBase>();
	if (!e)
		return;
}



std::vector<std::shared_ptr<PROPERTY>> XITEM_ListViewBase::CreateProperties(XML3::XMLElement* el)
{
	el;
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::ListViewBase>();
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;

	// Add properties from Selector
	auto p2 = XITEM_Selector::CreateProperties(el);
	for (auto& pp : p2)
		p.push_back(pp);

	return p;
}

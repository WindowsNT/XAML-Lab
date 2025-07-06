#include "pch.h"
#include "property.hpp"


void XITEM_ButtonBase::ApplyProperties()
{
	XITEM_ContentControl::ApplyProperties();
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Primitives::ButtonBase>();
	if (!e)
		return;
}



std::vector<std::shared_ptr<PROPERTY>> XITEM_ButtonBase::CreateProperties(XML3::XMLElement* el)
{
	el;
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Primitives::ButtonBase>();
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;

	// Add properties from ContentControl
	auto p2 = XITEM_ContentControl::CreateProperties(el);
	for (auto& pp : p2)
		p.push_back(pp);

	return p;
}

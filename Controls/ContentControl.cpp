#include "pch.h"
#include "property.hpp"


void XITEM_ContentControl::ApplyProperties()
{
	XITEM_Control::ApplyProperties();
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Control>();
	if (!e)
		return;
}



std::vector<std::shared_ptr<PROPERTY>> XITEM_ContentControl::CreateProperties(XML3::XMLElement* el)
{
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::ContentControl>();
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;

	// Add properties from Control
	auto p2 = XITEM_Control::CreateProperties(el);
	for (auto& pp : p2)
		p.push_back(pp);

	return p;
}

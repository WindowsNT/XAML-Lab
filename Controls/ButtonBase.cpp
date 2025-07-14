#include "pch.h"
#include "property.hpp"


void XITEM_ButtonBase::ApplyProperties()
{
	XITEM_ContentControl::ApplyProperties();
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Primitives::ButtonBase>();
	if (!e)
		return;

	for(auto& p : properties)
	{
		if (p->n == L"Click")
		{
			auto op = std::dynamic_pointer_cast<FUNCTION_PROPERTY>(p);
			if (op)
			{
				if (op->value.empty())
					continue;

				CallbackFunctions.push_back(op->f);
			}
		}
	}
}



std::vector<std::shared_ptr<PROPERTY>> XITEM_ButtonBase::CreateProperties(XML3::XMLElement* el)
{
	el;
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Primitives::ButtonBase>();
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;

	if (1)
	{
		auto p1 = CreatePropertyForGenericCallback(e, L"Click");
		if (p1)
		{
			p1->g = L"Events.ButtonBase";
			auto op = std::dynamic_pointer_cast<FUNCTION_PROPERTY>(p1);
			if (op)
			{
				op->f.content = s(78);
			}
			p.push_back(p1);
		}
	}

	// Add properties from ContentControl
	auto p2 = XITEM_ContentControl::CreateProperties(el);
	for (auto& pp : p2)
		p.push_back(pp);

	return p;
}

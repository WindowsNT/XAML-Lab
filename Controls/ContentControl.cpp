#include "pch.h"
#include "property.hpp"


void XITEM_ContentControl::ApplyProperties()
{
	XITEM_Control::ApplyProperties();
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::ContentControl>();
	if (!e)
		return;

	for (auto& p : properties)
	{
		if (p->n == L"Content")
		{
			if (X.try_as<winrt::Microsoft::UI::Xaml::Controls::CommandBar>())
				continue;
			if (X.try_as<winrt::Microsoft::UI::Xaml::Controls::NavigationView>())
				continue;
			if (X.try_as<winrt::Microsoft::UI::Xaml::Controls::AppBarButton>())
				continue;
			auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
			if (op)
			{
				e.Content(winrt::box_value(op->value));
			}
		}
	}

}



std::vector<std::shared_ptr<PROPERTY>> XITEM_ContentControl::CreateProperties(XML3::XMLElement* el)
{
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::ContentControl>();
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;

	if (1)
	{
		auto b1 = X.try_as<CommandBar>();
		auto b2 = X.try_as<AppBarButton>();
		auto b3 = X.try_as<NavigationView>();
		if (!b1 && !b2 && !b3)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"ContentControl";
			op->n = L"Content";
			op->value = L"Content";
			try
			{
				auto cont = e.Content();
				if (cont)
					op->value = winrt::unbox_value<winrt::hstring>(cont);
				p.push_back(op);
			}
			catch (...)
			{
			}
		}
	}

	// Add properties from Control
	auto p2 = XITEM_Control::CreateProperties(el);
	for (auto& pp : p2)
		p.push_back(pp);

	return p;
}

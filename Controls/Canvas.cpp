#include "pch.h"
#include "property.hpp"

using namespace winrt::Microsoft::UI::Xaml::Controls;
class ITEM_Canvas : public XITEM_Panel
{
public:


	ITEM_Canvas()
	{
		ElementName = L"Canvas";
		X = winrt::Microsoft::UI::Xaml::Controls::Canvas();
	}


	virtual void ApplyProperties()
	{
		XITEM_Panel::ApplyProperties();
		auto e = X.try_as<Canvas>();
		if (!e)
			return;
		for (auto& p : properties)
		{
			p;
		}
	}

	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;
		auto p2 = XITEM_Panel::CreateProperties(el);
		for (auto& p : p2)
			properties.push_back(p);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = Canvas();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.try_as<Canvas>();
		b.Tag(box_value((long long)this));
		if (ForWhat == 0)
			b.Background(SolidColorBrush(Colors::Transparent()));
		return b;
	}
};

std::shared_ptr<XITEM> CreateXItemCanvas()
{
	return std::make_shared< ITEM_Canvas>();
}

bool ISXItemCanvas(XITEM* xit)
{
	if (!xit)
		return false;
	return dynamic_cast<ITEM_Canvas*>(xit) != nullptr;
}

#include "pch.h"
#include "property.hpp"

using namespace winrt::Microsoft::UI::Xaml::Controls;
class ITEM_SwapChainPanel : public XITEM_Panel
{
public:


	ITEM_SwapChainPanel()
	{
		ElementName = L"SwapChainPanel";
		X = winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel();
	}


	virtual void ApplyProperties()
	{
		XITEM_Panel::ApplyProperties();
		auto e = X.try_as<SwapChainPanel>();
		if (!e)
			return;
		e.BorderThickness({ 0, 0, 0, 0 });
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
		X = SwapChainPanel();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.try_as<SwapChainPanel>();
		b.Tag(box_value((long long)this));
		return b;
	}
	void Select()
	{
		auto b = X.try_as<SwapChainPanel>();
		if (!b)
			return;
		b.BorderBrush(SolidColorBrush(Colors::Red()));
		b.BorderThickness(ThicknessHelper::FromLengths(2.0, 2.0, 2.0, 2.0));
	}
	void Unselect()
	{
		auto b = X.try_as<SwapChainPanel>();
		if (!b)
			return;
		b.BorderBrush(nullptr);
		b.BorderThickness(ThicknessHelper::FromLengths(0, 0, 0, 0));

	}

};

std::shared_ptr<XITEM> CreateXItemSwapChainPanel()
{
	return std::make_shared< ITEM_SwapChainPanel>();
}

bool ISXItemSwapChainPanel(XITEM* xit)
{
	if (!xit)
		return false;
	return dynamic_cast<ITEM_SwapChainPanel*>(xit) != nullptr;
}

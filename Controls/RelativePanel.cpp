#include "pch.h"
#include "property.hpp"

using namespace winrt::Microsoft::UI::Xaml::Controls;
class ITEM_RelativePanel : public XITEM_Panel
{
public:


	ITEM_RelativePanel()
	{
		ElementName = L"RelativePanel";
		X = winrt::Microsoft::UI::Xaml::Controls::RelativePanel();
	}


	virtual void ApplyProperties()
	{
		XITEM_Panel::ApplyProperties();
		auto e = X.try_as<RelativePanel>();
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
		X = RelativePanel();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.try_as<RelativePanel>();
		b.Tag(box_value((long long)this));
		if (ForWhat == 0)
			b.Background(SolidColorBrush(Colors::Transparent()));
		return b;
	}
	void Select()
	{
		auto b = X.try_as<RelativePanel>();
		if (!b)
			return;
		b.BorderBrush(SolidColorBrush(Colors::Red()));
		b.BorderThickness(ThicknessHelper::FromLengths(2.0, 2.0, 2.0, 2.0));
	}
	void Unselect()
	{
		auto b = X.try_as<RelativePanel>();
		if (!b)
			return;
		b.BorderBrush(nullptr);
		b.BorderThickness(ThicknessHelper::FromLengths(0, 0, 0, 0));

	}

};

std::shared_ptr<XITEM> CreateXItemRelativePanel()
{
	return std::make_shared< ITEM_RelativePanel>();
}

bool ISXItemRelativePanel(XITEM* xit)
{
	if (!xit)
		return false;
	return dynamic_cast<ITEM_RelativePanel*>(xit) != nullptr;
}

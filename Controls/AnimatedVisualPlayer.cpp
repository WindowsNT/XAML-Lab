#include "pch.h"
#include "property.hpp"

class ITEM_AnimatedVisualPlayer : public XITEM_Control
{
public:

	ITEM_AnimatedVisualPlayer()
	{
		ElementName = L"AnimatedVisualPlayer";
		X = AnimatedVisualPlayer();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{
			auto e = X.as<AnimatedVisualPlayer>();
			for (auto& p : properties)
			{
				p;

			}
		}
		catch (...)
		{

		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;
		auto e = X.as<AnimatedVisualPlayer>();
		if (!properties.empty())
			return properties;
		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = AnimatedVisualPlayer();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<AnimatedVisualPlayer>();
		b.MinWidth(200);
		b.MinHeight(200);
		b.Tag(box_value((long long)this));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemAnimatedVisualPlayer()
{
	return std::make_shared< ITEM_AnimatedVisualPlayer>();
}

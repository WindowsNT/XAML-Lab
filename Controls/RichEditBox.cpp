#include "pch.h"
#include "property.hpp"

class ITEM_RichEditBox : public XITEM_Control
{
public:
	ITEM_RichEditBox()
	{
		ElementName = L"RichEditBox";
		X = RichEditBox();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;
		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = RichEditBox();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}
		auto b = X.as<RichEditBox>();
		b.Tag(box_value((long long)this));
		return b;
	}




};


std::shared_ptr<XITEM> CreateXItemRichEditBox()
{
	return std::make_shared< ITEM_RichEditBox>();
}

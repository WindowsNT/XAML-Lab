#include "pch.h"
#include "property.hpp"
extern std::map<HWND, winrt::Windows::Foundation::IInspectable> windows;
void LoadMenuNames(std::vector<std::wstring>& names, XML3::XMLElement&);

class ITEM_AppBarButton : public XITEM_ButtonBase
{
public:

	ITEM_AppBarButton()
	{
		ElementName = L"AppBarButton";
		X = AppBarButton();
	}


	virtual void ApplyProperties()
	{
		XITEM_ButtonBase::ApplyProperties();
		try
		{
			auto e = X.as<AppBarButton>();
			ApplyPropertiesForIcon(e, properties);
			for (auto& p : properties)
			{
				
				if (p->n == L"Label")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						e.Label(op->value);
					}
				}
			}
		}
		catch (...)
		{

		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		el;
		auto e = X.as<AppBarButton>();
		if (!properties.empty())
			return properties;

		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->S = 1;
			op->g = L"AppBarButton";
			op->n = L"Label";
			try
			{
				op->value = e.Label();
			}
			catch (...)
			{
			}
			properties.push_back(op);
		}
		if (1)
		{
			auto uip = CreatePropertiesForIcon<AppBarButton>(X.as<AppBarButton>());
			for (auto& p : uip)
			{
				p->g = L"AppBarButton";
				properties.push_back(p);
			}



		}
/*		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"AppBarButton";
			op->n = L"Icon";
			try
			{
				auto icon = e.Icon();
				if (icon)
				{
					op->value = icon.Name();
				}
			}
			catch (...)
			{
			}
			properties.push_back(op);
		}*/
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"AppBarButton";
			op->n = L"Secondary";
			op->DefaultIndex = 0;
			properties.push_back(op);
		}


		auto p2 = XITEM_ButtonBase::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = AppBarButton();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<AppBarButton>();
		b.HorizontalAlignment(winrt::Microsoft::UI::Xaml::HorizontalAlignment::Left);
		b.VerticalAlignment(winrt::Microsoft::UI::Xaml::VerticalAlignment::Top);
		b.Label(L"AppBarButton");
		b.Tag(box_value((long long)this));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemAppBarButton()
{
	return std::make_shared< ITEM_AppBarButton>();
}

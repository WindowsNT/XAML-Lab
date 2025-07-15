#include "pch.h"
#include "property.hpp"

class ITEM_ListBox : public XITEM_Control
{
public:

	ITEM_ListBox()
	{
		ElementName = L"ListBox";
		X = ListBox();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{

			auto e = X.as<ListBox>();
			for (auto& p : properties)
			{
				if (p->n == L"SelectionMode")
				{
					auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
					if (op)
					{
						e.SelectionMode((winrt::Microsoft::UI::Xaml::Controls::SelectionMode)op->SelectedIndex);
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
		auto e = X.as<ListBox>();
		if (!properties.empty())
			return properties;

		
		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"ListBox";
			op->n = L"SelectionMode";
			op->Items = { L"Single", L"Multiple",L"Extended" };
			op->DefaultIndex = 1;
			op->SelectedIndex = (size_t)e.SelectionMode();
			properties.push_back(op);

		}

		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = ListBox();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		
		AddSomeSource();
		
		auto b = X.as<ListBox>();
		b.Tag(box_value((long long)this));
		return b;
	}

	void AddSomeSource()
	{
		auto b = X.as<ListBox>();
		using namespace winrt;
		using namespace Windows::Foundation;
		using namespace Windows::Foundation::Collections;
		using namespace Microsoft::UI::Xaml;
		using namespace Microsoft::UI::Xaml::Controls;
		using namespace Microsoft::UI::Xaml::Media;
		IObservableVector<IInspectable> pages = single_threaded_observable_vector<IInspectable>();
		for (int i = 0; i < 10; i++)
		{
			wchar_t txt[100] = {};
			swprintf_s(txt, L"Item %d", i + 1);
			TextBlock tb1;
			tb1.Text(txt);
			tb1.HorizontalAlignment(HorizontalAlignment::Center);
			tb1.VerticalAlignment(VerticalAlignment::Center);
			pages.Append(tb1);
		}
		b.ItemsSource(pages);


	}
};


std::shared_ptr<XITEM> CreateXItemListBox()
{
	return std::make_shared< ITEM_ListBox>();
}

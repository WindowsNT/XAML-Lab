#include "pch.h"
#include "property.hpp"

class ITEM_ListView : public XITEM_ListViewBase
{
public:

	ITEM_ListView()
	{
		ElementName = L"ListView";
		X = ListView();
	}


	virtual void ApplyProperties()
	{
		XITEM_ListViewBase::ApplyProperties();
		try
		{

			auto e = X.as<ListView>();
			e;
		}
		catch (...)
		{

		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		auto e = X.as<ListView>();
		if (!properties.empty())
			return properties;

		

		auto p2 = XITEM_ListViewBase::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = ListView();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		
		AddSomeSource();
		
		auto b = X.as<ListView>();
		b.Tag(box_value((long long)this));
		return b;
	}

	void AddSomeSource()
	{
		auto b = X.as<ListView>();
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
			tb1.FontSize(24);
			tb1.HorizontalAlignment(HorizontalAlignment::Center);
			tb1.VerticalAlignment(VerticalAlignment::Center);
			pages.Append(tb1);
		}
		b.ItemsSource(pages);


	}
};


std::shared_ptr<XITEM> CreateXItemListView()
{
	return std::make_shared< ITEM_ListView>();
}

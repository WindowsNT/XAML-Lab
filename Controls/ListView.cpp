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


		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = CreatePropertyItemsSource(e);
			op->g = L"ListView";
			op->BindingAnyway = 1;
			properties.push_back(op);
		}


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
			tb1.HorizontalAlignment(HorizontalAlignment::Center);
			tb1.VerticalAlignment(VerticalAlignment::Center);
			pages.Append(tb1);
		}
		b.ItemsSource(pages);


	}




	std::optional<std::wstring> GetCodeForProperty([[maybe_unused]] PROPERTY* p, [[maybe_unused]] int Type) override
	{
		if (!p)
			return {};

		if (p->n == L"ItemsSource")
		{
			std::vector<wchar_t> txt(100000);
			if (Type == 0) // IDL
				swprintf_s(txt.data(), 100000, L"Windows.Foundation.Collections.IObservableVector<Microsoft.UI.Xaml.FrameworkElement> %s{ get; };", p->bindv.c_str());
			if (Type == 1) // H
				swprintf_s(txt.data(), 100000, L"IObservableVector<FrameworkElement> %s();", p->bindv.c_str());
			if (Type == 2) // CPP
			{
				swprintf_s(txt.data(), 100000, LR"(IObservableVector<FrameworkElement> %s()
{
	auto items = single_threaded_observable_vector<FrameworkElement>();

	for (int i = 0; i < 10; i++)
		{
			wchar_t txt[100] = {};
			swprintf_s(txt, L"Item %%d", i + 1);
			TextBlock tb1;
			tb1.Text(txt);
			tb1.HorizontalAlignment(HorizontalAlignment::Center);
			tb1.VerticalAlignment(VerticalAlignment::Center);
			items.Append(tb1);
		}

	return items;
}
)", p->bindv.c_str());

			}
			return txt.data();
		}

		return {};
	}



};


std::shared_ptr<XITEM> CreateXItemListView()
{
	return std::make_shared< ITEM_ListView>();
}

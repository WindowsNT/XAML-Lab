#include "pch.h"
#include "property.hpp"
#include "paragraphcode.hpp"


class ITEM_Pivot : public XITEM_ItemsControl
{
public:

	ITEM_Pivot()
	{
		ElementName = L"Pivot";
		X = Pivot();
	}


	virtual void ApplyProperties()
	{
		XITEM_ItemsControl::ApplyProperties();
		try
		{

			auto e = X.as<Pivot>();
			for (auto& p : properties)
			{
				p;//
			}
		}
		catch (...)
		{

		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		auto e = X.as<Pivot>();
		if (!properties.empty())
			return properties;

		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = CreatePropertyItemsSource(e);
			if (op)
			{
				op->g = L"Pivot";
				op->BindingAnyway = 1;
				properties.push_back(op);
			}
		}


		auto p2 = XITEM_ItemsControl::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = Pivot();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}


		AddSomeSource();
		auto b = X.as<Pivot>();
		b.Tag(box_value((long long)this));
		return b;
	}



	std::optional<std::wstring> GetCodeForProperty([[maybe_unused]] PROPERTY* p, [[maybe_unused]] int Type) override
	{
		if (!p)
			return {};

		if (p->n == L"ItemsSource")
		{
			std::vector<wchar_t> txt(100000);
			if (Type == 0) // IDL
				swprintf_s(txt.data(), 100000, L"Windows.Foundation.Collections.IObservableVector<Microsoft.UI.Xaml.PivotItem> %s{ get; };", p->bindv.c_str());
			if (Type == 1) // H
				swprintf_s(txt.data(), 100000, L"IObservableVector<PivotItem> %s();", p->bindv.c_str());
			if (Type == 2) // CPP
			{
				swprintf_s(txt.data(), 100000, LR"(IObservableVector<PivotItem> %s()
{
	auto items = single_threaded_observable_vector<PivotItem>();


	for (int i = 0; i < 5; i++)
	{
		wchar_t txt[100] = {};
		swprintf_s(txt, L"Item %%d", i + 1);
		PivotItem tb1;
		tb1.Header(winrt::box_value(txt));	
		pages.Append(tb1);
	}

	return items;
}
)", p->bindv.c_str());

			}
			return txt.data();
		}

		return {};
	}



	void AddSomeSource()
	{
		auto b = X.as<Pivot>();
		// Create a vector of IInspectable UIElements
		using namespace winrt;
		using namespace Windows::Foundation;
		using namespace Windows::Foundation::Collections;
		using namespace Microsoft::UI::Xaml;
		using namespace Microsoft::UI::Xaml::Controls;
		using namespace Microsoft::UI::Xaml::Media;
		IObservableVector<PivotItem> pages = single_threaded_observable_vector<PivotItem>();
		for (int i = 0; i < 5; i++)
		{
			wchar_t txt[100] = {};
			swprintf_s(txt, L"Item %d", i + 1);
			PivotItem tb1;
			tb1.Header(winrt::box_value(txt));	
			pages.Append(tb1);
		}
		b.ItemsSource(pages);


	}
};


std::shared_ptr<XITEM> CreateXItemPivot()
{
	return std::make_shared< ITEM_Pivot>();
}

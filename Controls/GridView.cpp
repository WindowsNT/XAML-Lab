#include "pch.h"
#include "property.hpp"
#include "paragraphcode.hpp"

class ITEM_GridView : public XITEM_ListViewBase
{
public:

	ITEM_GridView()
	{
		ElementName = L"GridView";
		X = GridView();
	}


	virtual void ApplyProperties()
	{
		XITEM_ListViewBase::ApplyProperties();
		try
		{

			auto e = X.as<GridView>();
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
		auto e = X.as<GridView>();
		if (!properties.empty())
			return properties;


		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = CreatePropertyItemsSource(e);
			if (op)
			{
				op->g = L"GridView";
				op->BindingAnyway = 1;
				properties.push_back(op);
			}
		}

		auto p2 = XITEM_ListViewBase::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = GridView();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		
		AddSomeSource();
		
		auto b = X.as<GridView>();
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
				swprintf_s(txt.data(), 100000, L"Windows.Foundation.Collections.IObservableVector<Microsoft.UI.Xaml.FrameworkElement> %s{ get; };", p->bindv.c_str());
			if (Type == 1) // H
				swprintf_s(txt.data(), 100000, L"IObservableVector<FrameworkElement> %s();", p->bindv.c_str());
			if (Type == 2) // CPP
			{
				swprintf_s(txt.data(), 100000, LR"(IObservableVector<FrameworkElement> %s()
{
	auto items = single_threaded_observable_vector<FrameworkElement>();

	for (int i = 0; i < 4; i++)
	{
		wchar_t txt[100] = {};
		swprintf_s(txt, L"Page %%d", i + 1);
		TextBlock tb1;
		tb1.Text(txt);
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




	void AddSomeSource()
	{
		auto b = X.as<GridView>();
		using namespace winrt;
		using namespace Windows::Foundation;
		using namespace Windows::Foundation::Collections;
		using namespace Microsoft::UI::Xaml;
		using namespace Microsoft::UI::Xaml::Controls;
		using namespace Microsoft::UI::Xaml::Media;
		IObservableVector<IInspectable> pages = single_threaded_observable_vector<IInspectable>();
		for (int i = 0; i < 4; i++)
		{
			wchar_t txt[100] = {};
			swprintf_s(txt, L"Page %d", i + 1);
			TextBlock tb1;
			tb1.Text(txt);
			tb1.HorizontalAlignment(HorizontalAlignment::Center);
			tb1.VerticalAlignment(VerticalAlignment::Center);
			pages.Append(tb1);
		}
		b.ItemsSource(pages);


	}
};


std::shared_ptr<XITEM> CreateXItemGridView()
{
	return std::make_shared< ITEM_GridView>();
}

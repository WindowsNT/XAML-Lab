#include "pch.h"
#include "property.hpp"

#include "paragraphcode.hpp"

class ITEM_BreadcrumbBar : public XITEM_Control
{
public:

	ITEM_BreadcrumbBar()
	{
		ElementName = L"BreadcrumbBar";
		X = BreadcrumbBar();
	}

	

	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{

			auto e = X.as<BreadcrumbBar>();
			for (auto& p : properties)
			{
				if (p->n == L"ItemClicked")
				{
					auto op = std::dynamic_pointer_cast<FUNCTION_PROPERTY>(p);
					if (op)
					{
						if (op->value.empty())
							continue;

						op->f.name = op->value;
						CallbackFunctions.push_back(op->f);
					}
				}
			}
		}
		catch (...)
		{

		}
	}

	std::optional<std::wstring> GetCodeForProperty([[maybe_unused]] PROPERTY* p, [[maybe_unused]] int Type) override
	{
		if (!p)
			return {};

		if (p->n == L"ItemsSource")
		{
			std::vector<wchar_t> txt(100000);
			if (Type == 0) // IDL
				swprintf_s(txt.data(), 100000, L"Windows.Foundation.Collections.IObservableVector<Microsoft.UI.Xaml.FrameworkElement> %s { get; };", p->bindv.c_str());
			if (Type == 1) // H
				swprintf_s(txt.data(), 100000, L"IObservableVector<FrameworkElement> %s();", p->bindv.c_str());
			if (Type == 2) // CPP
			{
				swprintf_s(txt.data(), 100000, LR"(IObservableVector<FrameworkElement> %s()
{
	auto items = single_threaded_observable_vector<FrameworkElement>();

	TextBox te1; 
	te1.Text(L"Hello"); 
	items.Append(te1);

	TextBox te2; 
	te1.Text(L"There"); 
	items.Append(te2);

	Button te3; 
	te3.Content(winrt::box_value(L"A button")); 
	items.Append(te3);

	return items;
}
)", p->bindv.c_str());

			}
			return txt.data();
		}

		return {};
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		auto e = X.as<BreadcrumbBar>();
		if (!properties.empty())
			return properties;

		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = CreatePropertyItemsSource(e);
			if (op)
			{
				op->g = L"BreadcrumbBar";
				op->BindingAnyway = 1;
				properties.push_back(op);
			}
		}
		if (1)
		{
			auto p1 = CreatePropertyForGenericCallback(e, L"ItemClicked");
			if (p1)
			{
				p1->g = L"Events.BreadcrumbBar";
				auto op = std::dynamic_pointer_cast<FUNCTION_PROPERTY>(p1);
				if (op)
				{
					op->f.content = s(78);
				}
				properties.push_back(p1);
			}
		}


		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = BreadcrumbBar();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		AddSomeSource();
		auto b = X.as<BreadcrumbBar>();
		b.Tag(box_value((long long)this));
		return b;
	}



	void AddSomeSource()
	{
		auto b = X.as<BreadcrumbBar>();
		// Create a vector of IInspectable UIElements
		using namespace winrt;
		using namespace Windows::Foundation;
		using namespace Windows::Foundation::Collections;
		using namespace Microsoft::UI::Xaml;
		using namespace Microsoft::UI::Xaml::Controls;
		using namespace Microsoft::UI::Xaml::Media;
		IObservableVector<IInspectable> pages = single_threaded_observable_vector<IInspectable>();
		for (int i = 0; i < 5; i++)
		{
			wchar_t txt[100] = {};
			swprintf_s(txt, L"Item %d", i + 1);
			if (i == 3)
			{
				Button b1;
				b1.Content(box_value(txt));
				pages.Append(winrt::box_value(b1));
				continue;
			}
			pages.Append(winrt::box_value(txt));
		}
		b.ItemsSource(pages);


	}

};


std::shared_ptr<XITEM> CreateXItemBreadcrumbBar()
{
	return std::make_shared< ITEM_BreadcrumbBar>();
}

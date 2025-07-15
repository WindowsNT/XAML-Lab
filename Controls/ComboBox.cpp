#include "pch.h"
#include "property.hpp"

class ITEM_ComboBox : public XITEM_Control
{
public:

	ITEM_ComboBox()
	{
		ElementName = L"ComboBox";
		X = ComboBox();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{
			auto e = X.as<ComboBox>();
			for(auto& p : properties)
			{
				if (p->n == L"Description")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						e.Description(winrt::box_value(op->value));
					}
				}
				if (p->n == L"Header")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						e.Header(winrt::box_value(op->value));
					}
				}
				if (p->n == L"IsEditable")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.IsEditable(op->SelectedIndex != 0);
					}
				}
				if (p->n == L"IsTextSearchEnabled")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.IsTextSearchEnabled(op->SelectedIndex != 0);
					}
				}
				if (p->n == L"PlaceholderText")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						e.PlaceholderText(op->value);
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
		auto e = X.as<ComboBox>();
		if (!properties.empty())
			return properties;


		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = CreatePropertyItemsSource(e);
			op->g = L"ComboBox";
			op->BindingAnyway = 1;
			properties.push_back(op);
		}

		if (1)
		{
			// "Description" string
			auto op = std::make_shared<STRING_PROPERTY>();
			op->g = L"ComboBox";
			op->n = L"Header";
			properties.push_back(op);
		}
		if (1)
		{
			// "Description" string
			auto op = std::make_shared<STRING_PROPERTY>();
			op->g = L"ComboBox";
			op->n = L"Description";
			properties.push_back(op);
		}
		if (1)
		{
			// IsEditable bool
			auto op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"ComboBox";
			op->n = L"IsEditable";
			op->SelectedIndex = e.IsEditable() ? 1 : 0;
			properties.push_back(op);
		}
		if (1)
		{
			// IsTextSearchEnabled bool
			auto op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"ComboBox";
			op->n = L"IsTextSearchEnabled";
			op->SelectedIndex = e.IsTextSearchEnabled() ? 1 : 0;
			properties.push_back(op);
		}

		if (1)
		{
			auto op = std::make_shared<STRING_PROPERTY>();
			op->g = L"ComboBox";
			op->n = L"PlaceholderText";
			properties.push_back(op);
		}

		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = ComboBox();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		
		AddSomeSource();
		
		auto b = X.as<ComboBox>();
		b.Tag(box_value((long long)this));
		return b;
	}

	void AddSomeSource()
	{
		auto b = X.as<ComboBox>();
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


	std::optional<std::wstring> GetCodeForProperty([[maybe_unused]] PROPERTY* p, [[maybe_unused]] int Type) override
	{
		if (!p)
			return {};
		
		if (p->n == L"ItemsSource")
		{
			std::vector<wchar_t> txt(100000);
			if (Type == 0) // IDL
				swprintf_s(txt.data(), 100000, L"Windows.Foundation.Collections.IObservableVector<Microsoft.UI.Xaml.FrameworkElement> %s;", p->bindv.c_str());
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
			tb1.FontSize(24);
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


std::shared_ptr<XITEM> CreateXItemComboBox()
{
	return std::make_shared< ITEM_ComboBox>();
}

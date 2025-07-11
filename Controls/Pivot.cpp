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

	std::optional<FrameworkElement> HasCodeDemos() override
	{
		using namespace Microsoft::UI::Xaml::Controls;
		using namespace Microsoft::UI::Xaml::Documents;
		using namespace Microsoft::UI::Xaml::Media;
		RichTextBlock rtb;

		rtb.Blocks().Append(PlainParagraph(LR"(To create the Pivot items:)"));
		rtb.Blocks().Append(ColoredParagraph(L"<!-- XAML -->"));
		rtb.Blocks().Append(PreParagraph(LR"(<Pivot ItemsSource="{x:Bind PivotItems,Mode=OneWay}" ... />)"));
		rtb.Blocks().Append(ColoredParagraph(L"// C++"));
		rtb.Blocks().Append(PreParagraph(LR"(IObservableVector<PivotItem> PivotItems()
{
	auto items = single_threaded_observable_vector<PivotItem>();

	for (int i = 0; i < 5; i++)
	{
		wchar_t txt[100] = {};
		swprintf_s(txt, L"Item %d", i + 1);
		PivotItem tb1;
		tb1.Header(winrt::box_value(txt));	
		pages.Append(tb1);
	}

	return items;
}
)"));

		return rtb;
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

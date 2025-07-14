#include "pch.h"
#include "property.hpp"
#include "paragraphcode.hpp"

class ITEM_FlipView : public XITEM_Selector
{
public:

	ITEM_FlipView()
	{
		ElementName = L"FlipView";
		X = FlipView();
	}


	virtual void ApplyProperties()
	{
		XITEM_Selector::ApplyProperties();
		try
		{

			auto e = X.as<FlipView>();
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
		auto e = X.as<FlipView>();
		if (!properties.empty())
			return properties;


		auto p2 = XITEM_Selector::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = FlipView();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		
		AddSomeSource();
		
		auto b = X.as<FlipView>();
		b.Tag(box_value((long long)this));
		return b;
	}



	std::optional<FrameworkElement> HasCodeDemos() override
	{
		using namespace Microsoft::UI::Xaml::Controls;
		using namespace Microsoft::UI::Xaml::Documents;
		using namespace Microsoft::UI::Xaml::Media;
		RichTextBlock rtb;

		rtb.Blocks().Append(PlainParagraph(LR"(To create the pages for the FlipView:)"));
		rtb.Blocks().Append(ColoredParagraph(L"<!-- XAML -->"));
		rtb.Blocks().Append(PreParagraph(LR"(<FlipView ItemsSource="{x:Bind FlipItems,Mode=OneWay}" ... />)")[0]);
		rtb.Blocks().Append(ColoredParagraph(L"// C++"));
		rtb.Blocks().Append(PreParagraph(LR"(IObservableVector<IInspectable> FlipItems()
{
	auto items = single_threaded_observable_vector<IInspectable>();

	for (int i = 0; i < 5; i++)
	{
		wchar_t txt[100] = {};
		swprintf_s(txt, L"Page %d", i + 1);
		TextBlock tb1;
		tb1.Text(txt);
		items.Append(tb1);
	}

	return items;
}
)")[0]);

		return rtb;
	}

	void AddSomeSource()
	{
		auto b = X.as<FlipView>();
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
			swprintf_s(txt, L"Page %d", i + 1);
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


std::shared_ptr<XITEM> CreateXItemFlipView()
{
	return std::make_shared< ITEM_FlipView>();
}

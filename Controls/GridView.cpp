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


	std::optional<FrameworkElement> HasCodeDemos() override
	{
		using namespace Microsoft::UI::Xaml::Controls;
		using namespace Microsoft::UI::Xaml::Documents;
		using namespace Microsoft::UI::Xaml::Media;
		RichTextBlock rtb;

		rtb.Blocks().Append(PlainParagraph(LR"(To create the pages for the GridView:)"));
		rtb.Blocks().Append(ColoredParagraph(L"<!-- XAML -->"));
		rtb.Blocks().Append(PreParagraph(LR"(<GridView ItemsSource="{x:Bind GridItems,Mode=OneWay}" ... />)")[0]);
		rtb.Blocks().Append(ColoredParagraph(L"// C++"));
		rtb.Blocks().Append(PreParagraph(LR"(IObservableVector<IInspectable> GridItems()
{
	auto items = single_threaded_observable_vector<IInspectable>();

	for (int i = 0; i < 4; i++)
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

	return items;
}
)")[0]);

		return rtb;
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
			tb1.FontSize(24);
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

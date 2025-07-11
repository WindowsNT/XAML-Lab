#include "pch.h"
#include "property.hpp"
#include "FileSystemTemplateSelector.g.h"

#include "paragraphcode.hpp"

class ITEM_BreadcrumbBar : public XITEM_Control
{
public:

	ITEM_BreadcrumbBar()
	{
		ElementName = L"BreadcrumbBar";
		X = BreadcrumbBar();
	}

	std::optional<FrameworkElement> HasCodeDemos() override
	{
		using namespace Microsoft::UI::Xaml::Controls;
		using namespace Microsoft::UI::Xaml::Documents;
		using namespace Microsoft::UI::Xaml::Media;
		RichTextBlock rtb;

		rtb.Blocks().Append(PlainParagraph(LR"(To populate the Breadcrumb with data, for example 2 TextBlocks and 1 Button:)"));
		rtb.Blocks().Append(ColoredParagraph(L"<!-- XAML -->"));
		rtb.Blocks().Append(PreParagraph(LR"(<BreadcrumbBar ItemsSource="{x:Bind BreadcrumbBarItems,Mode=OneWay}" ... />)"));
		rtb.Blocks().Append(ColoredParagraph(L"// C++"));
		rtb.Blocks().Append(PreParagraph(LR"(IObservableVector<FrameworkElement> BreadcrumbBarItems()
{
	auto items = single_threaded_observable_vector<FrameworkElement>();

	TextBox te1; 
	te1.Text(L"Hello"); 
	items.Append(winrt::box_value(te1));

	TextBox te2; 
	te1.Text(L"There"); 
	items.Append(winrt::box_value(te2));

	Button te3; 
	te3.Content(L"A button"); 
	items.Append(winrt::box_value(te3));

	return items;
}
)"));

		return rtb;
	}

	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{

			auto e = X.as<BreadcrumbBar>();
			for (auto& p : properties)
			{
				p;
			}
		}
		catch (...)
		{

		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		auto e = X.as<BreadcrumbBar>();
		if (!properties.empty())
			return properties;
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
			TextBox te1;
			te1.Text(txt);
			pages.Append(winrt::box_value(txt));
		}
		b.ItemsSource(pages);


	}

};


std::shared_ptr<XITEM> CreateXItemBreadcrumbBar()
{
	return std::make_shared< ITEM_BreadcrumbBar>();
}

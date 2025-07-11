#include "pch.h"
#include "property.hpp"
#include "FileSystemTemplateSelector.g.h"

extern std::map<HWND, winrt::Windows::Foundation::IInspectable> windows;

class ITEM_TreeView : public XITEM_Control
{
public:

	ITEM_TreeView()
	{
		ElementName = L"TreeView";
		X = TreeView();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		try
		{

			auto e = X.as<TreeView>();
			for (auto& p : properties)
			{
				if (p->n == L"CanDragItems")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.CanDragItems(op->SelectedIndex != 0);
					}
				}
				if (p->n == L"CanReorderItems")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.CanReorderItems(op->SelectedIndex != 0);
					}
				}
				if (p->n == L"SelectionMode")
				{
					auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
					if (op)
					{
						e.SelectionMode((winrt::Microsoft::UI::Xaml::Controls::TreeViewSelectionMode)op->SelectedIndex);
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
		auto e = X.as<TreeView>();
		if (!properties.empty())
			return properties;
		if (1)
		{

			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"TreeView";
			op->S = 1;
			op->n = L"CanDragItems";
			op->SelectedIndex = e.CanDragItems();
			properties.push_back(op);
		}
		if (1)
		{

			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"TreeView";
			op->n = L"CanReorderItems";
			op->SelectedIndex = e.CanReorderItems();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->g = L"TreeView";
			op->n = L"SelectionMode";
			op->Items = { L"None", L"Single",L"Multiple"};
			op->DefaultIndex = 1;
			op->SelectedIndex = (size_t)e.SelectionMode();
			properties.push_back(op);

		}


		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = TreeView();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		
		AddSomeSource();
		
		auto b = X.as<TreeView>();
		b.Tag(box_value((long long)this));
		return b;
	}
	XML3::XMLElement root_for_tree;

	void AddSomeSource()
	{
		auto b = X.as<TreeView>();
		winrt::VisualWinUI3::FileSystemTemplateSelector selector;

		for (auto& wi : windows)
		{
			auto the_window = wi.second.as<winrt::VisualWinUI3::MainWindow>();

			// Find the MainPage for it
			auto tnv = the_window.Content().as<winrt::Microsoft::UI::Xaml::Controls::NavigationView>();
			if (!tnv)
				continue;

			auto fr = tnv.FindName(L"contentFrame").as<winrt::Microsoft::UI::Xaml::Controls::Frame>();
			if (!fr)
				continue;
			auto main_page = fr.Content().as<winrt::VisualWinUI3::MainPage>();
			if (!main_page)
				continue;

			auto top = main_page.Content().as<Controls::Panel>();
			selector.LoadTemplates(top);
			if (1)
				break;
		}

		b.ItemTemplateSelector(selector);

		using namespace winrt;
		using namespace Windows::Foundation;
		using namespace Windows::Foundation::Collections;
		using namespace Microsoft::UI::Xaml;
		using namespace Microsoft::UI::Xaml::Controls;
		using namespace Microsoft::UI::Xaml::Media;
		auto m_rootItems = winrt::single_threaded_observable_vector<winrt::VisualWinUI3::FileSystemItem>();

		static bool jx = false;
		if (jx == false)
		{
			jx = true;
			const char* re = R"(<root n="Root">
<e n="Name 1">
	<e n="Sub name 1" />
	<e n="Sub name 2">
		<e n="Sub sub name 1" />
		<e n="Sub sub name 2" />
	</e>
</e>
</root>)";
			root_for_tree = re;
		}
		winrt::VisualWinUI3::FileSystemItem fsit((long long)&root_for_tree, 0);
		m_rootItems.Append(fsit);


		b.ItemsSource(m_rootItems);

	}
	
};


std::shared_ptr<XITEM> CreateXItemTreeView()
{
	return std::make_shared< ITEM_TreeView>();
}

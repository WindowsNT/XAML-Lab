#include "pch.h"
#include "property.hpp"
#include "filesystemitem.h"

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
			std::shared_ptr<STRING_PROPERTY> op = CreatePropertyItemsSource(e);
			op->g = L"TreeView";
			op->BindingAnyway = 1;
			properties.push_back(op);
		}

		if (1)
		{

			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"TreeView";
			op->S = 1;
			op->n = L"CanDragItems";
			properties.push_back(op);
		}
		if (1)
		{

			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"TreeView";
			op->n = L"CanReorderItems";
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

	std::optional<std::wstring> GetCodeForProperty([[maybe_unused]] PROPERTY* p, [[maybe_unused]] int Type) override
	{
		if (!p)
			return {};

		if (p->n == L"ItemsSource")
		{
			std::vector<wchar_t> txt(100000);
			if (Type == 0) // IDL
				swprintf_s(txt.data(), 100000, L"Windows.Foundation.Collections.IObservableVector<winrt::MyApp::MyItem> %s;", p->bindv.c_str());
			if (Type == 1) // H
				swprintf_s(txt.data(), 100000, L"IObservableVector<winrt::MyApp::MyItem> %s();", p->bindv.c_str());
			if (Type == 2) // CPP
			{
				swprintf_s(txt.data(), 100000, LR"(IObservableVector<winrt::MyApp::MyItem> %s()
{
	auto items = single_threaded_observable_vector<winrt::MyApp::MyItem>();

	for (int i = 0; i < 10; i++)
		{
			wchar_t txt[100] = {};
			swprintf_s(txt, L"Item %%d", i + 1);
			winrt::MyApp::MyItem tb1;
			tb1.Name(txt);
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



	TreeViewItem TItem(const wchar_t* t)
	{
		auto treeViewItem = TreeViewItem();
		treeViewItem.IsExpanded(true);
		// Create StackPanel
		auto stackPanel = StackPanel();
		stackPanel.Orientation(Orientation::Horizontal);
		auto textBlock = TextBlock(); textBlock.Text(t);
		stackPanel.Children().Append(textBlock);
		// Set as TreeViewItem Content
		treeViewItem.Content(stackPanel);
		return treeViewItem;
	}

	void AddSomeSource()
	{
		using namespace winrt::Microsoft::UI::Xaml::Controls;
		using namespace winrt::Windows::Foundation::Collections;

		auto tree = X.as<TreeView>(); // your TreeView

		// Create root node
		auto rootNode = TreeViewNode();
		rootNode.Content(box_value(L"Test 1"));
		rootNode.IsExpanded(true);

		// Add children
		for (int i = 0; i < 5; i++)
		{
			auto childNode = TreeViewNode();
			childNode.Content(box_value(L"Child " + std::to_wstring(i + 1)));
			rootNode.Children().Append(childNode);
		}

		// Add more root nodes
		auto rootNode2 = TreeViewNode();
		rootNode2.Content(box_value(L"Test 2"));

		auto rootNode3 = TreeViewNode();
		rootNode3.Content(box_value(L"Test 3"));

		// Set RootNodes
		auto rootNodes = tree.RootNodes();
		rootNodes.Append(rootNode);
		rootNodes.Append(rootNode2);
		rootNodes.Append(rootNode3);
	}

	
};


std::shared_ptr<XITEM> CreateXItemTreeView()
{
	return std::make_shared< ITEM_TreeView>();
}

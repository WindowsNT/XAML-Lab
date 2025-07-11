#pragma once

#include "MenuBuilder.g.h"
#include "FileSystemItem.h"
#include "project.h"
extern std::map<HWND, winrt::Windows::Foundation::IInspectable> windows;
void ReloadMenu2(XML3::XMLElement* root0,XML3::XMLElement* root1, winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItemBase> items);

namespace winrt::VisualWinUI3::implementation
{
    struct MenuBuilder : MenuBuilderT<MenuBuilder>
    {

        std::wstring _PropName;
		bool _PropSep = false;
        bool _TreeViewPropertiesVisibility = false;
        std::wstring _PropGroup;
        winrt::hstring PropGroup()
        {
            return _PropGroup.c_str();
        }
        winrt::hstring PropName()
        {
            return _PropName.c_str();
        }
        bool PropSep()
        {
            return _PropSep;
		}
        void TreeViewPropertiesVisibility(bool b)
        {
            if (_TreeViewPropertiesVisibility == b)
                return;
            _TreeViewPropertiesVisibility = b;
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"TreeViewPropertiesVisibility" });
		}
        bool TreeViewPropertiesVisibility()
        {
            return _TreeViewPropertiesVisibility;
        }

        std::wstring _i1, _i2;
        std::stack<XML3::XMLElement> undo, redo;
        void Push()
        {
            XML3::XMLElement e = root;
            undo.push(e);
        }
        void Undo()
        {
            if (undo.empty())
                return;
            XML3::XMLElement e = root;
            redo.push(e);
            root = undo.top();
            undo.pop();
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
            ReloadMenu();
        }
        void Redo()
        {
            if (redo.empty())
                return;
            XML3::XMLElement e = root;
            undo.push(e);
            root = redo.top();
            redo.pop();
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
            ReloadMenu();
        }



        void i1(winrt::hstring s)
        {
            if (_i1 == s)
                return;
            _i1 = s;
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"i1" });

        }
        winrt::hstring i1()
        {
            return _i1.c_str();
        }
        void i2(winrt::hstring s)
        {
            if (_i2 == s)
                return;
            _i2 = s;
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"i2" });

        }
        winrt::hstring i2()
        {
            return _i2.c_str();
        }

        static winrt::hstring txt(long jx)
        {
            return s(jx);
        }

        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
        {
            return m_propertyChanged.add(handler);
        }
        void PropertyChanged(winrt::event_token const& token) noexcept
        {
            m_propertyChanged.remove(token);
        }
        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        FileSystemTemplateSelector selector;
        XML3::XMLElement root;

        MenuBuilder()
        {
        }

        void TreeLoaded(IInspectable const& r1, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& )
        {
            auto treeView = r1.as<winrt::Microsoft::UI::Xaml::Controls::TreeView>();
            if (!treeView)
                return;
            ExpandAllTreeViewItems(treeView);
		}

        bool PrimaryOK = 0;
        void Input_OK(IInspectable, IInspectable)
        {
            PrimaryOK = 1;
        }

        void OnNew(IInspectable, IInspectable)
        {
            Push();
            root = {};
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
            ReloadMenu();
        }


        void OnSave(IInspectable, IInspectable)
        {
            auto top = Content().as<Controls::Panel>();
            auto cd = top.FindName(L"Input1").as<Controls::ContentDialog>();
            PrimaryOK = 0;
            i1(s(45));
            i2(L"Menu1");
            cd.ShowAsync();
            cd.Closed([this](IInspectable, IInspectable)
                {
                    if (PrimaryOK == 0)
                        return;
                    PrimaryOK = 0;

                    // find main_page
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
                        auto main_page = fr.Content().as<winrt::VisualWinUI3::MenuBuilder>();
                        if (!main_page)
                            continue;

                        if (main_page == *this)
                        {
							PROJECT* project = (PROJECT*)(long long)the_window.prjptr();
                            if (project && project->xfile)
                            {
								auto& m1 = project->xfile->GetRootElement()["Menus"];
                                XML3::XMLElement m2;
                                m2.vv("n").SetWideValue(_i2.c_str());
                                m2.AddElement(root);
                                m1.AddElement(m2);
                                project->xfile->Save();
                                ReloadMenuList();
                                TTip(ttitle, s(4));
                            }
						}
                    }

                });
        }

        void TTip(const wchar_t* t, const wchar_t* s)
        {
            i1(t);
            i2(s);
            auto topnv = Content().as<Panel>();
            TeachingTip cd = topnv.FindName(L"ttip").as<TeachingTip>();
            cd.IsOpen(true);
        }


        void OnUndo(IInspectable const&, IInspectable const&)
        {
            Undo();
        }
        void OnRedo(IInspectable const&, IInspectable const&)
        {
            Redo();
        }


        void ExpandAllTreeViewItems(Controls::TreeView treeView)
        {
            auto items = treeView.RootNodes();
            for (auto const& item : items)
            {
                ExpandItemRecursive(item);
            }
        }

        void ExpandItemRecursive(Controls::TreeViewNode node)
        {
            node.IsExpanded(true);
            for (auto const& child : node.Children())
            {
                ExpandItemRecursive(child);
            }
        }



        void ReloadMenuList()
        {
            // find main_page
            PROJECT* project = 0;
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
                auto main_page = fr.Content().as<winrt::VisualWinUI3::MenuBuilder>();
                if (!main_page)
                    continue;

                if (main_page == *this)
                {
                    project = (PROJECT*)(long long)the_window.prjptr();
                    break;
                }
            }
            if (!project || !project->xfile)
                return;

			auto top = Content().as<Controls::Panel>();

            if (1)
            {
                auto mf = top.FindName(L"MenuLoadX").as<Controls::MenuFlyoutSubItem>();
                mf.Items().Clear();
                auto& m1 = project->xfile->GetRootElement()["Menus"];
                for (auto& e : m1.GetChildren())
                {
                    if (!e)
                        continue;
                    auto n = e->vv("n").GetWideValue();
                    if (n.empty())
                        continue;
                    winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem item1;
                    item1.Text(n);
                    item1.Tag(box_value((long long)e.get()));
                    item1.Click([this](IInspectable wh, IInspectable)
                        {
                            auto item = wh.as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem>();
                            long long ptr = winrt::unbox_value<long long>(item.Tag());
                            XML3::XMLElement* el = (XML3::XMLElement*)ptr;
                            Push();
                            root = *el->GetChildren()[0];
                            ReloadMenu();
                            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
                        });
                    mf.Items().Append(item1);
                }
            }

            if (1)
            {
                auto mf = top.FindName(L"MenuDeleteX").as<Controls::MenuFlyoutSubItem>();
                mf.Items().Clear();
                auto& m1 = project->xfile->GetRootElement()["Menus"];
                for (auto& e : m1.GetChildren())
                {
                    if (!e)
                        continue;
                    auto n = e->vv("n").GetWideValue();
                    if (n.empty())
                        continue;
                    winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem item1;
                    item1.Text(n);
                    item1.Tag(box_value((long long)e.get()));
                    item1.Click([this](IInspectable wh, IInspectable)
                        {
                            auto item = wh.as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem>();
                            long long ptr = winrt::unbox_value<long long>(item.Tag());
                            XML3::XMLElement* el = (XML3::XMLElement*)ptr;
                            auto par = el->GetParent(el);
                            if (par)
                            {
                                par->RemoveElement(el);
                                ReloadMenuList();
                            }
                        });
                    mf.Items().Append(item1);
                }
            }

        }

        void PageLoaded(IInspectable, IInspectable)
        {
            ReloadMenuList();
        }


        void OnTemplate1(IInspectable const&, IInspectable const&)
        {
            Push();
            const char* menu = 
R"(<root>
	<e n="File">
		<e n="New"/>
		<e n="Open"/>
		<e n="Save"/>
		<e n="Save As..."/>
		<e sep="1" n="---"/>
		<e n="Exit"/>
	</e>
	<e n="Edit">
		<e n="Undo"/>
		<e n="Redo"/>
		<e sep="1" n="---"/>
		<e n="Select All"/>
		<e n="Unselect All"/>
	</e>
	<e n="View">
		<e n="Menu"/>
		<e n="Toolbar"/>
		<e n="Statusbar"/>  
	</e>
	<e n="Help">
		<e n="Help"/>
		<e sep="1" n="---"/>
		<e n="About..."/>
	</e>
</root>)";

            XML3::XMLElement e = menu;
            root = e;
            root.SetElementName(L"Root");
            root.vv("n").SetValue(L"Root");
            ReloadMenu();
			m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
        }

        void ReloadMenu()
        {
            auto top = Content().as<Controls::Panel>();
            if (!top)
                return;
            auto ddb = top.FindName(L"xDropDown").as<Controls::DropDownButton>();
            if (!ddb)
                return;
            auto fly = ddb.Flyout();
            if (!fly)
            {
                fly = winrt::Microsoft::UI::Xaml::Controls::MenuFlyout();
				ddb.Flyout(fly);
                fly = ddb.Flyout();
            }
            if (!fly)
				return;
            fly.Placement(winrt::Microsoft::UI::Xaml::Controls::Primitives::FlyoutPlacementMode::Bottom);
            auto ffly = fly.as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyout>();
            ffly.Items().Clear();
            
			ReloadMenu2(&root,&root,ffly.Items());
	
        }

        winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector dts()
        {
            auto top = Content().as<Controls::Panel>();
            selector.LoadTemplates(top);
            return selector;
        }
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::FileSystemItem> RootItems()
        {
            auto m_rootItems = winrt::single_threaded_observable_vector<winrt::VisualWinUI3::FileSystemItem>();
			root.SetElementName(L"Root");
			root.vv("n").SetValue(L"Root");
            winrt::VisualWinUI3::FileSystemItem fsit((long long)&root,0);
			m_rootItems.Append(fsit);
            return m_rootItems;
		}

        winrt::Microsoft::UI::Xaml::Controls::TreeViewItem SelectedTreeItem = nullptr;

        void PropGroup(winrt::hstring s)
        {
            if (_PropGroup == s)
                return;
            _PropGroup = s.c_str();
            if (SelectedTreeItem)
            {
                auto fsit = SelectedTreeItem.DataContext().as<winrt::VisualWinUI3::FileSystemItem>();
                if (!fsit)
                    return;
                fsit.Group(_PropGroup.c_str());
                ReloadMenu();
            }
            else
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"PropGroup" });
        }

        void PropName(winrt::hstring s)
        {
            if (_PropName == s)
                return;
            _PropName = s;
            if (SelectedTreeItem)
            {
                auto fsit = SelectedTreeItem.DataContext().as<winrt::VisualWinUI3::FileSystemItem>();
                if (!fsit)
					return;
				fsit.Name(_PropName.c_str());
                ReloadMenu();
            }
            else
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"PropName" });
        }
        void PropSep(bool b)
        {
            if (_PropSep == b)
                return;
            _PropSep = b;
            if (SelectedTreeItem)
            {
                auto fsit = SelectedTreeItem.DataContext().as<winrt::VisualWinUI3::FileSystemItem>();
                if (!fsit)
                    return;
                auto ptr = fsit.Ptr();
                XML3::XMLElement* el = (XML3::XMLElement*)ptr;
                if (!el)
                    return;
                el->vv("sep").SetValueInt(_PropSep);
                if (_PropSep)
                    el->vv("n").SetValue("-----");
                ReloadMenu();
                fsit.RaisePropertyChanged(L"Name");
            }
            m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"PropSep" });
        }

        std::unordered_set<long long> expandedItemIds;
        void SaveExpandedState(Microsoft::UI::Xaml::Controls::TreeView const& treeView)
        {
            expandedItemIds.clear();
            for (auto const& item : treeView.RootNodes())
            {
                SaveExpandedStateRecursive(item);
            }
        }

        void SaveExpandedStateRecursive(Microsoft::UI::Xaml::Controls::TreeViewNode const& node)
        {
            if (node.IsExpanded())
            {
                auto item = node.Content().as<winrt::VisualWinUI3::FileSystemItem>();
                expandedItemIds.insert(item.Ptr()); // Use a unique key like Name or GUID
            }

            for (auto const& child : node.Children())
            {
                SaveExpandedStateRecursive(child);
            }
        }

    
        void RestoreExpandedState(Microsoft::UI::Xaml::Controls::TreeView const& treeView)
        {
            for (auto const& item : treeView.RootNodes())
            {
                RestoreExpandedStateRecursive(item);
            }
        }

        void RestoreExpandedStateRecursive(Microsoft::UI::Xaml::Controls::TreeViewNode const& node)
        {
            auto item = node.Content().as<winrt::VisualWinUI3::FileSystemItem>();
            if (expandedItemIds.find(item.Ptr()) != expandedItemIds.end())
            {
//                node.IsExpanded(true);
            }

            for (auto const& child : node.Children())
            {
                RestoreExpandedStateRecursive(child);
            }
        }


        void LeftTree(IInspectable const& r1, [[maybe_unused]] winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const& r2)
        {
            auto tree = r1.as<winrt::Microsoft::UI::Xaml::Controls::TreeView>();
			auto si = tree.SelectedItem();
            if (!si)
                return;
			auto fsit = si.as<winrt::VisualWinUI3::FileSystemItem>();
			auto treeItem = tree.ContainerFromItem(si).try_as< winrt::Microsoft::UI::Xaml::Controls::TreeViewItem>();

/*            auto os = r2.OriginalSource();
            if (!os)
                return;
            // Find the TreeViewItem that was tapped
            if (os.try_as<winrt::Microsoft::UI::Xaml::Controls::TreeViewItem>())
                os = os.as<winrt::Microsoft::UI::Xaml::Controls::TreeViewItem>().DataContext();
            else
                os = os.as<winrt::Microsoft::UI::Xaml::FrameworkElement>().DataContext();
            auto item = tree.ContainerFromItem(os);
            if (!item)
                return;

            auto treeItem = item.as<winrt::Microsoft::UI::Xaml::Controls::TreeViewItem>();
            if (!treeItem)
                return;
            auto fsit = treeItem.DataContext().as<winrt::VisualWinUI3::FileSystemItem>();
            if (!fsit)
                return;
*/
            auto ptr = fsit.Ptr();
			XML3::XMLElement* el = (XML3::XMLElement*)ptr;
            if (!el)
				return;
            if (SelectedTreeItem == treeItem)
                return; // no change
            SelectedTreeItem = nullptr;
            PropName(el->vv("n").GetWideValue().c_str());
            PropSep(el->vv("sep").GetValue() == "1");
            PropGroup(el->vv("g").GetWideValue().c_str());
            SelectedTreeItem = treeItem;
            TreeViewPropertiesVisibility(true);
        }


        bool ReloadPar(winrt::VisualWinUI3::FileSystemItem fsit)
        {
            auto ParPtr = fsit.ParPtr();
            if (ParPtr)
            {
                FileSystemItem* fsitp = (FileSystemItem*)(long long)ParPtr;
                auto top = Content().as<Controls::Panel>();
                auto tree = top.FindName(L"xTreeView").as<Controls::TreeView>();
                if (tree)
                {
                    auto item = tree.ContainerFromItem(*fsitp).as<winrt::Microsoft::UI::Xaml::Controls::TreeViewItem>();
                    if (item)
                    {
                        item.IsExpanded(true);
                        SelectedTreeItem.IsExpanded(true);
                        fsitp->RaisePropertyChanged(L"Children");
                    }
                }
                return true;
            }
            else
                return false;
        }

        void ButtonAdd(IInspectable, IInspectable)
        {
            if (!SelectedTreeItem)
                return;
			auto fsit = SelectedTreeItem.DataContext().as<winrt::VisualWinUI3::FileSystemItem>();
            if (!fsit)
                return;
            long long ptr = fsit.Ptr();
            XML3::XMLElement* el = (XML3::XMLElement*)ptr;
            if (!el)
                return;
            Push();
            auto& newel = el->AddElement("e");
            newel.vv("n").SetValue(L"Item");
            ReloadMenu();

            if (!ReloadPar(fsit))
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
        }

        void ButtonAddSep(IInspectable, IInspectable)
        {
            if (!SelectedTreeItem)
                return;
            auto fsit = SelectedTreeItem.DataContext().as<winrt::VisualWinUI3::FileSystemItem>();
            if (!fsit)
                return;
            long long ptr = fsit.Ptr();
            XML3::XMLElement* el = (XML3::XMLElement*)ptr;
            if (!el)
                return;
            Push();
            auto& newel = el->AddElement("e");
            newel.vv("n").SetValue(L"-----");
            newel.vv("sep").SetValue(L"1");
            ReloadMenu();
       
            if (!ReloadPar(fsit))
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });

        }

        void ButtonDelete(IInspectable, IInspectable)
        {
            if (!SelectedTreeItem)
				return;
            auto fsit = SelectedTreeItem.DataContext().as<winrt::VisualWinUI3::FileSystemItem>();
			if (!fsit)
                return;
			long long ptr = fsit.Ptr();
			XML3::XMLElement* el = (XML3::XMLElement*)ptr;
			if (!el)
				return;
			auto par = el->GetParent(el);
			if (!par)
				return;
			Push();
			par->RemoveElement(el);
			ReloadMenu();

            if (!ReloadPar(fsit))
                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
        }


        void RightTree(IInspectable const& , winrt::Microsoft::UI::Xaml::Input::RightTappedRoutedEventArgs const& )
        {
/*            // TreeView right tapped, find the item which was tapped
			auto tree = r1.as<winrt::Microsoft::UI::Xaml::Controls::TreeView>();
            auto os = r2.OriginalSource();
			if (!os)
				return;
			// Find the TreeViewItem that was tapped
            if (os.try_as<winrt::Microsoft::UI::Xaml::Controls::TreeViewItem>())
                os = os.as<winrt::Microsoft::UI::Xaml::Controls::TreeViewItem>().DataContext();
            else
				os = os.as<winrt::Microsoft::UI::Xaml::FrameworkElement>().DataContext();
			auto item = tree.ContainerFromItem(os); 
			if (!item)
				return;

			auto treeItem = item.as<winrt::Microsoft::UI::Xaml::Controls::TreeViewItem>();
			if (!treeItem)
				return;
			auto fsit = treeItem.DataContext().as<winrt::VisualWinUI3::FileSystemItem>();
			if (!fsit)
				return;
            winrt::Microsoft::UI::Xaml::Controls::MenuFlyout menu;
			if (!menu)
				return;
			menu.Items().Clear();
            if (1)
            {
                winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem item1;
                item1.Text(s(39)); // add item
                item1.Click([tree, fsit, this](IInspectable const&, IInspectable const&)
                    {
                        static long long ptr;
                        ptr = fsit.Ptr();
                        i2(L"Item");
                        auto top = Content().as<Controls::Panel>();
                        auto cd = top.FindName(L"Input1").as<Controls::ContentDialog>();
                        PrimaryOK = 0;
                        cd.ShowAsync();
                        static bool bb = 0;
                        bb = 0;
                        cd.Closed([this](IInspectable, IInspectable)
                            {
                                if (bb == 1 || PrimaryOK == 0)
                                    return;
                                PrimaryOK = 0;
                                Push();
                                bb = 1;
                                XML3::XMLElement* el = (XML3::XMLElement*)ptr;
                                el->AddElement("e").vv("n").SetValue(_i2.c_str());
                                ReloadMenu();
                                m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
                            });
                    });
                menu.Items().Append(item1);
            }
            if (1)
            {
                winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem item1;
                item1.Text(s(40)); // add sep
                item1.Click([tree, fsit, this](IInspectable const&, IInspectable const&)
                    {
                        Push();
                        long long ptr = 0;
                        ptr = fsit.Ptr();
                        XML3::XMLElement* el = (XML3::XMLElement*)ptr;
                        auto& sepel = el->AddElement("e");
                        sepel.vv("sep").SetValue("1");
                        sepel.vv("n").SetValue("---------------");
                        ReloadMenu();
                        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
                    });
                menu.Items().Append(item1);
            }
            winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem item3;
            item3.Text(s(41)); // delete item
            item3.Click([fsit, this](IInspectable const&, IInspectable const&)
                {
                    long long ptr = fsit.Ptr();
                    XML3::XMLElement* el = (XML3::XMLElement*)ptr;
                    auto par = el->GetParent(el);
                    if (!par)
                        return;
                    Push();
                    par->RemoveElement(el);
                    ReloadMenu();
                    m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"" });
                });
            menu.Items().Append(item3);

			// show the menu at the position of the TreeViewItem
			auto position = r2.GetPosition(treeItem);
			menu.ShowAt(treeItem, position);
  */              

        }

    };
}

namespace winrt::VisualWinUI3::factory_implementation
{
    struct MenuBuilder : MenuBuilderT<MenuBuilder, implementation::MenuBuilder>
    {
    };
}




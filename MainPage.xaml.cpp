#include "pch.h"
#include "MainPage.xaml.h"
#if __has_include("MainPage.g.cpp")
#include "MainPage.g.cpp"
#endif

#include "BlankWindow.g.h"
#include "CodeView.g.h"
#include "templates.hpp"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
extern std::map<HWND, winrt::Windows::Foundation::IInspectable> windows;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.
bool IsCommandBar(std::shared_ptr<XITEM> xit);
bool IsNavigationView(std::shared_ptr<XITEM> xit);

bool RefreshFromGrid = 0;
void GetAllItems(std::shared_ptr<XITEM> r, std::vector<std::shared_ptr<XITEM>>& items);
void UnselectAll(std::shared_ptr<XITEM> r);
bool SelectLoop(std::shared_ptr<XITEM> r, XITEM* xit);
bool DeleteLoop(std::shared_ptr<XITEM> r, XITEM* xit);

namespace winrt::VisualWinUI3::implementation
{
    void MainPage::Refresh(const wchar_t* s)
    {
        m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ s });
    }
	void MainPage::Refresh2(winrt::hstring s)
	{
		m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ s });
	}

	void MainPage::OnAbout(IInspectable const&, IInspectable const&)
	{
		Message( L"XAML Lab\r\nCopyright Chourdakis Michael, 2025.");
		return;
	}
	void MainPage::OnGitHub(IInspectable const&, IInspectable const&)
	{
		ShellExecute(nullptr, L"open", L"https://github.com/WindowsNT/XAML-Lab", nullptr, nullptr, SW_SHOWNORMAL);
		return;
	}

	void MainPage::OnTemplate0(const char* t)
	{
		if (!t)
			return;
		Push();
		ForceCreateProject();
		project->xfile = std::make_shared<XML3::XML>();
		project->file.clear();
		project->xfile->Parse(t, strlen(t));
		project->Unser(*project->xfile->GetRootElement()["Design"].GetChildren()[0]);
		dirty = 1;
		Build();
		Refresh2(L"PropertyItems");
		Refresh2(L"dts");
		Refresh2(L"RootItems");
		Refresh2(L"ViewCodeSample");
		Refresh2(L"PropertyTypeSelector");
		Refresh2(L"CanAddPanel");
		Refresh2(L"CanAddElement");
	}

	void MainPage::OnTemplate1(IInspectable const&, IInspectable const&)
	{
		OnTemplate0(template_1);
	}
	void MainPage::OnTemplate2(IInspectable const&, IInspectable const&)
	{
		OnTemplate0(template_2);
	}
	void MainPage::OnTemplate3(IInspectable const&, IInspectable const&)
	{
		OnTemplate0(template_3);
	}
	void MainPage::OnTemplate4(IInspectable const&, IInspectable const&)
	{
		OnTemplate0(template_4);
	}
	void MainPage::OnTemplate5(IInspectable const&, IInspectable const&)
	{
		OnTemplate0(template_5);
	}
	void MainPage::OnTemplate6(IInspectable const&, IInspectable const&)
	{
		OnTemplate0(template_6);
	}


	int Message3Result = -1;
	void MainPage::Message3_1(IInspectable const&, IInspectable const&)
	{
		Message3Result = 1; 
	}
	void MainPage::Message3_2(IInspectable const&, IInspectable const&)
	{
		Message3Result = 2;
	}
	void MainPage::Message3_3(IInspectable const&, IInspectable const&)
	{
		Message3Result = 3;
	}


	void MainPage::AskSaveChanges()
	{
		i1(s(34));
		Message3Result = -1;
		auto topnv = Content().as<Panel>();
		ContentDialog cd = topnv.FindName(L"Message3").as<ContentDialog>();
		cd.ShowAsync();
		cd.Closed([this](IInspectable const&, ContentDialogClosedEventArgs const&)
		{
			if (Message3Result == 1)
			{
				OnSave(IInspectable(), IInspectable());
				if (dirty == 0)
					OnExit(IInspectable(), IInspectable());
			}
			else if (Message3Result == 2)
			{
				dirty = 0;
				OnExit(IInspectable(), IInspectable());
			}
			else if (Message3Result == 3)
			{
				return; // Cancel
			}
			Message3Result = -1;
			});
	}


	void MainPage::OnNew(IInspectable const&, IInspectable const&)
	{
		winrt::VisualWinUI3::MainWindow CreateWi();
		CreateWi();

	}

	void MainPage::OnViewElements(IInspectable const&, IInspectable const&)
	{
		ViewElements(!_ViewElements);
	}
	void MainPage::OnViewProperties(IInspectable const&, IInspectable const&)
	{
		ViewProperties(!_ViewProperties);
	}

	void MainPage::OnUndo(IInspectable const&, IInspectable const&)
	{
		Undo();
	}
	void MainPage::OnRedo(IInspectable const&, IInspectable const&)
	{
		Redo();
	}


	void SelectLoop3(std::shared_ptr<XITEM> r)
	{
		if (!r)
			return;
		r->Selected = 1;
		r->Select();
		for (auto& c : r->children)
		{
			SelectLoop3(c);
		}
	}


	void MainPage::OnSelectAll(IInspectable const&, IInspectable const&)
	{
 		if (!project)
			return;
		auto c = FirstSelectedItem();
		if (c && AllSelectedItems().size() == 1)
			SelectLoop3(c);
		else
			SelectLoop3(project->root);
	}

	void UnselectLoop3(std::shared_ptr<XITEM> r)
	{
		if (!r)
			return;
		r->Selected = 0;
		r->Unselect();
		for (auto& c : r->children)
		{
			SelectLoop3(c);
		}
	}


	void MainPage::OnUnselectAll(IInspectable const&, IInspectable const&)
	{
		if (!project)
			return;
		auto c = FirstSelectedItem();
		if (c && AllSelectedItems().size() == 1)
			UnselectLoop3(c);
		else
			UnselectLoop3(project->root);
	}

	void MainPage::OnClear(IInspectable const&, IInspectable const&)
	{
		if (!project)
			return;
		Push();
		project->root = nullptr;
		Build();
		Refresh();
	}

	void MainPage::OnDelete(IInspectable const&, IInspectable const&)
	{
		auto all = AllSelectedItems();
		if (all.empty())
			return;
		int C = 0;
		for (auto& SelectedItem : all)
		{
			if (SelectedItem == project->root)
			{
				Message(s(36));
				continue;
			}
			if (C == 0)
				Push();
			C++;
			DeleteLoop(project->root, SelectedItem.get());
		}
		Build();
		Refresh2(L"PropertyItems");
		Refresh2(L"dts");
		Refresh2(L"RootItems");
		Refresh2(L"ViewCodeSample");
		Refresh2(L"PropertyTypeSelector");
		Refresh2(L"CanAddPanel");
		Refresh2(L"CanAddElement");
	}

	void MainPage::ForceCreateProject()
	{
		if (!project)
		{
			project = std::make_shared<PROJECT>();

			// Pass it to my window
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

				if (main_page.ProjectPtr() == (long long)project.get())
				{
					the_window.prjptr((long long)project.get());
				}
			}
		}
	}

	void MainPage::OnOpen(IInspectable const&, IInspectable const&)
	{
		OPENFILENAME of = { 0 };
		of.lStructSize = sizeof(of);
		of.hwndOwner = (HWND)0;
		of.lpstrFilter = L"*.xamllab\0*.xamllab\0\0";
		std::vector<wchar_t> fnx(10000);
		of.lpstrFile = fnx.data();
		of.nMaxFile = 10000;
		of.Flags = OFN_FILEMUSTEXIST;
		if (!GetOpenFileName(&of))
			return;

		if (project && project->root)
		{
			winrt::VisualWinUI3::MainWindow CreateWi();
			ToOpenFile = fnx.data();
			CreateWi();
		}
		else
		{
			if (!project)
				project = std::make_shared<PROJECT>();
			if (!project->root)
			{
				project->file = fnx.data();
				if (!project->xfile)
					project->xfile = std::make_shared<XML3::XML>(project->file.c_str());	
				project->Unser(*project->xfile->GetRootElement()["Design"].GetChildren()[0]);
				Build();
				Refresh2(L"PropertyItems");
				Refresh2(L"PropertyTypeSelector");
				Refresh2(L"dts");
				Refresh2(L"ViewCodeSample");
				Refresh2(L"RootItems");
				Refresh2(L"CanAddPanel");
				Refresh2(L"CanAddElement");
				ReloadMenuList();

			}
		}

	}

	void MainPage::Save()
	{
		OnSave(nullptr, nullptr);
	}
	std::vector<std::shared_ptr<XML3::XMLElement>> CacheForMenuList;
	void MainPage::ReloadMenuList()
	{
		if (!project || !project->xfile)
			return;
		auto top = Content().as<Controls::Panel>();
		if (1)
		{
			auto mf = top.FindName(L"PageLoadMenu").as<Controls::MenuFlyoutSubItem>();
			mf.Items().Clear();
			auto& m1 = project->xfile->GetRootElement()["Pages"];
			for (auto& e : m1.GetChildren())
			{
				if (!e)
					continue;
				auto n = e->vv("n").GetWideValue();
				if (n.empty())
					continue;
				winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem item1;
				item1.Text(n);
				auto j = std::make_shared<XML3::XMLElement>(*e);
				CacheForMenuList.push_back(j);
				item1.Tag(box_value((long long)j.get()));
				item1.Click([this](IInspectable wh, IInspectable)
					{
						auto item = wh.as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem>();
						long long ptr = winrt::unbox_value<long long>(item.Tag());
						XML3::XMLElement* el = (XML3::XMLElement*)ptr;
						el;
						if (project)
							Push();
						else
							ForceCreateProject();
						project->Unser(*el->GetChildren()[0]);
						Build();
						Refresh();
					});
				mf.Items().Append(item1);
			}
		}

		if (1)
		{
			auto mf = top.FindName(L"PageDeleteMenu").as<Controls::MenuFlyoutSubItem>();
			mf.Items().Clear();
			auto& m1 = project->xfile->GetRootElement()["Pages"];
			for (auto& e : m1.GetChildren())
			{
				if (!e)
					continue;
				auto n = e->vv("n").GetWideValue();
				if (n.empty())
					continue;
				winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem item1;
				item1.Text(n);
				auto j = std::make_shared<XML3::XMLElement>(*e);
				CacheForMenuList.push_back(j);
				item1.Tag(box_value((long long)j.get()));
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

	void MainPage::OnPageSave(IInspectable const&, IInspectable const&)
	{
		auto top = Content().as<Controls::Panel>();
		auto cd = top.FindName(L"Input1").as<Controls::ContentDialog>();
		PrimaryOK = 0;
		i1(s(66));
		i2(L"Page1");
		cd.ShowAsync();
		cd.Closed([this](IInspectable, IInspectable)
			{
				if (PrimaryOK == 0)
					return;
				PrimaryOK = 0;

				if (project && project->xfile)
				{
					auto& m1 = project->xfile->GetRootElement()["Pages"];
					XML3::XMLElement m2;
					m2.vv("n").SetWideValue(i2().c_str());
					project->Ser(m2);
					m1.AddElement(m2);
					ReloadMenuList();
					dirty = 1;
				}

			});
	}

	void MainPage::OnSave(IInspectable const&, IInspectable const&)
	{
		if (!project)
			return;
		if (!project->root)
			return;
		if (project->file.empty())
		{
			OnSaveAs(nullptr, nullptr);
			return;
		}
		SwitchPropertyMode(0); // make sure we are in PropertyItemsMode 0
		DeleteFile(project->file.c_str());
		if (project->xfile)
		{
			auto& dr = project->xfile->GetRootElement()["Design"];
			dr.RemoveAllElements();
			dr.RemoveAllVariables();
			project->Ser(dr);
			project->xfile->Save(project->file.c_str());
		}
		else
		{
			project->xfile = std::make_shared<XML3::XML>(project->file.c_str());
			auto& dr = project->xfile->GetRootElement()["Design"];
			dr.RemoveAllElements();
			dr.RemoveAllVariables();
			project->Ser(dr);
			project->xfile->Save();
		}
		dirty = false;
	}
	void MainPage::OnExit(IInspectable const&, IInspectable const&)
	{
		PostMessage((HWND)wnd(), WM_CLOSE, 0, 0);
	}
	void MainPage::OnSaveAs(IInspectable const&, IInspectable const&)
	{
		OPENFILENAME of = { 0 };
		of.lStructSize = sizeof(of);
		of.hwndOwner = (HWND)0;
		of.lpstrFilter = L"*.xamllab\0*.xamllab\0\0";
		std::vector<wchar_t> fnx(10000);
		of.lpstrFile = fnx.data();
		of.nMaxFile = 10000;
		of.lpstrDefExt = L"xamllab";
		of.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
		if (!GetSaveFileName(&of))
			return;
		project->file = fnx.data();
		if (project->file.empty())
			return;
		OnSave(nullptr, nullptr);
	}
 

	void MainPage::SelectClick(IInspectable c1, IInspectable)
	{
		bool Shift = false;
		Shift = ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0);
		if (!project)
			return;
		if (!project->root)
			return;

		if (!Shift)	
			UnselectAll(project->root);
		long long tg = 0;
		MenuFlyoutItem mfi = c1.try_as<MenuFlyoutItem>();
		if (mfi)
		{
			tg = winrt::unbox_value<long long>(mfi.Tag());
		}
		MenuFlyoutSubItem mfsi = c1.try_as<MenuFlyoutSubItem>();
		if (mfsi)
		{
			tg = winrt::unbox_value<long long>(mfi.Tag());

		}
		if (tg == 0)
			return;
		XITEM* xit = (XITEM*)tg;
		if (!xit)
			return;

		SelectLoop(project->root, xit);
		Refresh(L"PropertyItems");
		Refresh2(L"dts");
		Refresh2(L"RootItems");
		Refresh2(L"ViewCodeSample");
		Refresh2(L"CanAddPanel");
		Refresh2(L"CanAddElement");
		Refresh(L"PropertyTypeSelector");
	}

	void MainPage::ApplyTopProperties()
	{
		if (!project)
			return;
		if (!project->root)
			return;
		dirty = 1;

		auto topnv = Content().as<Panel>();


#ifdef ROOT_NV_ENABLE
		if (ISXItemNavigationView(project->root.get()))
		{
			auto rootnv = topnv.FindName(L"PutRootNV").as<NavigationView>();
			rootnv.Visibility(Visibility::Visible);
			// Apply properties of the root to rootsp
			void ApplyNVProperties(NavigationView nv, std::vector<std::shared_ptr<PROPERTY>>);
			ApplyNVProperties(rootnv, project->root->properties);
		}
		else
		{
			auto rootnv = topnv.FindName(L"PutRootNV").as<NavigationView>();
			rootnv.Visibility(Visibility::Collapsed);
#else
			{
#endif
			Panel rootsp = StackPanel();
			if (ISXItemStackPanel(project->root.get()))
				rootsp = topnv.FindName(L"PutRootSP").as<Panel>();
			if (ISXItemGrid(project->root.get()))
				rootsp = topnv.FindName(L"PutRootGR").as<Panel>();
			if (ISXItemRelativePanel(project->root.get()))
				rootsp = topnv.FindName(L"PutRootRP").as<Panel>();
			if (ISXItemCanvas(project->root.get()))
				rootsp = topnv.FindName(L"PutRootCA").as<Panel>();
			if (ISXItemSwapChainPanel(project->root.get()))
				rootsp = topnv.FindName(L"PutRootSC").as<Panel>();

			// Apply properties of the root to rootsp
			XITEM_Panel panel;
			panel.properties = project->root->properties;
			panel.X = rootsp;
			panel.ApplyProperties();
		}

	}
	
	int NextNumberAv = 1;

	void MainPage::LeftTree(IInspectable const& r1, [[maybe_unused]] winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const& r2)
	{
		if (!project)
			return;
		if (!project->root)
			return;
		auto tree = r1.as<winrt::Microsoft::UI::Xaml::Controls::TreeView>();
		auto si = tree.SelectedItem();
		if (!si)
			return;
		auto fsit = si.as<winrt::VisualWinUI3::FileSystemItem>();
		auto treeItem = tree.ContainerFromItem(si).try_as< winrt::Microsoft::UI::Xaml::Controls::TreeViewItem>();
		auto ptr = fsit.Ptr();
		XML3::XMLElement* el = (XML3::XMLElement*)ptr;
		if (!el)
			return;

		// Already selected?
		auto vv = el->vv("vv").GetValueULongLong();
		if (!vv)
			return;
		XITEM* xit = (XITEM*)vv;
		auto FirstSelected = FirstSelectedItem();
		if (FirstSelected && FirstSelected.get() == xit)
		{
			// Already selected
			return;
		}

		// Select this xit
		bool Shift = false;
		Shift = ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0);
		if (!Shift)
			UnselectAll(project->root);

		SelectLoop(project->root, xit);
		Refresh(L"PropertyItems");
		Refresh2(L"ViewCodeSample");
		Refresh2(L"CanAddPanel");
		Refresh2(L"CanAddElement");
	}

	void PopulateElementRoot(XML3::XMLElement* e, XITEM* xit)
	{
		if (!e || !xit)
			return;

		e->vv("n").SetValue(xit->ElementName);	
		e->vv("vv").SetValueULongLong((unsigned long long)xit);
		for (size_t i = 0; i < xit->children.size(); i++)
		{
			auto& e2 = e->AddElement("e");
			PopulateElementRoot(&e2, xit->children[i].get());
		}
	}

	winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::FileSystemItem> MainPage::RootItems()
	{
		// XITEM is the SelectClick
		auto m_rootItems = winrt::single_threaded_observable_vector<winrt::VisualWinUI3::FileSystemItem>();
		root_for_tree = {};
		root_for_tree.SetElementName(L"Root");
		root_for_tree.vv("n").SetValue(L"Root");
		root_for_tree.vv("vv").SetValueULongLong(0);
		if (!project)
			return m_rootItems;
		PopulateElementRoot(&root_for_tree, project->root.get());
		winrt::VisualWinUI3::FileSystemItem fsit((long long)&root_for_tree, 0);
		m_rootItems.Append(fsit);
		return m_rootItems;
	}

	void MainPage::Build(winrt::VisualWinUI3::BlankWindow topbw,UIElement iroot, std::shared_ptr<XITEM> root, winrt::Windows::Foundation::IInspectable menu_root,int ForWhat, std::shared_ptr<XITEM> parentroot,winrt::VisualWinUI3::MainPage ptx)
	{
		if (!root)
			return;

		if (iroot == nullptr && topbw == nullptr)
		{
			// No root to put it in
			return;
		}

		auto ni = root->Create(ForWhat,parentroot.get());
		if (!ni)
			return;

		root->ApplyProperties();

		// Add to panel
		if (iroot)
		{
			auto ipanel = iroot.try_as<Panel>();
			if (!ipanel)
			{
				auto cc = iroot.try_as<ContentControl>();
				if (!cc)
					return;
				cc.Content(ni);
			}
			else
			{
				auto chi = ipanel.Children();
				chi.Append(ni);
			}
		}
		else
		if (topbw)
		{
			topbw.Content(ni);

		}

		// Add to menu
		MenuFlyoutSubItem item2;
		if (menu_root)
		{
			auto mbi = menu_root.try_as<MenuBarItem>();
			if (mbi)
			{
				if (root->children.size())
				{
					item2.Text(root->ElementName);
					item2.Tag(box_value((long long)root.get()));
					mbi.Items().Append(item2);

					// And put also this 
					MenuFlyoutItem item;
					item.Text(root->ElementName);
					item.Tag(box_value((long long)root.get()));
					item.Click([&](IInspectable const& c1, IInspectable const& c2)
						{
							ptx.SelectClick(c1, c2);
						});

					if (NextNumberAv < 10)
					{
						int ka = (int)winrt::Windows::System::VirtualKey::Number0;
						ka += NextNumberAv;
						winrt::Microsoft::UI::Xaml::Input::KeyboardAccelerator kacc;
						kacc.Key((winrt::Windows::System::VirtualKey)ka);
						kacc.Modifiers(winrt::Windows::System::VirtualKeyModifiers::Control);
						item.KeyboardAccelerators().Append(kacc);
						NextNumberAv++;
					}
					item2.Items().Append(item);

					item2.Items().Append(MenuFlyoutSeparator());
				}
				else
				{
					MenuFlyoutItem item;
					item.Text(root->ElementName);
					item.Tag(box_value((long long)root.get()));
					item.Click([&](IInspectable const& c1, IInspectable const& c2)
						{
							ptx.SelectClick(c1, c2);
						});
					mbi.Items().Append(item);
				}
			}
			else
			{
				// see if it s a MenuFlyoutSubItem
				auto mfs = menu_root.try_as<MenuFlyoutSubItem>();
				if (mfs)
				{
					if (root->children.size())
					{
						item2.Text(root->ElementName);
						item2.Tag(box_value((long long)root.get()));
						mfs.Items().Append(item2);

						// And put also this 
						MenuFlyoutItem item;
						item.Text(root->ElementName);
						item.Tag(box_value((long long)root.get()));
						item.Click([&](IInspectable const& c1, IInspectable const& c2)
							{
								ptx.SelectClick(c1, c2);
							});

						if (NextNumberAv < 10)
						{
							int ka = (int)winrt::Windows::System::VirtualKey::Number0;
							ka += NextNumberAv;
							winrt::Microsoft::UI::Xaml::Input::KeyboardAccelerator kacc;
							kacc.Key((winrt::Windows::System::VirtualKey)ka);
							kacc.Modifiers(winrt::Windows::System::VirtualKeyModifiers::Control);
							item.KeyboardAccelerators().Append(kacc);
							NextNumberAv++;
						}

						item2.Items().Append(item);
						item2.Items().Append(MenuFlyoutSeparator());

					}
					else
					{
						MenuFlyoutItem item;
						item.Text(root->ElementName);
						item.Tag(box_value((long long)root.get()));
						item.Click([&](IInspectable const& c1, IInspectable const& c2)
							{
								ptx.SelectClick(c1, c2);
							});
						mfs.Items().Append(item);
					}
				}
			}
		}

		// If it's CommandBar, Don't call 
		if (IsCommandBar(root))
		{
			void BuildCommandBar(winrt::VisualWinUI3::BlankWindow topbw, UIElement iroot, std::shared_ptr<XITEM> root, winrt::Windows::Foundation::IInspectable menu_root, int ForWhat, std::shared_ptr<XITEM> parentroot);
			BuildCommandBar(topbw, ni, root, item2, ForWhat, parentroot);
		}
		else		
		if (IsNavigationView(root))
		{
			void BuildNavigationView(winrt::VisualWinUI3::BlankWindow topbw, UIElement iroot, std::shared_ptr<XITEM> root, winrt::Windows::Foundation::IInspectable menu_root, int ForWhat, std::shared_ptr<XITEM> parentroot);
			BuildNavigationView(topbw, ni, root, item2, ForWhat, parentroot);
		}
		else		
		for (auto& r : root->children)
		{
			Build(topbw, ni, r, item2, ForWhat, root,ptx);
		}
	}

	void MainPage::PageLoaded(IInspectable const&, IInspectable const&)
	{
		if (ToOpenFile.length())
		{
			ForceCreateProject();
			if (GetFileAttributes(ToOpenFile.c_str()) == 0xFFFFFFFF)
				return;
			project->file = ToOpenFile;
			project->xfile = std::make_shared<XML3::XML>(project->file.c_str());
			ToOpenFile.clear();
			project->Unser(*project->xfile->GetRootElement()["Design"].GetChildren()[0]);
			Build();
			Refresh();
			ReloadMenuList();
			dirty = 0;
		}
		else
		{
			TipsEnabled = 1;
			Tip1 = 1;
			auto topnv = Content().as<Panel>();
			TTip(ttitle, s(68), topnv.FindName(L"xLine1").as<FrameworkElement>());
		}
	}

	void MainPage::Build()
	{
		auto topnv = Content().as<Panel>();
		topnv.FindName(L"PutRootSP").as<Panel>().Children().Clear();
		topnv.FindName(L"PutRootGR").as<Panel>().Children().Clear();
		topnv.FindName(L"PutRootRP").as<Panel>().Children().Clear();
		topnv.FindName(L"PutRootCA").as<Panel>().Children().Clear();
		topnv.FindName(L"PutRootSC").as<Panel>().Children().Clear();
#ifdef ROOT_NV_ENABLE
		topnv.FindName(L"PutRootNV").as<NavigationView>().Content(nullptr);
#else
		topnv.FindName(L"PutRootNV").as<Panel>().Children().Clear();
#endif

		auto mrs = topnv.FindName(L"MenuRootSelect").as<MenuBar>();
		if (mrs)
			mrs.Items().Clear();

		if (!project)
			return;
		if (!project->root)
			return;

		Panel rootsp = StackPanel();
		if (ISXItemStackPanel(project->root.get()))
			rootsp = topnv.FindName(L"PutRootSP").as<Panel>();
		if (ISXItemGrid(project->root.get()))
			rootsp = topnv.FindName(L"PutRootGR").as<Panel>();
		if (ISXItemRelativePanel(project->root.get()))
			rootsp = topnv.FindName(L"PutRootRP").as<Panel>();
		if (ISXItemCanvas(project->root.get()))
			rootsp = topnv.FindName(L"PutRootCA").as<Panel>();
		if (ISXItemSwapChainPanel(project->root.get()))
			rootsp = topnv.FindName(L"PutRootSC").as<Panel>();
#ifndef ROOT_NV_ENABLE
		if (ISXItemNavigationView(project->root.get()))
			rootsp = topnv.FindName(L"PutRootNV").as<Panel>();
#endif
		rootsp.Children().Clear();

	
		MenuBarItem mrsitem;
		mrsitem.Title(txt(23).c_str());
		if (mrs)
			mrs.Items().Append(mrsitem);
		NextNumberAv = 1;
#ifdef	ROOT_NV_ENABLE
		if (ISXItemNavigationView(project->root.get()))
		{
			auto nv = topnv.FindName(L"PutRootNV").as<NavigationView>();
			Build(nullptr, nv, project->root, mrsitem, 0, nullptr);
		}
		else
#endif
		MainPage::Build(nullptr, rootsp, project->root, mrsitem, 0,nullptr,*this);
		ApplyTopProperties();
	}

	

	winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector MainPage::PropertyTypeSelector()
	{
		auto m = VisualWinUI3::ItemSelector();
		return m;
	}

	void MainPage::Message(const wchar_t* s)
	{
		i1(s);
		auto topnv = Content().as<Panel>();
		ContentDialog cd = topnv.FindName(L"Message1").as<ContentDialog>();
		cd.ShowAsync();
	}
	void MainPage::TTip(const wchar_t* t,const wchar_t* s, FrameworkElement trg)
	{
		i1(t);
		i2(s);
		auto topnv = Content().as<Panel>();
		TeachingTip cd = topnv.FindName(L"ttip").as<TeachingTip>();
		if (trg)
		{
			cd.Target(trg);
			cd.PreferredPlacement(TeachingTipPlacementMode::Auto);
			if (!TTThemeSet)
			{
				int Theme = SettingsX->GetRootElement().vv("Theme").GetValueInt();
				if (Theme == 0)
				{
					bool is_light_theme();
					if (is_light_theme())
						Theme = 1;
					else
						Theme = 2;
				}
				if (Theme == 2)
					cd.RequestedTheme(winrt::Microsoft::UI::Xaml::ElementTheme::Light);
				else
					cd.RequestedTheme(winrt::Microsoft::UI::Xaml::ElementTheme::Dark);
				TTThemeSet = 1;
			}
		}
		else
			cd.Target(nullptr);
		cd.IsOpen(true);	
	}

	long long MainPage::FirstSelectedItemPtr()
	{
		auto SelectedItem = FirstSelectedItem();
		if (!SelectedItem)
			return 0;
		return (long long)SelectedItem.get();
	}

	std::shared_ptr<XITEM> MainPage::FirstSelectedItem()
	{
		auto all = AllSelectedItems();
		if (all.empty())
			return nullptr;
		return all[0];
	}

	void SelectLoop2(std::vector<std::shared_ptr<XITEM>>& si,std::shared_ptr<XITEM> r)
	{
		if (r->Selected)
		{
			si.push_back(r);
		}
		for (auto& c : r->children)
		{
			SelectLoop2(si, c);
		}
	}



	std::vector<std::shared_ptr<XITEM>> MainPage::AllSelectedItems()
	{
		std::vector<std::shared_ptr<XITEM>> items;
		if (!project)
			return items;
		if (!project->root)
			return items;
		SelectLoop2(items, project->root);
		return items;
	}

	std::vector<std::wstring> items0 = {L"Canvas",L"Grid",L"NavigationView",L"RelativePanel",L"StackPanel",L"SwapchainPanel"};

	std::vector<std::function<std::shared_ptr<XITEM>()>> itemcalls0 = {
	CreateXItemCanvas,		CreateXItemGrid,		CreateXItemNavigationView,		CreateXItemRelativePanel,		CreateXItemStackPanel, 	CreateXItemSwapChainPanel, 		CreateXItemTextBlock,
	};
	
	std::vector<std::wstring> items1 = { L"AnimatedVisualPlayer", L"AppBarButton",L"BreadcrumbBar",L"Button",L"CalendarDatePicker",L"CalendarView",L"CheckBox",L"ColorPicker",
		L"CommandBar",L"DatePicker",L"DropDownButton",L"Expander",L"FlipView",L"GridView",L"HyperlinkButton",
		L"Image",L"InfoBadge",L"InfoBar",L"ListView",L"MapControl",L"MediaPlayerElement",L"MenuBar",
		L"NavigationViewItem",L"NumberBox",L"PasswordBox",L"PersonPicture",L"Pivot",L"ProgressBar",L"ProgressRing",L"RadioButton",L"RatingControl",
		L"RepeatButton",L"RichEditBox",L"RichTextBlock",L"Slider",L"TextBlock",L"TreeView",
		L"TextBox",L"TimePicker",L"ToggleSwitch",L"WebView2" };

	std::vector<std::function<std::shared_ptr<XITEM>()>> itemcalls1 = {
		CreateXItemAnimatedVisualPlayer,	CreateXItemAppBarButton,	CreateXItemBreadcrumbBar,		CreateXItemButton,		CreateXItemCalendarDatePicker,		CreateXItemCalendarView,		CreateXItemCheckBox,		CreateXItemColorPicker,
		CreateXItemCommandBar,CreateXItemDatePicker,		CreateXItemDropDownButton,		CreateXItemExpander,		CreateXItemFlipView,			CreateXItemGridView,		CreateXItemHLButton,
		CreateXItemImage,		CreateXItemInfoBadge,		CreateXItemInfoBar,		CreateXItemListView,		CreateXItemMapControl,		CreateXItemMediaPlayerElement,		CreateXItemMenuBar,
		CreateXItemNavigationViewItem,CreateXItemNumberBox,		CreateXItemPasswordBox,		CreateXItemPersonPicture,		CreateXItemPivot,		CreateXItemProgressBar,		CreateXItemProgressRing,		CreateXItemRadioButton,		CreateXItemRatingControl,
		CreateXItemRepeatButton,		CreateXItemRichEditBox,		CreateXItemRichTextBlock,		CreateXItemSlider,			CreateXItemTextBlock,CreateXItemTreeView,
		CreateXItemTextBox, CreateXItemTimePicker, 	CreateXItemToggleSwitch, CreateXItemWebView2
	};

	void MainPage::ListViewPick1(IInspectable const& sender, IInspectable const&)
	{
		auto listView = sender.as<winrt::Microsoft::UI::Xaml::Controls::ListView>();
		auto selectedItem = listView.SelectedItem(); // This is an IInspectable

		auto item = selectedItem.try_as<winrt::VisualWinUI3::Item>();
		if (item)
		{
			auto n = item.Name1();
			for (size_t i = 0; i < items0.size(); i++)
			{
				if (items0[i] == n)
				{
					if (i < itemcalls0.size())
					{
						auto f = itemcalls0[i];
						if (f)
						{
							GenericAddPanel(f);
						}
					}
					break;
				}
			}


		}
	}

	void MainPage::ListViewPick(IInspectable const& sender, IInspectable const&)
	{
		auto listView = sender.as<winrt::Microsoft::UI::Xaml::Controls::ListView>();
		auto selectedItem = listView.SelectedItem(); // This is an IInspectable

		auto item = selectedItem.try_as<winrt::VisualWinUI3::Item>();
		if (item)
		{
			auto n = item.Name1();
			for(size_t i = 0 ; i < items1.size() ; i++)
			{
				if (items1[i] == n)
				{
					if (i < itemcalls1.size())
					{
						auto f = itemcalls1[i];
						if (f)
						{
							if (!CanAddThis())
								return;
							if (n == L"NavigationViewItem")
							{
								auto SelectedItem = FirstSelectedItem();
								if (!ISXItemNavigationView(SelectedItem.get()))
								{
									Message(txt(67).c_str());
									return;
								}
							}
							GenericAddItemUnder(f);
						}
					}
					break;
				}
			}


		}
	}

	winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::Item> MainPage::Elements()
	{
		auto children = single_threaded_observable_vector<VisualWinUI3::Item>();
		for(size_t i = 0 ; i <  items1.size() ; i++)
		{
			VisualWinUI3::Item i1;
			i1.Name1(items1[i]);
			i1.Number0((double)i);
			children.Append(i1);

		}
		return children;
	}


	winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::Item> MainPage::Panels()
	{
		auto children = single_threaded_observable_vector<VisualWinUI3::Item>();
		for (size_t i = 0; i < items0.size(); i++)
		{
			VisualWinUI3::Item i1;
			i1.Name1(items0[i]);
			i1.Number0((double)i);
			children.Append(i1);

		}
		return children;
	}

	winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::Item> MainPage::PropertyItems()
	{
		auto children = single_threaded_observable_vector<VisualWinUI3::Item>();
		auto SelectedItem = FirstSelectedItem();
		if (!SelectedItem || AllSelectedItems().size() != 1)
			return children;

		bool HasParentGrid = IsSomeParentGrid(SelectedItem->the_par);
		bool HasParentCanvas = IsSomeParentCanvas(SelectedItem->the_par);

		bool UseTree = 0;

		// Put the properties of this item
		std::wstring Current_Group;
		bool CGV = 1;
		for (size_t ip = 0; ip < SelectedItem->properties.size(); ip++)
		{
			auto& pro = SelectedItem->properties[ip];

			if (pro->g == L"Parent Grid" && !HasParentGrid)
				continue; // Skip this group if it is Parent Grid and we are not in a parent grid
			if (pro->g == L"Parent Canvas" && !HasParentCanvas)
				continue; // Skip this group if it is Parent Grid and we are not in a parent grid

			if (pro->g != Current_Group)
			{
				// Put the group as well
				VisualWinUI3::Item item;
				item.PropertyX((long long)pro.get());
				item.Type(PT_HEADER);
				item.Name1(pro->g);

				if (ip == 0 && ISXItemGrid(SelectedItem.get()) && RefreshFromGrid)
				{
					pro->S = 1;
					RefreshFromGrid = 0;
				}

				item.Sel(pro->S);
				children.Append(item);
				if (UseTree)
					children = item.TVChildren();
				CGV = pro->S;
				Current_Group = pro->g;
			}

			if (CGV == 0)
				continue; // skip this group

			// Check CurrentFilter against pro->n
			if (_CurrentFilter.length())
			{
				if (pro->n.find(_CurrentFilter) == std::wstring::npos)
					continue; // Skip this property
			}

			bool IsBindingProperty = false;
			if (pro->bindv.length())
				IsBindingProperty = true;

			auto string_type = std::dynamic_pointer_cast<STRING_PROPERTY>(pro);
			if (string_type && string_type->BindingAnyway)
				IsBindingProperty = true;

			if (PropertyItemsMode() == 0 && IsBindingProperty)
				continue; // This property is used in binding

			if (PropertyItemsMode() == 1)
			{
				// Bindable
				auto func_type = std::dynamic_pointer_cast<FUNCTION_PROPERTY>(pro);
				if (func_type)
					continue;

				VisualWinUI3::Item item;
				item.PropertyX((long long)pro.get());
				item.Type(PT_STRING);
				item.Name1(pro->n);
				item.Value0(pro->bindv);
				item.Tip(pro->tip);
				children.Append(item);
			}
			else
			{
				auto double_type = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(pro);
				if (double_type)
				{
					VisualWinUI3::Item item;
					item.PropertyX((long long)double_type.get());
					item.Type(PT_DOUBLE);
					item.Name1(pro->n);
					item.Tip(pro->tip);
					item.Number0(double_type->value);
					item.Number1(double_type->mmin);
					item.Number2(double_type->mmax);
					item.Change1(double_type->smallchange);
					item.Change2(double_type->largechange);
					children.Append(item);
				}
				auto bool_type = std::dynamic_pointer_cast<BOOL_PROPERTY>(pro);
				if (bool_type)
				{
					VisualWinUI3::Item item;
					item.PropertyX((long long)bool_type.get());
					item.Type(PT_BOOL);
					item.Int0((int)bool_type->SelectedIndex);
					item.Boolean0(bool_type->SelectedIndex);
					item.Name1(pro->n);
					item.Tip(pro->tip);
					children.Append(item);
				}
				auto list_type = std::dynamic_pointer_cast<LIST_PROPERTY>(pro);
				if (list_type)
				{
					VisualWinUI3::Item item;
					item.PropertyX((long long)list_type.get());
					item.Type(PT_LIST);
					item.Int0((int)list_type->SelectedIndex);
					auto its = item.xitems();
					for (auto& e : list_type->Items)
					{
						its.Append(e.c_str());
					}
					item.Name1(pro->n);
					item.Tip(pro->tip);
					children.Append(item);
				}
				auto func_type = std::dynamic_pointer_cast<FUNCTION_PROPERTY>(pro);
				if (func_type)
				{
					VisualWinUI3::Item item;
					item.PropertyX((long long)func_type.get());
					item.Type(PT_FUNCTION);
					item.Value0(func_type->value);
					item.Name1(pro->n);
					item.Tip(pro->tip);
					children.Append(item);
				}

				if (string_type)
				{
					VisualWinUI3::Item item;
					item.PropertyX((long long)string_type.get());
					item.Type(PT_STRING);
					item.Value0(string_type->value);
					item.Name1(pro->n);
					item.Tip(pro->tip);
					children.Append(item);
				}
				auto color_type = std::dynamic_pointer_cast<COLOR_PROPERTY>(pro);
				if (color_type)
				{
					VisualWinUI3::Item item;
					item.PropertyX((long long)color_type.get());
					item.Type(PT_COLOR);
					item.Color0(color_type->value);
					item.Name1(pro->n);
					item.Tip(pro->tip);
					children.Append(item);
				}
			}
		}

		return children;
	}

	void MainPage::E_RUN3(IInspectable const&, IInspectable const&)
	{
		if (!project)
			return;
		if (!project->root)
			return;


		auto topnv = Content().as<Panel>();
		ContentDialog cd = topnv.FindName(L"TestCD").as<ContentDialog>();
		auto ni = project->root->Create(0,project->root.get());
		if (!ni)
			return;

		MainPage::Build(nullptr, ni, project->root, nullptr, 1, nullptr, *this);
		cd.Content(ni);

		cd.ShowAsync();
	}

	void MainPage::E_RUN1(IInspectable const&, IInspectable const&)
	{
		if (!project)
			return;
		if (!project->root)
			return;

		// Show it in the BlankWindow
		winrt::VisualWinUI3::BlankWindow bw;
		
		auto cont = bw.Content();
		auto topnv = Content().as<Panel>();
		MainPage::Build(bw,nullptr, project->root, nullptr,1,nullptr,*this);

		bw.Activate();
	}


	void MainPage::E_RUN2(IInspectable const&, IInspectable const&)
	{
		if (!project)
			return;
		if (!project->root)
			return;

		// Show it in the BlankWindow
		winrt::VisualWinUI3::BlankWindow bw;

		bw.ExtendsContentIntoTitleBar(true);
		auto cont = bw.Content();
		auto topnv = Content().as<Panel>();
		MainPage::Build(bw, nullptr, project->root, nullptr, 1, nullptr,*this);

		bw.Activate();
	}

	void CollectCallbackFunctionsRecursive(std::shared_ptr<XITEM> r, std::vector<FUNCTION_SIG>& fprops)
	{
		if (!r)
			return;
		for (auto& f : r->CallbackFunctions)
		{
			fprops.push_back(f);
		}
		for (auto& c : r->children)
		{
			CollectCallbackFunctionsRecursive(c, fprops);
		}
	}


	void MainPage::SwitchPropertyMode(int nm)
	{
		if (PropertyItemsMode() == nm)
			return; // Already in this mode	


		std::vector<std::shared_ptr<XITEM>> allitems;
		GetAllItems(project->root, allitems);

		if (nm == 1)
			PropertyItemsMode(1);
		else
			PropertyItemsMode(0);
	}

	void MainPage::RDBSC(IInspectable radiobutts, IInspectable)
	{
		auto rb = radiobutts.as<winrt::Microsoft::UI::Xaml::Controls::RadioButtons>();
		auto selectedItem = rb.SelectedItem(); 
		if (!selectedItem)
			return;
		auto str = winrt::unbox_value<winrt::hstring>(selectedItem);
		if (str == L"x:Bind")
		{
			SwitchPropertyMode(1);
			Refresh2(L"PropertyItems");
		}
		else
		{
			SwitchPropertyMode(0);
			Refresh2(L"PropertyItems");
		}
	}


	void MainPage::E_CODE(IInspectable const&, IInspectable const&)
	{
		if (!project)
			return;
		if (!project->root)
			return;
		winrt::VisualWinUI3::CodeView cv;

		// Get the XAML
		ExportForXAML = 1;
		auto el = project->root->SaveEl();
		ExportForXAML = 0;
		auto xaml = el.Serialize();

		// Get callback functions
		std::vector<FUNCTION_SIG> fprops;
		int NextCallbackIfNotName = 1;
		CollectCallbackFunctionsRecursive(project->root, fprops);


		std::wstring idl = LR"(
namespace MyApp
{
	[default_interface]
	runtimeclass MyWindow : Microsoft.UI.Xaml.Window
	{
		MyWindow();
		...
)";

	

		idl += L"***";
		// Add also {x: Bind} found to the props
		std::vector<std::shared_ptr<XITEM>> allitems;
		GetAllItems(project->root, allitems);
		for(auto& item : allitems)
		{
			if (!item)
				continue;
			for (auto& prop : item->properties)
			{
				if (!prop)
					continue;
				if (prop->bindv.length())
				{
					// To the idl
					auto cvi = item->GetCodeForProperty(prop.get(),0);
					if (cvi.has_value())
					{
						idl += L"\t\t";
						idl += cvi.value();
						idl += L"\r\n";
						continue;
					}
					if (1)
					{
						auto str = std::dynamic_pointer_cast<STRING_PROPERTY>(prop);
						if (str)
						{
							idl += L"\t\tString ";
							idl += str->bindv;
							idl += L";\r\n";
						}
					}
					if (1)
					{
						auto dbl = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(prop);
						if (dbl)
						{
							idl += L"\t\tDouble ";
							idl += dbl->bindv;
							idl += L";\r\n";
						}
					}
					//* More stuff here
				}
			}
		}
		idl += L"***";
		idl += LR"(
	}
}
)";


		cv.t_idl(idl.c_str());
		if (1)
		{
			// Create the .H file code
			std::wstring cod = LR"(#pragma once

#include "MyWindow.g.h"

namespace winrt::MyApp::implementation
{
	struct MyWindow : MyWindowT<MyWindow>
	{					
		...

	)";
			cod += L"***";
			for(size_t i = 0 ; i < fprops.size() ; i++)
			{
				auto& f = fprops[i];


				auto name = f.name;
				if (name.length() == 0)
					name = L"Callback" + std::to_wstring(NextCallbackIfNotName++);

				cod += L"\t";
				cod += f.return_type;
				cod += L" ";
				cod += name;
				cod += L"(";
				for (size_t j = 0; j < f.types.size(); j++)
				{
					if (j > 0)
						cod += L", ";
					cod += f.types[j];
					cod += L" ";
					cod += L"param" + std::to_wstring(j + 1);
				}
				cod += L");\r\n";
			}

			// And the declaration of the properties
			for (auto& item : allitems)
			{
				wchar_t x[1000] = {};
				if (!item)
					continue;
				for (auto& prop : item->properties)
				{
					if (!prop)
						continue;

					auto cvi = item->GetCodeForProperty(prop.get(),1);
					if (cvi.has_value())
					{
						cod += L"\t\t";
						cod += cvi.value();
						cod += L"\r\n";
						continue;
					}

					if (prop->bindv.length())
					{
						// To the idl
						if (1)
						{
							auto str = std::dynamic_pointer_cast<STRING_PROPERTY>(prop);
							if (str)
							{
								cod += L"\r\n";
								swprintf_s(x, 1000, LR"(std::wstring _%s = 0;)", prop->bindv.c_str());
								cod += L"\t";
								cod += x;
								cod += L"\r\n";
								swprintf_s(x, 1000, LR"(winrt::hstring %s() { return _%s.c_str();} )", prop->bindv.c_str(), prop->bindv.c_str());
								cod += L"\t";
								cod += x;
								cod += L"\r\n";
								swprintf_s(x, 1000, LR"(void %s(winrt::hstring _param) { _%s = _param;} )", prop->bindv.c_str(), prop->bindv.c_str());
								cod += L"\t";
								cod += x;
								cod += L"\r\n";
							}
						}
						if (1)
						{
							auto dbl = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(prop);
							if (dbl)
							{
								cod += L"\r\n";
								swprintf_s(x, 1000, LR"(double _%s = 0;)",prop->bindv.c_str());
								cod += L"\t";
								cod += x;
								cod += L"\r\n";
								swprintf_s(x, 1000, LR"(double %s() { return _%s;} )", prop->bindv.c_str(), prop->bindv.c_str());
								cod += L"\t";
								cod += x;
								cod += L"\r\n";
								swprintf_s(x, 1000, LR"(void %s(double _param) { _%s = _param;} )", prop->bindv.c_str(), prop->bindv.c_str());
								cod += L"\t";
								cod += x;
								cod += L"\r\n";
							}
						}
						//* More stuff here
					}
				}
			}

			cod += L"***";

			cod += LR"(
	};
}

namespace winrt::MyApp::factory_implementation
{
	struct MyWindow : MyWindowT<MyWindow, implementation::MyWindow>
	{
	};
}
)";
			cv.t_h(cod.c_str());
		}

		NextCallbackIfNotName = 1;
		if (1)
		{
			// Create the .CPP file code
			std::wstring cod = LR"(#include "pch.h"
#include "MyWindow.xaml.h"
#if __has_include("MyWindow.g.cpp")
#include "MyWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::MyApp::implementation
{
	...

)";
			cod += L"***";
			for (size_t i = 0; i < fprops.size(); i++)
			{
				auto& f = fprops[i];
				auto name = f.name;
				if (name.length() == 0)
					name = L"Callback" + std::to_wstring(NextCallbackIfNotName++);

				cod += L"\t";
				cod += f.return_type;
				cod += L" ";
				cod += L"MyWindow::";
				cod += name;
				cod += L"(";
				for (size_t j = 0; j < f.types.size(); j++)
				{
					if (j > 0)
						cod += L", ";
					cod += f.types[j];
					cod += L" ";
					cod += L"param" + std::to_wstring(j + 1);
				}
				cod += L")\r\n\t{";

				// Split f.content to lines
				auto lines = split(f.content.c_str(), '\n');
				for(auto& line : lines)
				{
					if (line.length() == 0)
						continue;
					while(line.length() && line[line.size() - 1] == '\r')
						line.erase(line.size() - 1, 1); // Remove \r at the end
					cod += L"\r\n\t\t";
					cod += line;
				}
				cod += L"\r\n\t}\r\n";
			}


			// And the declaration of the properties
			for (auto& item : allitems)
			{
				if (!item)
					continue;
				for (auto& prop : item->properties)
				{
					if (!prop)
						continue;

					auto cvi = item->GetCodeForProperty(prop.get(), 2);
					if (cvi.has_value())
					{
						auto lines = split(cvi.value(), '\n');
						for (auto& line : lines)
						{
							if (line.length() == 0)
								continue;
							while (line.length() && line[line.size() - 1] == '\r')
								line.erase(line.size() - 1, 1); // Remove \r at the end
							cod += L"\r\n\t\t";
							cod += line;
						}
					}
				}
			}

			cod += L"***";
			cod += LR"(
}
)";
			cv.t_cpp(cod.c_str());
		}


		// Show it in the BlankWindow
		cv.t_xaml(XML3::XMLU(xaml.c_str()).wc());
		cv.ExtendsContentIntoTitleBar(true);
		cv.Activate();
	}

	void MainPage::E_XAML(IInspectable const&, IInspectable const&)
	{
		if (!project)
			return;
		if (!project->root)
			return;

		ExportForXAML = 1;
		auto el = project->root->SaveEl();
		ExportForXAML = 0;
		auto s = el.Serialize();
		void ToClip(HWND MainWindow, const wchar_t* t, bool Empty);
		ToClip((HWND)wnd(), XML3::XMLU(s.c_str()).wc(), true);
#ifdef _DEBUG
		MessageBoxA((HWND)wnd(), s.c_str(), "XAML DEBUG", MB_OK);
#else
		TTip(L"", txt(35).c_str());
#endif
	}

	bool MainPage::GenericAddItemUnder(std::function<std::shared_ptr<XITEM>()> foocall)
	{
		auto SelectedItem = FirstSelectedItem();
		if (!SelectedItem)
			return false;
		Push();
		auto j = foocall();
		if (!j)
			return false;
		SelectedItem->children.push_back(j);
		UnselectAll(project->root);
		Build();
		j->Selected = 1;
		j->Select();
		Refresh2(L"PropertyItems");
		Refresh2(L"ViewCodeSample");
		Refresh2(L"dts");
		Refresh2(L"RootItems");
		Refresh2(L"PropertyTypeSelector");
		Refresh2(L"CanAddPanel");
		Refresh2(L"CanAddElement");
		return true;
	}


	bool MainPage::GenericAddPanel(std::function<std::shared_ptr<XITEM>()> foocall, bool TestOnly )
	{
		std::shared_ptr<XITEM> j;
		if (TestOnly)
		{
			if (!project)
				return true;
			if (!project->root)
				return true;
			auto SelectedItem = FirstSelectedItem();
			if (SelectedItem && std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem))
				return true;
			return false;
		}
		ForceCreateProject();
		auto SelectedItem = FirstSelectedItem();
		if (!project->root)
		{
			j = foocall();
			project->root = j;
			UnselectAll(project->root);
			project->root->Selected = true;
			project->root->Select();
		}
		else
		if (SelectedItem && std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem))
		{
			j = foocall();
			SelectedItem->children.push_back(j);
			UnselectAll(project->root);
		}
		else
		if (SelectedItem && ISXItemNavigationView(SelectedItem.get()))
		{
			j = foocall();
			SelectedItem->children.push_back(j);
			UnselectAll(project->root);
		}
		else
		{
			Message(txt(31).c_str());
			return false;
		}
		Build();
		if (j)
		{
			j->Select();
			j->Selected = 1;
		}
		Refresh2(L"PropertyItems");
		Refresh2(L"ViewCodeSample");
		Refresh2(L"dts");
		Refresh2(L"RootItems");
		Refresh2(L"PropertyTypeSelector");
		Refresh2(L"CanAddPanel");
		Refresh2(L"CanAddElement");

		if (TipsEnabled && Tip2 == 0)
		{
			Tip2 = 1;
			auto topnv = Content().as<Panel>();
			TTip(ttitle, s(69), topnv.FindName(L"xLine2").as<FrameworkElement>());
		}

		return true;
	}

	void MainPage::I_StackPanel(IInspectable const&, IInspectable const&)
	{
		GenericAddPanel(CreateXItemStackPanel);
	}

	void MainPage::I_SwapChainPanel(IInspectable const&, IInspectable const&)
	{
		GenericAddPanel(CreateXItemSwapChainPanel);
	}

	void MainPage::I_RelativePanel(IInspectable const&, IInspectable const&)
	{
		GenericAddPanel(CreateXItemRelativePanel);
	}

	void MainPage::I_Canvas(IInspectable const&, IInspectable const&)
	{
		GenericAddPanel(CreateXItemCanvas);
	}


	void MainPage::I_Grid(IInspectable const&, IInspectable const&)
	{
		GenericAddPanel(CreateXItemGrid);
	}


	bool MainPage::CanAddElement()
	{
		if (CanAddThis(0))
			return true;
		return false;
	}
	bool MainPage::CanAddPanel()
	{
		if (GenericAddPanel([]() -> std::shared_ptr<XITEM>
			{
				return nullptr;
			}, true))
			return true;
		return false;
	}

	bool MainPage::CanAddThis(bool ShowMsg)
	{
		auto SelectedItem = FirstSelectedItem();
		if (!project || !project->root || !SelectedItem)
		{
			if (ShowMsg == 0)
				return false;
			if (!project || !project->root)
				Message(txt(32).c_str());
			else
				Message(txt(31).c_str());
			return false;
		}
		if (std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem))
			return true;
		if (IsCommandBar(SelectedItem))
			return true;
		if (IsNavigationView(SelectedItem))
			return true;
		if (auto cc = std::dynamic_pointer_cast<XITEM_ContentControl>(SelectedItem))
		{
			if (cc->children.size() == 0)
				return true;
			return false;
		}
		if (ShowMsg)
			Message(txt(31).c_str());
		return false;
	}

	
	void MainPage::I_Button(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemButton);
	}

	void MainPage::I_Expander(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemExpander);
	}

	void MainPage::I_HLButton(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemHLButton);
	}

	void MainPage::I_DropDownButton(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemDropDownButton);	
	}
	void MainPage::I_AppBarButton(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemAppBarButton);
	}

	void MainPage::I_ColorPicker(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemColorPicker);
	}
	void MainPage::I_MapControl(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemMapControl);
	}
	void MainPage::I_AnimatedVisualPlayer(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemAnimatedVisualPlayer);
	}
	void MainPage::I_RadioButton(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemRadioButton);
	}

	void MainPage::I_ToggleSwitch(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemToggleSwitch);
	}

	void MainPage::I_RepeatButton(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemRepeatButton);
	}

	void MainPage::I_CheckBox(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemCheckBox);
	}

	void MainPage::I_CalendarDatePicker(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemCalendarDatePicker);
	}

	void MainPage::I_DatePicker(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemDatePicker);
	}

	void MainPage::I_TimePicker(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemTimePicker);
	}

	void MainPage::I_CalendarView(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemCalendarView);
	}

	void MainPage::I_TextBlock(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemTextBlock);
	}

	void MainPage::I_TextBox(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemTextBox);
	}

	void MainPage::I_RichEditBox(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemRichEditBox);
	}
	void MainPage::I_RichTextBlock(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemRichTextBlock);
	}

	void MainPage::I_NumberBox(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemNumberBox);
	}

	void MainPage::I_BreadcrumbBar(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemBreadcrumbBar);
	}

	void MainPage::I_NavigationView(IInspectable const&, IInspectable const&)
	{
		GenericAddPanel(CreateXItemNavigationView);
	}

	void MainPage::I_NavigationViewItem(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		auto SelectedItem = FirstSelectedItem();
		if (!ISXItemNavigationView(SelectedItem.get()))
		{
			Message(txt(67).c_str());
			return;
		}

		GenericAddItemUnder(CreateXItemPivot);
	}

	void MainPage::I_Pivot(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemPivot);	
	}

	void MainPage::I_MenuBar(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemMenuBar);
	}
	void MainPage::I_CommandBar(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemCommandBar);
	}


	void MainPage::I_PasswordBox(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemPasswordBox);
	}

	void MainPage::I_RatingControl(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemRatingControl);
	}

	void MainPage::I_Slider(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemSlider);
	}

	void MainPage::I_WebView2(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemWebView2);
	}

	void MainPage::I_Image(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemImage);
	}

	void MainPage::I_PersonPicture(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemPersonPicture);
	}

	void MainPage::I_MediaPlayerElement(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemMediaPlayerElement);
	}

	void MainPage::I_InfoBar(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemInfoBar);
	}

	void MainPage::I_InfoBadge(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemInfoBadge);
	}

	void MainPage::I_ProgressBar(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemProgressBar);
	}

	void MainPage::I_ProgressRing(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemProgressRing);
	}

	void MainPage::I_FlipView(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemFlipView);
	}

	void MainPage::I_GridView(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemGridView);
	}

	void MainPage::I_ListView(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemListView);
	}
	void MainPage::I_TreeView(IInspectable const&, IInspectable const&)
	{
		if (!CanAddThis())
			return;
		GenericAddItemUnder(CreateXItemTreeView);
	}

}



extern std::map<HWND, winrt::Windows::Foundation::IInspectable> windows;

void GetAllItems(std::shared_ptr<XITEM> r, std::vector<std::shared_ptr<XITEM>>& items)
{
	if (!r)
		return;
	items.push_back(r);
	for (auto& c : r->children)
	{
		GetAllItems(c, items);
	}
}

void UnselectAll(std::shared_ptr<XITEM> r)
{
	if (!r)
		return;

	r->Selected = 0;
	r->Unselect();
	for (auto& c : r->children)
	{
		UnselectAll(c);
	}
}

bool DeleteLoop(std::shared_ptr<XITEM> r, XITEM* xit)
{
	if (!r || !xit)
		return false;

	for (size_t i = 0 ; i < r->children.size() ; i++)
	{
		if (r->children[i].get() == xit)
		{
			r->children.erase(r->children.begin() + i);
			return true;
		}
		if (DeleteLoop(r->children[i], xit))
			return true;
	}
	return false;
}

bool SelectLoop(std::shared_ptr<XITEM> r, XITEM* xit)
{
	if (!r || !xit)
		return false;

	if (r.get() == xit)
	{
		r->Selected = 1;
		r->Select();
		return true;
	}

	for (auto& c : r->children)
	{
		if (SelectLoop(c, xit))
			return true;
	}
	return false;
}


void GenericTap(winrt::Windows::Foundation::IInspectable it)
{
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


		auto ui = it.as<winrt::Microsoft::UI::Xaml::FrameworkElement>();
		if (!ui)
			continue;

		auto Tag = winrt::unbox_value<long long>(ui.Tag());
		if (!Tag)
			continue;

		XITEM* ir = (XITEM*)Tag;
		if (!ir)
			continue;

		PROJECT* prj = (PROJECT*)main_page.ProjectPtr();
		if (!prj)
			continue;
		if (!prj->root)
			continue;
		UnselectAll(prj->root);

		if (SelectLoop(prj->root, ir))
		{
			main_page.Refresh2(L"PropertyItems");
			main_page.Refresh2(L"ViewCodeSample");
			main_page.Refresh2(L"dts");
			main_page.Refresh2(L"RootItems");
			main_page.Refresh2(L"CanAddPanel");
			main_page.Refresh2(L"CanAddElement");
			return;
		}

	}

}

void LoadMenuNames(std::vector<std::wstring>& names, XML3::XMLElement& mr)
{
	// Get the project
	names.clear();
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

		PROJECT* project = (PROJECT*)(long long)the_window.prjptr();
		if (project && project->xfile)
		{
			auto& m1 = project->xfile->GetRootElement()["Menus"];
			mr = m1;
			for (auto& el : m1)
			{
				names.push_back(el.vv("n").GetWideValue());
			}
		}
	}
}


void LoadPageNames(std::vector<std::wstring>& names, XML3::XMLElement& mr)
{
	// Get the project
	names.clear();
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

		PROJECT* project = (PROJECT*)(long long)the_window.prjptr();
		if (project && project->xfile)
		{
			auto& m1 = project->xfile->GetRootElement()["Pages"];
			mr = m1;
			for (auto& el : m1)
			{
				names.push_back(el.vv("n").GetWideValue());
			}
		}
	}
}

void MenuBarToXML(winrt::Microsoft::UI::Xaml::Controls::MenuBar& mb, winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem> curitems, winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItemBase> curitems2, XML3::XMLElement* root, XML3::XMLElement* curr)
{
	mb; curitems; root; curr; curitems; curitems2;
	if (!root || !curr)
		return;

	if (curitems)
	{
		for (auto item : curitems)
		{
			auto item2 = item.as<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem>();
			if (!item2)
				continue;
			auto& el = curr->AddElement("MenuBarItem");
			el.vv("Title").SetWideValue(item2.Title().c_str());
			if (item2.Items().Size() > 0)
			{	
				MenuBarToXML(mb, nullptr, item2.Items(), root, &el);
			}
		}
	}
	if (curitems2)
	{
		for (auto item : curitems2)
		{
			auto item2 = item.as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItemBase>();
			if (!item2)
				continue;
			if (auto mfs = item2.try_as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutSubItem>())
			{
				auto& el = curr->AddElement("MenuFlyoutSubItem");
				el.vv("Text").SetWideValue(mfs.Text().c_str());
				if (mfs.Items().Size() > 0)
				{
					MenuBarToXML(mb, nullptr, mfs.Items(), root, &el);
				}
			}
			else
			if (auto mfi = item2.try_as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem>())
			{
				auto& el = curr->AddElement("MenuFlyoutItem");
				el.vv("Text").SetWideValue(mfi.Text().c_str());
			}
			else
			if (auto mfi2 = item2.try_as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutSeparator>())
			{
				[[maybe_unused]] auto& el = curr->AddElement("MenuFlyoutSeparator");
			}
		}
	}

}


void DropDownButtonToXML(winrt::Microsoft::UI::Xaml::Controls::DropDownButton& mb, winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItemBase> curitems2, XML3::XMLElement* root, XML3::XMLElement* curr)
{
	mb; curitems2; root; curr; 
	if (!root || !curr)
		return;

	if (curitems2)
	{
		for (auto item : curitems2)
		{
			auto item2 = item.as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItemBase>();
			if (!item2)
				continue;
			if (auto mfs = item2.try_as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutSubItem>())
			{
				auto& el = curr->AddElement("MenuFlyoutSubItem");
				el.vv("Text").SetWideValue(mfs.Text().c_str());
				if (mfs.Items().Size() > 0)
				{
					DropDownButtonToXML(mb, mfs.Items(), root, &el);
				}
			}
			else
			if (auto mfi = item2.try_as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem>())
			{
				auto& el = curr->AddElement("MenuFlyoutItem");
				el.vv("Text").SetWideValue(mfi.Text().c_str());
			}
			else
			if (auto mfi2 = item2.try_as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutSeparator>())
			{
				[[maybe_unused]] auto& el = curr->AddElement("MenuFlyoutSeparator");
			}
		}
	}
}


UIElement BuildThisProject(PROJECT& prj)
{

	// Build this
	Panel rootsp = StackPanel();
	if (ISXItemStackPanel(prj.root.get()))
		rootsp = StackPanel();
	if (ISXItemGrid(prj.root.get()))
		rootsp = Grid();
	if (ISXItemRelativePanel(prj.root.get()))
		rootsp = RelativePanel();
	if (ISXItemCanvas(prj.root.get()))
		rootsp = Canvas();
	if (ISXItemSwapChainPanel(prj.root.get()))
		rootsp = SwapChainPanel();
#ifndef ROOT_NV_ENABLE
	if (ISXItemNavigationView(prj.root.get()))
		rootsp = Grid();
#endif

	winrt::VisualWinUI3::implementation::MainPage::Build(nullptr, rootsp, prj.root, MenuBarItem(), 0, nullptr, nullptr);
	return rootsp;
}


std::optional<XML3::XMLElement> FindPageByName(const std::wstring& name)
{
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
		auto main_page = fr.Content().as<winrt::VisualWinUI3::MainPage>();
		if (!main_page)
			continue;

		PROJECT* project = (PROJECT*)(long long)the_window.prjptr();
		if (project && project->xfile)
		{
			auto& m1 = project->xfile->GetRootElement()["Pages"];
			for (auto& mm : m1)
			{
				if (mm.vv("n").GetWideValue() == name)
				{
					return mm;
				}
			}
		}
	}
	return {};
}

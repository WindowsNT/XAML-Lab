#include "pch.h"
#include "MainPage.xaml.h"
#if __has_include("MainPage.g.cpp")
#include "MainPage.g.cpp"
#endif

#include "BlankWindow.g.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

bool RefreshFromGrid = 0;
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
		MessageBox((HWND)wnd(), L"Visual WinUI3 Designer\r\nCopyright Chourdakis Michael, 2025.", ttitle, 0);
		return;
	}
	void MainPage::OnGitHub(IInspectable const&, IInspectable const&)
	{
		ShellExecute(nullptr, L"open", L"https://github.com/WindowsNT/VisualWinUI3", nullptr, nullptr, SW_SHOWNORMAL);
		return;
	}


	void MainPage::OnNew(IInspectable const&, IInspectable const&)
	{
		winrt::VisualWinUI3::MainWindow CreateWi();
		CreateWi();

	}
	void MainPage::OnUndo(IInspectable const&, IInspectable const&)
	{
		Undo();
	}
	void MainPage::OnRedo(IInspectable const&, IInspectable const&)
	{
		Redo();
	}
	void MainPage::OnDelete(IInspectable const&, IInspectable const&)
	{
		if (!SelectedItem)
			return;

		Push();
		DeleteLoop(project->root, SelectedItem.get());	
		Build();
	}

	void MainPage::OnOpen(IInspectable const&, IInspectable const&)
	{
		OPENFILENAME of = { 0 };
		of.lStructSize = sizeof(of);
		of.hwndOwner = (HWND)0;
		of.lpstrFilter = L"*.vwui3\0*.vwui3\0\0";
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
				XML3::XML xx(project->file.c_str());
				project->Unser(*xx.GetRootElement().GetChildren()[0]);
				Build();
			}
		}

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
		DeleteFile(project->file.c_str());
		XML3::XML x(project->file.c_str());
		project->Ser(x.GetRootElement());
		x.Save();
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
		of.lpstrFilter = L"*.vwui3\0*.vwui3\0\0";
		std::vector<wchar_t> fnx(10000);
		of.lpstrFile = fnx.data();
		of.nMaxFile = 10000;
		of.lpstrDefExt = L"vwui3";
		of.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
		if (!GetSaveFileName(&of))
			return;
		project->file = fnx.data();
		if (project->file.empty())
			return;
		OnSave(nullptr, nullptr);
	}
 

	void MainPage::SelectClick(IInspectable const& c1, IInspectable const&)
	{
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

		if (!project)
			return;
		if (!project->root)
			return;
		UnselectAll(project->root);
		SelectLoop(project->root, xit);
		Refresh(L"PropertyItems");
		Refresh(L"PropertyTypeSelector");
	}

	void MainPage::ApplyTopProperties()
	{
		if (!project)
			return;
		if (!project->root)
			return;

		auto topnv = Content().as<Panel>();

		Panel rootsp = StackPanel();
		if (ISXItemStackPanel(project->root.get()))
			rootsp = topnv.FindName(L"PutRootSP").as<Panel>();
		if (ISXItemGrid(project->root.get()))
			rootsp = topnv.FindName(L"PutRootGR").as<Panel>();

		// Apply properties of the root to rootsp
		XITEM_Panel panel;
		panel.properties = project->root->properties;
		panel.X = rootsp;
		panel.ApplyProperties();

	}

	void MainPage::Build(winrt::VisualWinUI3::BlankWindow topbw,UIElement iroot, std::shared_ptr<XITEM> root, winrt::Windows::Foundation::IInspectable menu_root,int ForWhat, std::shared_ptr<XITEM> parentroot)
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
			auto ipanel = iroot.as<Panel>();
			if (!ipanel)
				return;
			auto chi = ipanel.Children();
			chi.Append(ni);
		}
		else
		if (topbw)
		{
			topbw.Content(ni);

		}

		int NextNumberAv = 1;
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
							SelectClick(c1, c2);
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
							SelectClick(c1, c2);
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
								SelectClick(c1, c2);
							});
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
								SelectClick(c1, c2);
							});
						mfs.Items().Append(item);
					}
				}
			}
		}
		for (auto& r : root->children)
		{
			Build(topbw,ni,r,item2,ForWhat,root);
		}
	}

	void MainPage::PageLoaded(IInspectable const&, IInspectable const&)
	{
		if (ToOpenFile.length())
		{
			if (!project)
				project = std::make_shared<PROJECT>();
			if (GetFileAttributes(ToOpenFile.c_str()) == 0xFFFFFFFF)
				return;
			project->file = ToOpenFile;
			ToOpenFile.clear();
			XML3::XML xx(project->file.c_str());
			project->Unser(*xx.GetRootElement().GetChildren()[0]);
			Build();
		}
	}

	void MainPage::Build()
	{
		auto topnv = Content().as<Panel>();

		if (!project)
			return;
		if (!project->root)
			return;

		Panel rootsp = StackPanel();
		if (ISXItemStackPanel(project->root.get()))
			rootsp = topnv.FindName(L"PutRootSP").as<Panel>();
		if (ISXItemGrid(project->root.get()))
			rootsp = topnv.FindName(L"PutRootGR").as<Panel>();
		rootsp.Children().Clear();
		auto mrs = topnv.FindName(L"MenuRootSelect").as<MenuBar>();

		mrs.Items().Clear();
		MenuBarItem mrsitem;
		mrsitem.Title(txt(23).c_str());
		mrs.Items().Append(mrsitem);
		Build(nullptr, rootsp, project->root, mrsitem, 0,nullptr);

		ApplyTopProperties();
		Refresh(L"PropertyItems");
		Refresh(L"PropertyTypeSelector");
	}

	

	winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector MainPage::PropertyTypeSelector()
	{
		auto m = VisualWinUI3::ItemSelector();
		return m;
	}

	winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::Item> MainPage::PropertyItems()
	{
		auto children = single_threaded_observable_vector<VisualWinUI3::Item>();
		if (!SelectedItem)
			return children;

		bool HasParentGrid = IsSomeParentGrid(SelectedItem->the_par);

		bool UseTree = 0;

		// Put the properties of this item
		std::wstring Current_Group;
		bool CGV = 1;
		for (size_t ip = 0 ; ip < SelectedItem->properties.size() ; ip++)
		{
			auto& pro = SelectedItem->properties[ip];

			if (pro->g == L"Parent Grid" && !HasParentGrid)
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

			auto int_type = std::dynamic_pointer_cast<INT_PROPERTY>(pro);
			if (int_type)
			{
				VisualWinUI3::Item item;
				item.PropertyX((long long)int_type.get());
				item.Type(PT_INT);
				item.Name1(pro->n);
//				item.Number0((double)int_type->value);
	//			item.Number1((double)int_type->mmin);
	//			item.Number2((double)int_type->mmax);
	//			item.Change1((double)int_type->smallchange);
	//			item.Change2((double)int_type->largechange);
				children.Append(item);
			}

			auto double_type = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(pro);
			if (double_type)
			{
				VisualWinUI3::Item item;
				item.PropertyX((long long)double_type.get());
				item.Type(PT_DOUBLE);
				item.Name1(pro->n);
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
				item.Name1(pro->n);
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
				for(auto& e : list_type->Items)
				{
					its.Append(e.c_str());
				}
				item.Name1(pro->n);
				children.Append(item);
			}
			auto string_type = std::dynamic_pointer_cast<STRING_PROPERTY>(pro);
			if (string_type)
			{
				VisualWinUI3::Item item;
				item.PropertyX((long long)string_type.get());
				item.Type(PT_STRING);
				item.Value0(string_type->value);
				item.Name1(pro->n);
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
				children.Append(item);
			}
		}

		return children;
	}


	void MainPage::E_RUN(IInspectable const&, IInspectable const&)
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
		Build(bw,nullptr, project->root, nullptr,1,nullptr);

		bw.Activate();
	}

	void MainPage::E_XAML(IInspectable const&, IInspectable const&)
	{
		if (!project)
			return;
		if (!project->root)
			return;

		auto el = project->root->SaveEl();
		auto s = el.Serialize();
		MessageBox((HWND)wnd(), XML3::XMLU(s.c_str()).wc(), ttitle, 0);
	}

	void MainPage::I_StackPanel(IInspectable const&, IInspectable const&)
	{
		if (!project)
			project = std::make_shared<PROJECT>();
		if (!project->root)
		{
			project->root = CreateXItemStackPanel();
			SelectedItem = project->root;
		}
		else
		if (SelectedItem && std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem))
		{
			auto j = CreateXItemStackPanel();
			SelectedItem->children.push_back(j);
			SelectedItem = j;
		}
		Build();
	}

	void MainPage::I_Grid(IInspectable const&, IInspectable const&)
	{
		if (!project)
			project = std::make_shared<PROJECT>();
		if (!project->root)
		{
			project->root = CreateXItemGrid();
			SelectedItem = project->root;
		}
		else
		if (SelectedItem && std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem))
		{
			auto j = CreateXItemGrid();
			SelectedItem->children.push_back(j);
			SelectedItem = j;
		}
		Build();
	}

	void MainPage::I_Button(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}
		auto j = CreateXItemButton();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_HLButton(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemHLButton();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_ToggleButton(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemToggleButton();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_CheckBox(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemCheckBox();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_CalendarDatePicker(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemCalendarDatePicker();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_DatePicker(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemDatePicker();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_TimePicker(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemTimePicker();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_CalendarView(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemCalendarView();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_TextBlock(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemTextBlock();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_TextBox(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemTextBox();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_RatingControl(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemRatingControl();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_Slider(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemSlider();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_WebView2(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemWebView2();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_Image(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemImage();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_PersonPicture(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemPersonPicture();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_MediaPlayerElement(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemMediaPlayerElement();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_InfoBar(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemInfoBar();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_ProgressBar(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemProgressBar();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_ProgressRing(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemProgressRing();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_FlipView(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemFlipView();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

	void MainPage::I_GridView(IInspectable const&, IInspectable const&)
	{
		if (!project || !project->root || !SelectedItem || std::dynamic_pointer_cast<XITEM_Panel>(SelectedItem) == nullptr)
		{
			MessageBox((HWND)wnd(), txt(31).c_str(), ttitle, 0);
			return;
		}

		auto j = CreateXItemGridView();
		SelectedItem->children.push_back(j);
		SelectedItem = j;
		Build();
	}

}



extern std::map<HWND, winrt::Windows::Foundation::IInspectable> windows;

void UnselectAll(std::shared_ptr<XITEM> r)
{
	if (!r)
		return;

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
		SelectedItem = r;
		SelectedItem->Select();
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
			return;
		}

	}

}

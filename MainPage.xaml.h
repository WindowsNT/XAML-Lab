#pragma once

#include "MainPage.g.h"
#include "BlankWindow.g.h"
#include "Item.h"
#include "project.h"

namespace winrt::VisualWinUI3::implementation
{

   
    struct MainPage : MainPageT<MainPage>
    {
        std::shared_ptr<PROJECT> project;
        std::stack<XML3::XMLElement> undo, redo;

        long long ProjectPtr()
        {
            if (!project)
                return 0;
            return (long long)project.get();
        }

        void ForceCreateProject();

        void Push()
        {
            if (!project || !project->root)
                return;
            XML3::XMLElement e;
            project->root->Ser(e);
            undo.push(e);
        }
        void Undo()
        {
            if (undo.empty())
                return;
            XML3::XMLElement e;
            project->root->Ser(e);
            redo.push(e);
            project->root->Unser(undo.top());
            undo.pop();
            Build();
			Refresh();
        }
        void Redo()
        {
            if (redo.empty())
                return;
            XML3::XMLElement e;
            project->root->Ser(e);
            undo.push(e);
            project->root->Unser(redo.top());
            redo.pop();
            Build();
            Refresh();
        }

        MainPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
        void PageLoaded(IInspectable const&, IInspectable const&);
        void SelectClick(IInspectable, IInspectable);
        void RDBSC(IInspectable, IInspectable);
        void SwitchPropertyMode(int nm);
               

        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
        {
            return m_propertyChanged.add(handler);
        }
        void PropertyChanged(winrt::event_token const& token) noexcept
        {
            m_propertyChanged.remove(token);
        }
        static winrt::hstring txt(long jx)
        {
            return s(jx);
        }

        std::wstring _CurrentFilter;

        void CurrentFilter(winrt::hstring j)
        {
            if (_CurrentFilter == j)
				return; // No change
            _CurrentFilter = j;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"CurrentFilter"));
            Refresh(L"PropertyItems");
        }
        winrt::hstring CurrentFilter()
        {
            return _CurrentFilter.c_str();
        }

		bool _ViewProperties = true;
        bool ViewProperties()
        {
            return _ViewProperties;
		}
        void ViewProperties(bool v)
        {
            if (_ViewProperties == v)
                return; // No change
            _ViewProperties = v;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ViewProperties"));
		}
		bool _ViewElements = true;
        bool ViewElements()
        {
            return _ViewElements;
        }
        void ViewElements(bool v)
        {
            if (_ViewElements == v)
                return; // No change
            _ViewElements = v;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ViewElements"));
		}

        bool _ViewPanels = true;
        bool ViewPanels()
        {
            return _ViewPanels;
        }
        void ViewPanels(bool v)
        {
            if (_ViewPanels == v)
                return; // No change
            _ViewPanels = v;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ViewPanels"));
        }

        bool dirty = false;
        bool Dirty()
        {
            return dirty;
		}
        void Dirty(bool d)
        {
            dirty = d;
        }



		long long _wnd = 0;
        long long wnd()
        {
            return _wnd;
        }

		void wnd(long long value)
		{
			_wnd = value;
		}

		std::wstring _i1, _i2, _i3;
        winrt::hstring i1()
        {
            return _i1.c_str();
		}
        void i1(winrt::hstring value)
        {
            if (_i1 == value)
                return; // No change
            _i1 = value;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"i1"));
        }
        winrt::hstring i2()
        {
            return _i2.c_str();
		}
        void i2(winrt::hstring value)
        {
            if (_i2 == value)
                return; // No change
            _i2 = value;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"i2"));
		}
        winrt::hstring i3()
        {
			return _i3.c_str();
		}
        void i3(winrt::hstring value)
        {
            if (_i3 == value)
                return; // No change
            _i3 = value;
			m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"i3"));
		}

        FileSystemTemplateSelector selector;
        bool SelectorLoaded = false;

        winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector dts()
        {
            if (!SelectorLoaded)
            {
                SelectorLoaded = true;
                auto top = Content().as<Controls::Panel>();
                selector.LoadTemplates(top);
            }
            return selector;
        }

        std::shared_ptr<XML3::XMLElement> root_for_tree;
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::FileSystemItem> RootItems();
        void LeftTree(IInspectable const& r1, [[maybe_unused]] winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const& r2);


        bool TipsEnabled = 0;
        bool TTThemeSet = 0;
        int Tip1 = 0;
        int Tip2 = 0;
        bool CanAddElement();
		bool CanAddPanel();
        long long FirstSelectedItemPtr();
        std::shared_ptr<XITEM> FirstSelectedItem();
        std::vector<std::shared_ptr<XITEM>> AllSelectedItems();
        void Message(const wchar_t* s);
        void TTip(const wchar_t* t,const wchar_t* s,FrameworkElement trg = nullptr);
        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
        void Refresh(std::vector<std::wstring> strs);
        void Refresh(const wchar_t* s = L"");
        void Refresh2(winrt::hstring);
        void OnViewElements(IInspectable const&, IInspectable const&);
        void OnViewProperties(IInspectable const&, IInspectable const&);
        void OnUndo(IInspectable const&, IInspectable const&);
        void OnRedo(IInspectable const&, IInspectable const&);
        void OnDelete(IInspectable const&, IInspectable const&);
        void OnClear(IInspectable const&, IInspectable const&);
        void OnSelectAll(IInspectable const&, IInspectable const&);
        void OnUnselectAll(IInspectable const&, IInspectable const&);
        void OnNew(IInspectable const&, IInspectable const&);
        void OnOpen(IInspectable const&, IInspectable const&);
        void Save();
        void OnSave(IInspectable const&, IInspectable const&);

        bool PrimaryOK = 0;
        void Input_OK(IInspectable, IInspectable)
        {
            PrimaryOK = 1;
        }
        void ReloadMenuList();

        void OnPageSave(IInspectable const&, IInspectable const&);
        void OnExit(IInspectable const&, IInspectable const&);
        void OnSaveAs(IInspectable const&, IInspectable const&);
        void OnAbout(IInspectable const&, IInspectable const&);
        void OnGitHub(IInspectable const&, IInspectable const&);

        void OnTemplate0(const char* t);
        void OnTemplate1(IInspectable const&, IInspectable const&);
        void OnTemplate2(IInspectable const&, IInspectable const&);
        void OnTemplate3(IInspectable const&, IInspectable const&);
        void OnTemplate4(IInspectable const&, IInspectable const&);
        void OnTemplate5(IInspectable const&, IInspectable const&);
        void OnTemplate6(IInspectable const&, IInspectable const&);
        
        void ListViewPick(IInspectable const&, IInspectable const&);
        void ListViewPick1(IInspectable const&, IInspectable const&);

		void ApplyTopProperties();
        void Build();
        static void Build(winrt::VisualWinUI3::BlankWindow topbw,winrt::Microsoft::UI::Xaml::UIElement iroot,std::shared_ptr<XITEM> root,winrt::Windows::Foundation::IInspectable menu_root,int ForWhat, std::shared_ptr<XITEM> parentroot, winrt::VisualWinUI3::MainPage ptx);
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::Item> PropertyItems();
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::Item> Elements();
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::Item> Panels();
		winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector PropertyTypeSelector();

        void AskSaveChanges();
        void Message3_1(IInspectable const&, IInspectable const&);
        void Message3_2(IInspectable const&, IInspectable const&);
        void Message3_3(IInspectable const&, IInspectable const&);

        bool CanAddThis(bool ShowMsg = 1);
        bool GenericAddItemUnder(std::function<std::shared_ptr<XITEM>()> foocall);
        bool GenericAddPanel(std::function<std::shared_ptr<XITEM>()> foocall,bool TestOnly = 0);


		long long _PropertyItemsMode = 0;
        long long PropertyItemsMode()
        {
            return _PropertyItemsMode;
        }
        void PropertyItemsMode(long long v)
        {
            if (_PropertyItemsMode == v)
                return; // No change
            _PropertyItemsMode = v;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"PropertyItemsMode"));
		}

		bool _SeeTreeView = true;
        bool SeeTreeView()
        {
            return _SeeTreeView;
		}
        void SeeTreeView(bool v)
        {
            if (_SeeTreeView == v)
                return; // No change
            _SeeTreeView = v;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"SeeTreeView"));
		}
		bool _SeeSelectButton = false;
        bool SeeSelectButton()
        {
            return _SeeSelectButton;
        }
        void SeeSelectButton(bool v)
        {
            if (_SeeSelectButton == v)
                return; // No change
            _SeeSelectButton = v;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"SeeSelectButton"));
		}
        void OnViewRootSP1(IInspectable const&, IInspectable const&)
        {
            SeeTreeView(true);
            SeeSelectButton(false);
        }
        void OnViewRootSP2(IInspectable const&, IInspectable const&)
        {
            SeeTreeView(false);
            SeeSelectButton(true);

        }

        void I_StackPanel(IInspectable const&, IInspectable const&);
        void I_SwapChainPanel(IInspectable const&, IInspectable const&);
        void I_RelativePanel(IInspectable const&, IInspectable const&);
        void I_Canvas(IInspectable const&, IInspectable const&);
        void I_Grid(IInspectable const&, IInspectable const&);
        void I_Button(IInspectable const&, IInspectable const&);
        void I_Expander(IInspectable const&, IInspectable const&);
        void I_HLButton(IInspectable const&, IInspectable const&);
        void I_AppBarButton(IInspectable const&, IInspectable const&);
        void I_DropDownButton(IInspectable const&, IInspectable const&);
        void I_ColorPicker(IInspectable const&, IInspectable const&);
        void I_MapControl(IInspectable const&, IInspectable const&);
        void I_AnimatedVisualPlayer(IInspectable const&, IInspectable const&);
        void I_RadioButton(IInspectable const&, IInspectable const&);
        void I_ToggleSwitch(IInspectable const&, IInspectable const&);
        void I_RepeatButton(IInspectable const&, IInspectable const&);
        void I_CheckBox(IInspectable const&, IInspectable const&);
        void I_CalendarDatePicker(IInspectable const&, IInspectable const&);
        void I_DatePicker(IInspectable const&, IInspectable const&);
        void I_TimePicker(IInspectable const&, IInspectable const&);
        void I_CalendarView(IInspectable const&, IInspectable const&);
        void I_TextBlock(IInspectable const&, IInspectable const&);
        void I_TextBox(IInspectable const&, IInspectable const&);
        void I_RichEditBox(IInspectable const&, IInspectable const&);
        void I_RichTextBlock(IInspectable const&, IInspectable const&);
        void I_NumberBox(IInspectable const&, IInspectable const&);
        void I_PasswordBox(IInspectable const&, IInspectable const&);
        void I_BreadcrumbBar(IInspectable const&, IInspectable const&);
        void I_NavigationView(IInspectable const&, IInspectable const&);
        void I_NavigationViewItem(IInspectable const&, IInspectable const&);
        void I_Pivot(IInspectable const&, IInspectable const&);
        void I_MenuBar(IInspectable const&, IInspectable const&);
        void I_CommandBar(IInspectable const&, IInspectable const&);
        void I_RatingControl(IInspectable const&, IInspectable const&);
        void I_Slider(IInspectable const&, IInspectable const&);
        void I_WebView2(IInspectable const&, IInspectable const&);
        void I_Image(IInspectable const&, IInspectable const&);
        void I_PersonPicture(IInspectable const&, IInspectable const&);
        void I_MediaPlayerElement(IInspectable const&, IInspectable const&);
        void I_InfoBar(IInspectable const&, IInspectable const&);
        void I_InfoBadge(IInspectable const&, IInspectable const&);
        void I_ProgressBar(IInspectable const&, IInspectable const&);
        void I_ProgressRing(IInspectable const&, IInspectable const&);
        void I_FlipView(IInspectable const&, IInspectable const&);
        void I_GridView(IInspectable const&, IInspectable const&);
        void I_ListView(IInspectable const&, IInspectable const&);
        void I_ListBox(IInspectable const&, IInspectable const&);
        void I_ComboBox(IInspectable const&, IInspectable const&);
        void I_TreeView(IInspectable const&, IInspectable const&);
        void E_XAML(IInspectable const&, IInspectable const&);
        void E_CODE(IInspectable const&, IInspectable const&);
        void E_RUN1(IInspectable const&, IInspectable const&);
        void E_RUN2(IInspectable const&, IInspectable const&);
        void E_RUN3(IInspectable const&, IInspectable const&);

    };
}

namespace winrt::VisualWinUI3::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}

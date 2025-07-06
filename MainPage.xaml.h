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
        void SelectClick(IInspectable const&, IInspectable const&);


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

		long long _wnd = 0;
        long long wnd()
        {
            return _wnd;
        }

		void wnd(long long value)
		{
			_wnd = value;
		}

        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
        void Refresh(std::vector<std::wstring> strs);
        void Refresh(const wchar_t* s = L"");
        void Refresh2(winrt::hstring);
        void OnUndo(IInspectable const&, IInspectable const&);
        void OnRedo(IInspectable const&, IInspectable const&);
        void OnDelete(IInspectable const&, IInspectable const&);
        void OnNew(IInspectable const&, IInspectable const&);
        void OnOpen(IInspectable const&, IInspectable const&);
        void OnSave(IInspectable const&, IInspectable const&);
        void OnExit(IInspectable const&, IInspectable const&);
        void OnSaveAs(IInspectable const&, IInspectable const&);
        void OnAbout(IInspectable const&, IInspectable const&);
        void OnGitHub(IInspectable const&, IInspectable const&);

		void ApplyTopProperties();
        void Build();
        void Build(winrt::VisualWinUI3::BlankWindow topbw,winrt::Microsoft::UI::Xaml::UIElement iroot,std::shared_ptr<XITEM> root,winrt::Windows::Foundation::IInspectable menu_root,int ForWhat, std::shared_ptr<XITEM> parentroot);
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::Item> PropertyItems();
		winrt::Microsoft::UI::Xaml::Controls::DataTemplateSelector PropertyTypeSelector();

        void I_StackPanel(IInspectable const&, IInspectable const&);
        void I_Grid(IInspectable const&, IInspectable const&);
        void I_Button(IInspectable const&, IInspectable const&);
        void I_HLButton(IInspectable const&, IInspectable const&);
        void I_ToggleButton(IInspectable const&, IInspectable const&);
        void I_CheckBox(IInspectable const&, IInspectable const&);
        void I_CalendarDatePicker(IInspectable const&, IInspectable const&);
        void I_DatePicker(IInspectable const&, IInspectable const&);
        void I_TimePicker(IInspectable const&, IInspectable const&);
        void I_CalendarView(IInspectable const&, IInspectable const&);
        void I_TextBlock(IInspectable const&, IInspectable const&);
        void I_TextBox(IInspectable const&, IInspectable const&);
        void I_RatingControl(IInspectable const&, IInspectable const&);
        void I_Slider(IInspectable const&, IInspectable const&);
        void I_WebView2(IInspectable const&, IInspectable const&);
        void I_Image(IInspectable const&, IInspectable const&);
        void I_PersonPicture(IInspectable const&, IInspectable const&);
        void I_MediaPlayerElement(IInspectable const&, IInspectable const&);
        void I_InfoBar(IInspectable const&, IInspectable const&);
        void I_ProgressBar(IInspectable const&, IInspectable const&);
        void I_ProgressRing(IInspectable const&, IInspectable const&);
        void I_FlipView(IInspectable const&, IInspectable const&);
        void I_GridView(IInspectable const&, IInspectable const&);
        void E_XAML(IInspectable const&, IInspectable const&);
        void E_RUN(IInspectable const&, IInspectable const&);

    };
}

namespace winrt::VisualWinUI3::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}

#pragma once

#include "MainWindow.g.h"

namespace winrt::VisualWinUI3::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
      
        void OnDark(IInspectable const&, IInspectable const&);
        void OnLight(IInspectable const&, IInspectable const&);
        void OnDefault(IInspectable const&, IInspectable const&); 
        void OnExit(IInspectable const&, IInspectable const&);
        void OnSaveAs(IInspectable const&, IInspectable const&);
        void NVI_ChangeLightTapped(IInspectable const&, IInspectable const&);
        MainWindow()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        void OnLoad(IInspectable, IInspectable);
        void ItemInvoked(IInspectable, winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs);
        static winrt::hstring txt(long jx)
        {
            return s(jx);
        }


        long long _wnd = 0, _prjptr = 0;
        long long wnd()
        {
            return _wnd;
        }
        void wnd(long long value)
        {
            _wnd = value;
        }
        long long prjptr()
        {
            return _prjptr;
		}
        void prjptr(long long value)
        {
            _prjptr = value;
        }

    };
}

namespace winrt::VisualWinUI3::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}

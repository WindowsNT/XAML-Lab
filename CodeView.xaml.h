#pragma once

#include "CodeView.g.h"

namespace winrt::VisualWinUI3::implementation
{
    struct CodeView : CodeViewT<CodeView>
    {
        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
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


        void CopyCode(IInspectable const&, IInspectable const&);

        CodeView()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

		std::wstring _t_xaml;
        winrt::hstring t_xaml()
        {
            return _t_xaml.c_str();
        }
        void t_xaml(winrt::hstring value)
        {
            if (_t_xaml == value)
                return; // No change
            _t_xaml = value;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"t_xaml"));
		}

        std::wstring _t_idl;
        winrt::hstring t_idl()
        {
            return _t_idl.c_str();
		}
        void t_idl(winrt::hstring value)
        {
            if (_t_idl == value)
                return; // No change
            _t_idl = value;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"t_idl"));
        }


        std::wstring _t_h;
        std::wstring _t_cpp;
        winrt::hstring t_h()
        {
            return _t_h.c_str();
        }
        winrt::hstring t_cpp()
        {
            return _t_cpp.c_str();
        }
        void t_h(winrt::hstring value)
        {
            if (_t_h == value)
                return; // No change
            _t_h = value;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"t_h"));
		}
        void t_cpp(winrt::hstring value)
        {
            if (_t_cpp == value)
                return; // No change
            _t_cpp = value;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"t_cpp"));
		}



		void LVC(IInspectable lv, IInspectable);

		int _selected_index = 0;
        void PopulateRB();

    };
}

namespace winrt::VisualWinUI3::factory_implementation
{
    struct CodeView : CodeViewT<CodeView, implementation::CodeView>
    {
    };
}

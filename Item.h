#pragma once
#include "Item.g.h"

namespace winrt::VisualWinUI3::implementation
{
    struct Item : ItemT<Item>
    {
        Item()
        {

        }
        long long _PropertyX = 0;
        int _type = 0;
        long _i0 = 0;
        double _d0 = 0.0;
        double _d1 = 0.0;
		double _d2 = 0.0;
        std::wstring _n1;
        std::wstring _n2;
        std::wstring _n3;
        std::wstring _n4;
        std::wstring _v0;

        // xitems
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> _xitems = single_threaded_observable_vector<winrt::hstring>();
        void xitems(winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> v)
        {
            _xitems = v;
		}
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::hstring> xitems()
        {
            return _xitems;
		}

		std::vector<VisualWinUI3::Item> _children;
        winrt::Windows::Foundation::Collections::IObservableVector<VisualWinUI3::Item> TVChildren();

        hstring Name1()
        {
            return _n1.c_str();
        }
        hstring Name2()
        {
            return _n2.c_str();
        }
        hstring Name3()
        {
            return _n3.c_str();
        }
        hstring Name4()
        {
            return _n4.c_str();
        }
        int Type()
        {
            return _type;
		}
        double Number0()
        {
            return _d0;
		}
        double Number1()
        {
            return _d1;
		}
        double Number2()
        {
            return _d2;
        }
        long long PropertyX()
        {
            return _PropertyX;
		}
     


        bool _boolean0 = false;
        bool Boolean0()
        {
            return _boolean0;
        }
        void Boolean0(bool b);


		double _ch1 = 1.0, _ch2 = 1.0;
        void Change1(double n)
        {
            if (_ch1 == n)
                return;
            _ch1 = n;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"Change1"));
		}
        void Change2(double n)
        {
            if (_ch2 == n)
				return;
            _ch2 = n;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"Change2"));
        }
        double Change1()
        {
            return _ch1;
        }
        double Change2()
        {
            return _ch2;
		}   

        void Int0(long n);
        void Value0(hstring n);

        IInspectable _II;
        void II(IInspectable j)
        {
            _II = j;
        }
        IInspectable II()
        {
            return _II;
        }

        winrt::hstring ToString() 
        {
            return Name1();
        }

        void Name1(hstring n)
        {
            _n1 = n.c_str();
        }

        void Name2(hstring n)
        {
            _n2 = n.c_str();
        }
        void Name3(hstring n)
        {
            _n3 = n.c_str();
        }
        void Name4(hstring n)
        {
            _n4 = n.c_str();
        }


        winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage _bitmap1;
        void Bitmap1(winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage b)
        {
			if (_bitmap1 == b)
                return;
			_bitmap1 = b;
			m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"Bitmap1"));
		}
        winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage Bitmap1()
        {
			return _bitmap1;
		}


        std::wstring _tip;
        bool TipVisible()
        {
			return !_tip.empty();
        }
        void Tip(winrt::hstring t)
        {
            if (_tip == t)
				return;
            _tip = t;
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"Tip"));
        }
        winrt::hstring Tip()
        {
            return _tip.c_str();
		}

        void Type(int t)
        {
            _type = t;
		}
        void Number0(double n);
        long Int0()
        {
            return _i0;
		}
        winrt::hstring Value0()
        {
            return _v0.c_str();
		}

        void Number1(double n)
        {
            _d1 = n;
        }
        void Number2(double n)
        {
            _d2 = n;
		}
        void PropertyX(long long n)
        {
            _PropertyX = n;
        }

        bool _sel = 0;
        bool Sel()
        {
            return _sel;
        }
        void Sel(bool s);


        winrt::Windows::UI::Color _color;

        winrt::Windows::UI::Color Color0()
        {
            return _color;
		}
        void Color0(winrt::Windows::UI::Color c);

        winrt::Microsoft::UI::Xaml::Media::Brush ColorX()
        {
            auto col = winrt::Windows::UI::Color();
            col.A = 100;
            col.R = 255;
            if (Sel())
                col.A = 255;
            auto sb = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush();
            sb.Color(col);
            return sb;
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


        winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::Item> ComboItems();

    };
}
namespace winrt::VisualWinUI3::factory_implementation
{
    struct Item : ItemT<Item, implementation::Item>
    {
    };
}

#include "pch.h"
#include "Item.h"
#include "Item.g.cpp"
#include "MainWindow.xaml.h"
#include "property.hpp"

extern std::map<HWND, winrt::Windows::Foundation::IInspectable> windows;

namespace winrt::VisualWinUI3::implementation
{

    winrt::Windows::Foundation::Collections::IObservableVector<VisualWinUI3::Item> Item::TVChildren()
    {
        auto children = single_threaded_observable_vector<VisualWinUI3::Item>();
        for (auto pro2 : _children)
        {
            VisualWinUI3::Item item;
			children.Append(item);
        }
        return children;
    }

    winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::Item> Item::ComboItems()
    {
        auto children = single_threaded_observable_vector<VisualWinUI3::Item>();
        for(auto e : _xitems)
        {
            VisualWinUI3::Item item;
            item.Name1(e);
            children.Append(item);
		}
        return children;
    }

    void Item::Int0(long n)
    {
        _i0 = n;

        // Apply
        for (auto& wi : windows)
        {
			auto main_page = PageFromWindow(wi.second).try_as<VisualWinUI3::MainPage>();
            if (!main_page)
                continue;
            // Find the item
			XITEM* SelectedItem = (XITEM*)main_page.FirstSelectedItemPtr();
            if (!SelectedItem)
                continue;

            for (auto& props : SelectedItem->properties)
            {
                if ((long long)props.get() == _PropertyX)
                {
                    auto list_type = std::dynamic_pointer_cast<LIST_PROPERTY>(props);
                    if (list_type)
                    {
                        if (list_type->SelectedIndex == _i0)
                            return; // No change
                        list_type->SelectedIndex = _i0;

                    }
                    break;
                }
            }

            SelectedItem->ApplyProperties();
            main_page.ApplyTopProperties();

        }

    }

    void Item::Color0(winrt::Windows::UI::Color c)
    {
        _color = c;

        // Apply
        for (auto& wi : windows)
        {
            auto main_page = PageFromWindow(wi.second).try_as<VisualWinUI3::MainPage>();
            if (!main_page)
                continue;

            // Find the item
            XITEM* SelectedItem = (XITEM*)main_page.FirstSelectedItemPtr();
            if (!SelectedItem)
                continue;

            for (auto& props : SelectedItem->properties)
            {
                if ((long long)props.get() == _PropertyX)
                {
                    auto list_type = std::dynamic_pointer_cast<COLOR_PROPERTY>(props);
                    if (list_type)
                    {
                        if (list_type->value == _color)
                            return; // No change
                        list_type->value = _color;

                    }
                    break;
                }
            }

            SelectedItem->ApplyProperties();
            main_page.ApplyTopProperties();

        }


    }


    void Item::Boolean0(bool b)
    {
        _boolean0 = b;

        // Apply
        for (auto& wi : windows)
        {
            auto main_page = PageFromWindow(wi.second).try_as<VisualWinUI3::MainPage>();
            if (!main_page)
                continue;

            // Find the item
            XITEM* SelectedItem = (XITEM*)main_page.FirstSelectedItemPtr();
            if (!SelectedItem)
                continue;

            for (auto& props : SelectedItem->properties)
            {
                if ((long long)props.get() == _PropertyX)
                {
                    auto list_type = std::dynamic_pointer_cast<BOOL_PROPERTY>(props);
                    if (list_type)
                    {
                        if (list_type->SelectedIndex == (int)_boolean0)
                            return; // No change
                        list_type->SelectedIndex = _boolean0;

                    }
                    break;
                }
            }

            SelectedItem->ApplyProperties();
            main_page.ApplyTopProperties();
        }
    }

    void Item::Value0(winrt::hstring n)
    {
        _v0 = n;

        // Apply
        for (auto& wi : windows)
        {
            auto main_page = PageFromWindow(wi.second).try_as<VisualWinUI3::MainPage>();
            if (!main_page)
                continue;

            // Find the item
            XITEM* SelectedItem = (XITEM*)main_page.FirstSelectedItemPtr();
            if (!SelectedItem)
                continue;

            for (auto& props : SelectedItem->properties)
            {
                if ((long long)props.get() == _PropertyX)
                {
                    auto fu = std::dynamic_pointer_cast<FUNCTION_PROPERTY>(props);
                    if (fu)
                    {
                        if (fu->value == _v0)
                            return; // No change
                        fu->value = _v0;

                    }

                    if (main_page.PropertyItemsMode() == 1)
                    {
                        if (props->bindv == _v0)
                            return;
						// No change
                        props->bindv = _v0;
                        continue;
                    }

                    auto list_type = std::dynamic_pointer_cast<STRING_PROPERTY>(props);
                    if (list_type)
                    {
                        auto& what = list_type->value;
                        if (what == _v0)
                            return; // No change
                        what = _v0;

                    }
                    break;
                }
            }

            if (main_page.PropertyItemsMode() == 0)
            {
                SelectedItem->ApplyProperties();
                main_page.ApplyTopProperties();
            }
        }

    }

    void Item::Sel(bool s)
    {
        _sel = s;

        // Apply
        for (auto& wi : windows)
        {
            auto main_page = PageFromWindow(wi.second).try_as<VisualWinUI3::MainPage>();
            if (!main_page)
                continue;

            // Find the item
            XITEM* SelectedItem = (XITEM*)main_page.FirstSelectedItemPtr();
            if (!SelectedItem)
                continue;

            for (auto& props : SelectedItem->properties)
            {
                if ((long long)props.get() == _PropertyX)
                {
                    if (props->S == s)
						return; // No change
                    props->S = s;
                    main_page.Refresh2(L"PropertyItems");
                    return;
                }
            }

        }
    }


    void Item::Number0(double n)
    {
        _d0 = n;

        // Apply
        for (auto& wi : windows)
        {
            auto main_page = PageFromWindow(wi.second).try_as<VisualWinUI3::MainPage>();
            if (!main_page)
                continue;

            // Find the item
            XITEM* SelectedItem = (XITEM*)main_page.FirstSelectedItemPtr();
            if (!SelectedItem)
                continue;

            for (auto& props : SelectedItem->properties)
            {
                if ((long long)props.get() == _PropertyX)
                {
					auto double_type = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(props);
                    if (double_type)
                    {
                        if (double_type->value == _d0)
							return; // No change
                        double_type->value = _d0;

                    }
                    break;
                }
            }

			SelectedItem->ApplyProperties();
            main_page.ApplyTopProperties();
        }
        
    }

}



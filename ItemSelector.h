#pragma once
#include "ItemSelector.g.h"
#include "Item.h"
#include "MainPage.xaml.h"
#include "property.hpp"
extern std::map<HWND, winrt::Windows::Foundation::IInspectable> windows;

namespace winrt::VisualWinUI3::implementation
{
    struct ItemSelector : ItemSelectorT<ItemSelector>
    {
        ItemSelector()
        {

        }

        virtual winrt::Microsoft::UI::Xaml::DataTemplate SelectTemplateCore(winrt::Windows::Foundation::IInspectable const& item, winrt::Microsoft::UI::Xaml::DependencyObject const&) const
        {
            return SelectTemplateCore(item);
        }
        virtual winrt::Microsoft::UI::Xaml::DataTemplate SelectTemplateCore(winrt::Windows::Foundation::IInspectable const& item) const
        {
			auto it = item.try_as<winrt::VisualWinUI3::Item>();
            if  (!it)
                return nullptr;
            
            for (auto& w : windows)
            {
                auto root = w.second.as<winrt::VisualWinUI3::MainWindow>();
				auto top = root.Content().as<winrt::Microsoft::UI::Xaml::Controls::NavigationView>();

                if (it.TVChildren().Size() > 0)
                {
                    auto r_tree = top.Resources().TryLookup(winrt::box_value(L"TreeHeaderTemplate"));
                    if (r_tree)
                    {
                        auto template1 = r_tree.try_as<winrt::Microsoft::UI::Xaml::DataTemplate>();
                        if (template1)
                        {
                            return template1;
                        }
                    }
				}
                if (it.Type() == PT_HEADER)
                {
                    auto r_double = top.Resources().TryLookup(winrt::box_value(L"HeaderTemplate"));
                    if (r_double)
                    {
                        auto template1 = r_double.try_as<winrt::Microsoft::UI::Xaml::DataTemplate>();
                        if (template1)
                        {
                            return template1;
                        }
                    }
                }
                if (it.Type() == PT_DOUBLE)
                {
                    auto r_double = top.Resources().TryLookup(winrt::box_value(L"DoubleTemplate"));
                    if (r_double)
                    {
                        auto template1 = r_double.try_as<winrt::Microsoft::UI::Xaml::DataTemplate>();
                        if (template1)
                        {
                            return template1;
                        }
                    }
                }
                if (it.Type() == PT_INT)
                {
                    auto r_double = top.Resources().TryLookup(winrt::box_value(L"IntTemplate"));
                    if (r_double)
                    {
                        auto template1 = r_double.try_as<winrt::Microsoft::UI::Xaml::DataTemplate>();
                        if (template1)
                        {
                            return template1;
                        }
                    }
                }
                if (it.Type() == PT_STRING)
                {
                    auto r_string = top.Resources().TryLookup(winrt::box_value(L"StringTemplate"));
                    if (r_string)
                    {
                        auto template1 = r_string.try_as<winrt::Microsoft::UI::Xaml::DataTemplate>();
                        if (template1)
                        {
                            return template1;
                        }
                    }
                }
                if (it.Type() == PT_FUNCTION)
                {
                    auto r_string = top.Resources().TryLookup(winrt::box_value(L"FunctionTemplate"));
                    if (r_string)
                    {
                        auto template1 = r_string.try_as<winrt::Microsoft::UI::Xaml::DataTemplate>();
                        if (template1)
                        {
                            return template1;
                        }
                    }
                }
                if (it.Type() == PT_GROUP)
                {
                    auto r_string = top.Resources().TryLookup(winrt::box_value(L"GroupTemplate"));
                    if (r_string)
                    {
                        auto template1 = r_string.try_as<winrt::Microsoft::UI::Xaml::DataTemplate>();
                        if (template1)
                        {
                            return template1;
                        }
                    }
                }
                if (it.Type() == PT_COLOR)
                {
                    auto r_string = top.Resources().TryLookup(winrt::box_value(L"ColorTemplate"));
                    if (r_string)
                    {
                        auto template1 = r_string.try_as<winrt::Microsoft::UI::Xaml::DataTemplate>();
                        if (template1)
                        {
                            return template1;
                        }
                    }
                }
                if (it.Type() == PT_LIST)
                {
                    auto r_list = top.Resources().TryLookup(winrt::box_value(L"ListTemplate"));
                    if (r_list)
                    {
                        auto template1 = r_list.try_as<winrt::Microsoft::UI::Xaml::DataTemplate>();
                        if (template1)
                        {
                            return template1;
                        }
                    }
                }
                if (it.Type() == PT_BOOL)
                {
                    auto r_list = top.Resources().TryLookup(winrt::box_value(L"BoolTemplate"));
                    if (r_list)
                    {
                        auto template1 = r_list.try_as<winrt::Microsoft::UI::Xaml::DataTemplate>();
                        if (template1)
                        {
                            return template1;
                        }
                    }
                }
            }
            return nullptr;
        }



    };
}
namespace winrt::VisualWinUI3::factory_implementation
{
    struct ItemSelector : ItemSelectorT<ItemSelector, implementation::ItemSelector>
    {
    };
}

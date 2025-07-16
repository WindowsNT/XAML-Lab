#pragma once

#include "FileDialog.g.h"
#include "FileSystemItem.h"
#include "Item.g.h"


namespace winrt::VisualWinUI3::implementation
{
    struct FileDialog : FileDialogT<FileDialog>
    {
        FileDialog()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }


        MYPIDL _pidl;
        IInspectable CurrentPath()
        {
			return SerializePIDL(_pidl.get());
		}
        void CurrentPath(IInspectable sr)
        {
            _pidl.reset();
            if (sr)
            {
                auto buffer = sr.as<winrt::Windows::Storage::Streams::IBuffer>();
#pragma warning(disable: 4090)
                _pidl.reset(DeserializePIDL(buffer));
            }
            else
            {
                _pidl.reset();
			}
		}

        std::wstring _SelectedPath;
        winrt::hstring SelectedPath()
        {
			return _SelectedPath.c_str();
		}
        void SelectedPath(winrt::hstring path)
        {
            if (_SelectedPath == path.c_str())
				return;
            _SelectedPath = path.c_str();
            m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"SelectedPath"));
		}


        void ListT(IInspectable param1, IInspectable param2);
        void ListDT(IInspectable param1, IInspectable param2);
        void BIClicked(IInspectable param1, IInspectable param2);
        void ButtonUp(IInspectable param1, IInspectable param2);
        void ButtonNewFolder(IInspectable param1, IInspectable param2);
        void Callback1(IInspectable param1, IInspectable param2);
        void itClick(IInspectable param1, IInspectable param2);
        void buttonViewGrid(IInspectable param1, IInspectable param2);
        void buttonViewList(IInspectable param1, IInspectable param2);
        void ButtonOK(IInspectable param1, IInspectable param2);
        void ButtonCancel(IInspectable param1, IInspectable param2);
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::Item> BreadcrumbBarItems();
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::FileSystemItem> TreeItems();
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::Item> FileItemsX();


        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
        {
            return m_propertyChanged.add(handler);
        }
        void PropertyChanged(winrt::event_token const& token) noexcept
        {
            m_propertyChanged.remove(token);
        }
        winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

    };
}

namespace winrt::VisualWinUI3::factory_implementation
{
    struct FileDialog : FileDialogT<FileDialog, implementation::FileDialog>
    {
    };
}

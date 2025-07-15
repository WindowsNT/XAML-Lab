#pragma once

#include "FileDialog.g.h"
#include "FileSystemItem.h"


namespace winrt::VisualWinUI3::implementation
{
    struct FileDialog : FileDialogT<FileDialog>
    {
        FileDialog()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }


        void ButtonUp(IInspectable param1, IInspectable param2);
        void ButtonNewFolder(IInspectable param1, IInspectable param2);
        void Callback1(IInspectable param1, IInspectable param2);
        void itClick(IInspectable param1, IInspectable param2);
        void buttonViewGrid(IInspectable param1, IInspectable param2);
        void buttonViewList(IInspectable param1, IInspectable param2);
        void ButtonOK(IInspectable param1, IInspectable param2);
        void ButtonCancel(IInspectable param1, IInspectable param2);
        winrt::Windows::Foundation::Collections::IObservableVector<FrameworkElement> BreadcrumbBarItems();
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::VisualWinUI3::FileSystemItem> TreeItems();
        winrt::Windows::Foundation::Collections::IObservableVector<FrameworkElement> FileItemsX();


    };
}

namespace winrt::VisualWinUI3::factory_implementation
{
    struct FileDialog : FileDialogT<FileDialog, implementation::FileDialog>
    {
    };
}

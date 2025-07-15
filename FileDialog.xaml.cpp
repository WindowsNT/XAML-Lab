#include "pch.h"
#include "FileDialog.xaml.h"
#if __has_include("FileDialog.g.cpp")
#include "FileDialog.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::VisualWinUI3::implementation
{
 
#pragma warning(disable : 4100) // Unreferenced formal parameter
	void FileDialog::ButtonUp(IInspectable param1, IInspectable param2)
	{
		// Element is clicked
	}
	void FileDialog::ButtonNewFolder(IInspectable param1, IInspectable param2)
	{
		// Element is clicked
	}
	void FileDialog::Callback1(IInspectable param1, IInspectable param2)
	{
	}
	void FileDialog::itClick(IInspectable param1, IInspectable param2)
	{
	}
	void FileDialog::buttonViewGrid(IInspectable param1, IInspectable param2)
	{
	}
	void FileDialog::buttonViewList(IInspectable param1, IInspectable param2)
	{
	}
	void FileDialog::ButtonOK(IInspectable param1, IInspectable param2)
	{
		// Element is clicked
	}
	void FileDialog::ButtonCancel(IInspectable param1, IInspectable param2)
	{
		// Element is clicked
	}

	IObservableVector<FrameworkElement> FileDialog::BreadcrumbBarItems()
	{
		auto items = single_threaded_observable_vector<FrameworkElement>();
		return items;
	}
	IObservableVector<winrt::VisualWinUI3::FileSystemItem> FileDialog::TreeItems()
	{
		auto items = single_threaded_observable_vector<winrt::VisualWinUI3::FileSystemItem>();
		return items;
	}
	IObservableVector<FrameworkElement> FileDialog::FileItemsX()
	{
		auto items = single_threaded_observable_vector<FrameworkElement>();
		return items;
	}
}

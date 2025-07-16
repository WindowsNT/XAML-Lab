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

	void FileDialog::BIClicked(IInspectable param1, IInspectable param2)
	{
		auto bcb = param1.as<BreadcrumbBar>();
		auto p2 = param2.try_as< BreadcrumbBarItemClickedEventArgs>();
		if (!p2)
			return;
		auto it = p2.Item().try_as<winrt::VisualWinUI3::Item>();
		if (!it)
			return;

		CurrentPath(it.II());
		m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"BreadcrumbBarItems"});
		m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"FileItemsX" });
	}

	IObservableVector<winrt::VisualWinUI3::Item> FileDialog::BreadcrumbBarItems()
	{
		auto items = single_threaded_observable_vector<winrt::VisualWinUI3::Item>();

		struct STR2
		{
			std::wstring s;
			IInspectable i;
		};

		std::vector<STR2> strings;
		auto cidl = _pidl;
		for (;;)
		{
			if (!cidl)
				break;

			// Get the name of this ITEMIDLIST
			PWSTR pszName = nullptr;
			HRESULT hr = SHGetNameFromIDList(cidl.get(), SIGDN_NORMALDISPLAY, &pszName);
			if (FAILED(hr))
				break;
			STR2 str2;
			str2.s = pszName;
			str2.i = SerializePIDL(cidl.get());
			strings.insert(strings.begin(),str2);
			CoTaskMemFree(pszName);
			auto par = GetParentPIDL(cidl.get());
			if (!par)
				break;
			cidl.reset(par);
		}

		for (const auto& str : strings)
		{
			Item itx;
			itx.Name1(str.s);
			itx.II(str.i);
			items.Append(itx);
		}

		return items;
	}
	IObservableVector<winrt::VisualWinUI3::FileSystemItem> FileDialog::TreeItems()
	{
		auto items = single_threaded_observable_vector<winrt::VisualWinUI3::FileSystemItem>();
		return items;
	}
	IObservableVector<winrt::VisualWinUI3::Item> FileDialog::FileItemsX()
	{
		auto items = single_threaded_observable_vector<winrt::VisualWinUI3::Item>();
		if (!_pidl)
			return items;


		std::vector<CHILD_ITEM> ci;
		void EnumerateChildren(ITEMIDLIST * root, std::vector<CHILD_ITEM>&r);
		EnumerateChildren(_pidl.get(), ci);

		std::sort(ci.begin(), ci.end(), [](CHILD_ITEM& left, CHILD_ITEM& right) -> bool
			{
				if (left.Type == 1 && right.Type == 0)
					return true;
				if (left.Type == 0 && right.Type == 1)
					return false;
				return left.displname < right.displname;
			});
		for (auto& c : ci)
		{
			Item itx;
			itx.Name1(c.displname);
			items.Append(itx);
		}
		return items;
	}
}

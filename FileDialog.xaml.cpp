#include "pch.h"
#include "FileDialog.xaml.h"
#if __has_include("FileDialog.g.cpp")
#include "FileDialog.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;



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
		auto bcb = param1.as<winrt::Microsoft::UI::Xaml::Controls::BreadcrumbBar>();
		auto p2 = param2.try_as<winrt::Microsoft::UI::Xaml::Controls::BreadcrumbBarItemClickedEventArgs>();
		if (!p2)
			return;
		auto it = p2.Item().try_as<winrt::VisualWinUI3::Item>();
		if (!it)
			return;

		CurrentPath(it.II());
		m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"BreadcrumbBarItems"});
		m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"FileItemsX" });
		m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"TreeItems" });
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

	XML3::XMLElement tree_root;
	IObservableVector<winrt::VisualWinUI3::FileSystemItem> FileDialog::TreeItems()
	{
		auto items = single_threaded_observable_vector<winrt::VisualWinUI3::FileSystemItem>();
		void GetTheTree(ITEMIDLIST * root, ITEMIDLIST * end, XML3::XMLElement & tree_root,int Deep);
		tree_root = {};
		GetTheTree(0,_pidl.get(),tree_root,0);
#ifdef _DEBUG
		auto ser = tree_root.Serialize();
		ser;
#endif
		winrt::VisualWinUI3::FileSystemItem rootItem((long long)&tree_root, 0);
		items.Append(rootItem);
		return items;
	}

	void FileDialog::ListT(IInspectable param1, IInspectable param2)
	{
		auto lv = param1.as<ListView>();
		auto selectedItems = lv.SelectedItems();
		if (selectedItems.Size() == 0)
			return;
		auto it = selectedItems.GetAt(0).try_as<winrt::VisualWinUI3::Item>();
		if (!it)
			return;
		if (it.Boolean0())
		{
			SelectedPath(L"");
			return;
		}

		// Put it to the text box
		auto ii = it.II();
		if (ii)
		{
			auto pidl = DeserializePIDL(ii.as<winrt::Windows::Storage::Streams::IBuffer>());
			if (pidl)
			{
				// Get the full path
				PWSTR pszName = nullptr;
				HRESULT hr = SHGetNameFromIDList(pidl, SIGDN_FILESYSPATH, &pszName);
				if (SUCCEEDED(hr))
				{
					SelectedPath(pszName);
					CoTaskMemFree(pszName);
				}
			}
		}
		else
		{
			SelectedPath(L"");
		}
	}

	void FileDialog::ListDT(IInspectable param1, IInspectable param2)
	{
		auto lv = param1.as<ListView>();
		auto selectedItems = lv.SelectedItems();
		if (selectedItems.Size() == 0)
			return;
		auto it = selectedItems.GetAt(0).try_as<winrt::VisualWinUI3::Item>();
		if (!it)
			return;

		// Is it folder?
		if (it.Boolean0())
		{

			// Set the current path to the selected item
			CurrentPath(it.II());
			m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"BreadcrumbBarItems" });
			m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"FileItemsX" });
			m_propertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"TreeItems" });
		}
		else
		{

		}
	}



	IObservableVector<winrt::VisualWinUI3::Item> FileDialog::FileItemsX()
	{
		auto items = single_threaded_observable_vector<winrt::VisualWinUI3::Item>();
		if (!_pidl)
			return items;


		std::vector<CHILD_ITEM> ci;
		void EnumerateChildren(ITEMIDLIST * root, std::vector<CHILD_ITEM>&r,bool);
		EnumerateChildren(_pidl.get(), ci,false);

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
			itx.Boolean0(c.Type == 1);
			itx.Name1(c.displname);
			if (c.pidl.get())
			{
				// Absolute path
				MYPIDL fullPidl;
				fullPidl.reset(ILCombine(_pidl.get(), c.pidl.get()));

				itx.II(SerializePIDL(fullPidl.get()));
			}
			if (c.icon.has_value())
			{
				// Cast it to BitmapImage
				auto bim = std::any_cast<winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage>(c.icon);
				itx.Bitmap1(bim);
			}
			items.Append(itx);
		}
		return items;
	}
}

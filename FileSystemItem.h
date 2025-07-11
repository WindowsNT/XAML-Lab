#pragma once

#include "FileSystemItem.g.h"

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Data;

namespace winrt::VisualWinUI3::implementation
{
	struct FileSystemItem : FileSystemItemT<FileSystemItem>
	{
		FileSystemItem(long long fullPath,long long parptr)
		{
			_isSelected = true;
			_ptr = fullPath;
			_parptr = parptr;
		}

		winrt::hstring Name() const
		{
			XML3::XMLElement* el = (XML3::XMLElement*)_ptr;
			if (el == 0) return L"";
			return el->vv("n").GetWideValue().c_str();
		}
		void Name(winrt::hstring const& name)
		{
			if (name.empty())
				return;
			XML3::XMLElement* el = (XML3::XMLElement*)_ptr;
			if (el == 0) return;
			if (el->vv("n").GetWideValue() == name)
				return;
			el->vv("n").SetWideValue(name.c_str());
			RaisePropertyChanged(L"Name");
		}

		winrt::hstring Group() const
		{
			XML3::XMLElement* el = (XML3::XMLElement*)_ptr;
			if (el == 0) return L"";
			return el->vv("g").GetWideValue().c_str();
		}
		void Group(winrt::hstring const& group)
		{
			XML3::XMLElement* el = (XML3::XMLElement*)_ptr;
			if (el == 0) return;
			if (el->vv("g").GetWideValue() == group)
				return;
			el->vv("g").SetWideValue(group.c_str());
			RaisePropertyChanged(L"Group");
		}

		bool IsFolder() const
		{
			XML3::XMLElement* el = (XML3::XMLElement*)_ptr;
			if (el == 0) return false;
			if (el->GetChildrenNum() == 0)
				return 0;
			return 1;
		}

		long long  Ptr() { return _ptr; }
		long long  ParPtr() { return _parptr; }

		bool IsSelected() const { return _isSelected; }
		void IsSelected(bool const& selected){
			if (selected == _isSelected)
				return;
			
			_isSelected = selected;
			RaisePropertyChanged(L"IsSelected");
		}

		IObservableVector<VisualWinUI3::FileSystemItem> Children()
		{
			auto children{ single_threaded_observable_vector<VisualWinUI3::FileSystemItem>() };
			if (_ptr)
			{
				XML3::XMLElement* el = (XML3::XMLElement*)_ptr;
				if (!el)
					return children;
				for(auto& child : el->GetChildren())
				{
					if (child)
					{
						VisualWinUI3::FileSystemItem fsItem((long long)child.get(),(long long)this);
						children.Append(fsItem);
					}
				}
			}
			return children;
		}

		// public events
		event_token PropertyChanged(PropertyChangedEventHandler const& handler) { return _propertyChanged.add(handler); }
		void PropertyChanged(event_token token) { _propertyChanged.remove(token); }

		void RaisePropertyChanged(hstring propertyName)
		{
			_propertyChanged(*this, PropertyChangedEventArgs(propertyName));
		}

	private:
		event<PropertyChangedEventHandler> _propertyChanged;
		long long  _ptr = 0;
		long long  _parptr = 0;
		bool _isSelected;

	};
}

namespace winrt::VisualWinUI3::factory_implementation
{
	struct FileSystemItem : FileSystemItemT<FileSystemItem, implementation::FileSystemItem>
	{
	};
}

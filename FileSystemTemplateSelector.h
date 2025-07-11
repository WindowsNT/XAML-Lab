#pragma once

#include "FileSystemTemplateSelector.g.h"

using namespace winrt::Windows::Foundation;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::VisualWinUI3::implementation
{
	struct FileSystemTemplateSelector : FileSystemTemplateSelectorT<FileSystemTemplateSelector>
	{
		FileSystemTemplateSelector()
		{
		}
		
		bool TemplatesLoaded = false;
		void LoadTemplates(IInspectable root)
		{
			if (!root)
				return;
			if (TemplatesLoaded)
				return; // Already loaded
			auto res = root.as<Controls::Panel>().Resources();
			if (!res)
				return;
			auto f1 = res.Lookup(winrt::box_value(L"FileTemplate"));
			if (f1)
				_fileTemplate = f1.as<DataTemplate>();
			auto f2 = res.Lookup(winrt::box_value(L"FolderTemplate"));
			if (f2)
				_folderTemplate = f2.as<DataTemplate>();	
			TemplatesLoaded = true;
		}



		// public properties (XAML usable)
		DataTemplate FileTemplate()
		{
			return _fileTemplate;
		}
		void FileTemplate(DataTemplate const& value)
		{
			_fileTemplate = value;
		}

		DataTemplate FolderTemplate()
		{
			return _folderTemplate;
		}
		void FolderTemplate(DataTemplate const& value)
		{
			_folderTemplate = value;
		}

		// IDataTemplateSelectorOverrides
		DataTemplate SelectTemplateCore(IInspectable const& item, DependencyObject const& /*container*/)
		{
			return SelectTemplateCore(item);
		}

		DataTemplate SelectTemplateCore(IInspectable const& item)
		{
			auto fs = item.try_as<VisualWinUI3::FileSystemItem>();
			if (!fs)
				return _fileTemplate;
			return fs.IsFolder() ? _folderTemplate : _fileTemplate;
		}

		DataTemplate _fileTemplate;
		DataTemplate _folderTemplate;
	};
}




namespace winrt::VisualWinUI3::factory_implementation
{
	struct FileSystemTemplateSelector : FileSystemTemplateSelectorT<FileSystemTemplateSelector, implementation::FileSystemTemplateSelector>
	{
	};
}

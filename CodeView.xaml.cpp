#include "pch.h"
#include "CodeView.xaml.h"
#if __has_include("CodeView.g.cpp")
#include "CodeView.g.cpp"
#endif

#include "paragraphcode.hpp"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::VisualWinUI3::implementation
{

    void CodeView::LVC(IInspectable lv, IInspectable)
    {
		auto li = lv.try_as<SelectorBar>();
        if (!li)
            return;
		auto it = li.SelectedItem().as<SelectorBarItem>();
        if (!it)
			return;
		auto name = it.Name();
		if (name == L"si0") _selected_index = 0;
		if (name == L"si1") _selected_index = 1;
		if (name == L"si2") _selected_index = 2;
		if (name == L"si3") _selected_index = 3;
		PopulateRB();

    }

	void CodeView::CopyCode(IInspectable const&, IInspectable const&)
	{
		std::wstring txt;
		if (_selected_index == 0) txt = _t_idl;
		if (_selected_index == 1) txt = _t_xaml;
		if (_selected_index == 2) txt = _t_h;
		if (_selected_index == 3) txt = _t_cpp;
		if (txt.empty())
			return;
		void ToClip(HWND MainWindow, const wchar_t* t, bool Empty);
		ToClip(0, txt.c_str(), true);
	}

    void CodeView::PopulateRB()
    {
		auto top = Content().as<Panel>();
		auto rtb = top.FindName(L"rtb").as<RichTextBlock>();
		rtb.Blocks().Clear();

		if (_selected_index == 0)
		{
			// Plain IDL
			auto p1 = PreParagraph(_t_idl.c_str());
			for(auto& pp : p1)
				rtb.Blocks().Append(pp);
		}


		if (_selected_index == 1)
		{
			// Plain XAML
			auto p1 = PreParagraph(_t_xaml.c_str());
			for (auto& pp : p1)
				rtb.Blocks().Append(pp);
		}

		if (_selected_index == 2)
		{
			// Plain H
			auto p1 = PreParagraph(_t_h.c_str());
			for (auto& pp : p1)
				rtb.Blocks().Append(pp);
		}

		if (_selected_index == 3)
		{
			// Plain CPP
			auto p1 = PreParagraph(_t_cpp.c_str());
			for (auto& pp : p1)
				rtb.Blocks().Append(pp);
		}

    }

}

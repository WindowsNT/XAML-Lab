#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::VisualWinUI3::implementation
{

	void MainWindow::OnDark(IInspectable const&, IInspectable const&)
	{
		SettingsX->GetRootElement().vv("Theme").SetValueInt(2);
		SettingsX->Save();
		void PostThemeChange();
		PostThemeChange();

	}
	void MainWindow::OnLight(IInspectable const&, IInspectable const&)
	{
		SettingsX->GetRootElement().vv("Theme").SetValueInt(1);
		SettingsX->Save();
		void PostThemeChange();
		PostThemeChange();

	}
	void MainWindow::OnDefault(IInspectable const&, IInspectable const&)
	{
		SettingsX->GetRootElement().vv("Theme").SetValueInt(0);
		SettingsX->Save();
		void PostThemeChange();
		PostThemeChange();
	}


	void MainWindow::OnExit(IInspectable const&, IInspectable const&)
	{
		PostMessage((HWND)wnd(), WM_CLOSE, 0, 0);
	}
	void MainWindow::OnSaveAs(IInspectable const&, IInspectable const&)
	{

	}

	void MainWindow::ItemInvoked(IInspectable, NavigationViewItemInvokedEventArgs ar)
	{
		auto topnv = Content().as<NavigationView>();
		if (!topnv)
			return;
		Frame fr = topnv.FindName(L"contentFrame").as<Frame>();
		if (!fr)
			return;
		if (ar.IsSettingsInvoked())
		{
			fr.Navigate(winrt::xaml_typename<winrt::VisualWinUI3::Settings>());
			return;
		}
		auto it = ar.InvokedItemContainer().as<NavigationViewItem>();
		if (!it)
			return;
		auto n = it.Name();
		if (n == L"ViewMain")
			fr.Navigate(winrt::xaml_typename<winrt::VisualWinUI3::MainPage>());
	//	if (n == L"ViewNetwork")
	//		fr.Navigate(winrt::xaml_typename<winrt::VisualDML::Network>());
		/*		if (n == L"ViewAudio")
			fr.Navigate(winrt::xaml_typename<winrt::tsed::Audio>());
		if (n == L"ViewLinks")
			fr.Navigate(winrt::xaml_typename<winrt::tsed::Links>());
			*/
	}

	void MainWindow :: NVI_ChangeLightTapped(IInspectable const& it, IInspectable const& e)
	{
		auto nt = it.as<NavigationViewItem>();
		auto fly = nt.ContextFlyout();
		fly.SetAttachedFlyout(nt,fly);
		fly.ShowAttachedFlyout(nt);
		auto tr = e.as<winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs>();
		tr.Handled(true);
	}

	void MainWindow::OnLoad(IInspectable, IInspectable)
	{
		auto topnv = Content().as<NavigationView>();
		if (topnv)
		{
			Frame fr = topnv.FindName(L"contentFrame").as<Frame>();
			if (fr)
			{
				fr.Navigate(winrt::xaml_typename<winrt::VisualWinUI3::MainPage>());
				fr.Content().as<winrt::VisualWinUI3::MainPage>().wnd(wnd());
			}

			/*
			topnv.KeyDown([this](IInspectable const&, Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& a)
				{
					auto topnv = Content().as<NavigationView>();
					if (topnv)
					{
						Frame fr = topnv.FindName(L"contentFrame").as<Frame>();
						if (fr)
						{
							auto co = fr.Content().try_as<winrt::VisualDML::MLGraph>();
							if (co)
							{
								co.Key((long long)a.Key(), false);
							}
						}
					}
				});
				*/

		}
	}

}

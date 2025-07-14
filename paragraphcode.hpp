//


inline winrt::Microsoft::UI::Xaml::Documents::Paragraph PlainParagraph(const wchar_t* txt)
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Documents;
	using namespace Microsoft::UI::Xaml::Media;



	Paragraph paragraph;
	paragraph.Margin(Microsoft::UI::Xaml::ThicknessHelper::FromLengths(0, 0, 0, 0));
	Run run1;
	run1.Text(txt);
	paragraph.Inlines().Append(run1);
	return paragraph;
}



inline winrt::Microsoft::UI::Xaml::Documents::Paragraph ColoredParagraph(const wchar_t* txt,winrt::Windows::UI::Color color = winrt::Windows::UI::Colors::Red())
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Documents;
	using namespace Microsoft::UI::Xaml::Media; 
	
	
	Paragraph paragraph;
	paragraph.Margin(Microsoft::UI::Xaml::ThicknessHelper::FromLengths(0, 10, 0, 0));	
	Run run1;
	run1.Text(txt);
	run1.Foreground(SolidColorBrush(color));
	paragraph.Inlines().Append(run1);
	return paragraph;
}


inline std::vector<winrt::Microsoft::UI::Xaml::Documents::Paragraph> PreParagraph(const wchar_t* txt)
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Documents;
	using namespace Microsoft::UI::Xaml::Media;


	std::vector<winrt::Microsoft::UI::Xaml::Documents::Paragraph> pars;

	// Split text into steps with *** separator to make that paragraph red
	struct STEP
	{
		std::wstring txt;
		bool R = 0;
	};
	std::vector<STEP> steps;
	const wchar_t* start = txt;
	for (;;)
	{
		auto str = wcsstr(start, L"***");
		if (!str)
		{
			if (wcslen(start) > 0)
			{
				STEP stp;
				stp.txt = start;
				stp.R = false;
				steps.push_back(stp);
			}
			break;
		}
		if (str > start)
		{
			STEP stp;
			stp.txt = std::wstring(start, str - start);
			stp.R = false;
			steps.push_back(stp);
		}
		start = str + 3; // Move past the ***

		// find another ***
		auto end = wcsstr(start, L"***");
		if (!end)
		{
			if (wcslen(start) > 0)
			{
				STEP stp;
				stp.txt = std::wstring(start);
				stp.R = true;
				steps.push_back(stp);
			}
			break;
		}
		if (end > start)
		{
			STEP stp;
			stp.txt = std::wstring(start, end - start);
			stp.R = true;
			steps.push_back(stp);
		}
		start = end + 3; // Move past the ***
	}

	for (auto& st : steps)
	{
		Paragraph prePara;
		prePara.Margin(Microsoft::UI::Xaml::ThicknessHelper::FromLengths(0, 10, 0, 0));
		Run preRun;
		preRun.Foreground(SolidColorBrush(Windows::UI::Colors::Gray()));
		if (st.R)
			preRun.Foreground(SolidColorBrush(Windows::UI::Colors::Red()));
		preRun.Text(st.txt);
		preRun.FontFamily(FontFamily(L"Consolas"));
		preRun.FontSize(14);
		prePara.LineStackingStrategy(winrt::Microsoft::UI::Xaml::LineStackingStrategy::BlockLineHeight);
		prePara.LineHeight(18);
		prePara.Inlines().Append(preRun);

		pars.push_back(prePara);
	}

	return pars;
}
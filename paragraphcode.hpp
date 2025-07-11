//


inline Microsoft::UI::Xaml::Documents::Paragraph PlainParagraph(const wchar_t* txt)
{
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



inline Microsoft::UI::Xaml::Documents::Paragraph ColoredParagraph(const wchar_t* txt,Windows::UI::Color color = Windows::UI::Colors::Red())
{
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


inline Microsoft::UI::Xaml::Documents::Paragraph PreParagraph(const wchar_t* txt)
{
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Documents;
	using namespace Microsoft::UI::Xaml::Media;


	Paragraph prePara;
	prePara.Margin(Microsoft::UI::Xaml::ThicknessHelper::FromLengths(0, 10, 0, 0));
	Run preRun;
	preRun.Foreground(SolidColorBrush(Windows::UI::Colors::Gray()));
	preRun.Text(txt);
	preRun.FontFamily(FontFamily(L"Consolas"));
	preRun.FontSize(14);
	prePara.LineStackingStrategy(LineStackingStrategy::BlockLineHeight);
	prePara.LineHeight(18);
	prePara.Inlines().Append(preRun);

	return prePara;
}
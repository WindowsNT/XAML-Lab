template <typename T>
void ApplyPropertiesForParentGrid(T e, std::vector <std::shared_ptr<PROPERTY>> properties)
{
	for (auto& p : properties)
	{
		if (p->n == L"Grid.Column")
		{
		}
		if (p->n == L"Grid.Row")
		{
		}
	}
}


template <typename T>
void ApplyPropertiesForText(T e, std::vector <std::shared_ptr<PROPERTY>> properties)
{
	for (auto& p : properties)
	{
		if (p->n == L"Text")
		{
			auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
			if (op)
			{
				e.Text(op->value);
			}
		}
	}
}

template <typename T>
void ApplyPropertiesForIcon(T e,std::vector<std::shared_ptr<PROPERTY>> properties)
{
	for (auto& p : properties)
	{
		if (p->n == L"Icon")
		{
			auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
			if (op)
			{
				if (op->SelectedIndex >= 0 && op->SelectedIndex < op->Items.size())
				{
					auto icon = op->Items[op->SelectedIndex];
					if (icon.length())
					{
						auto fi = winrt::Microsoft::UI::Xaml::Controls::SymbolIcon();
						std::map<std::wstring, int> LoadFontIconNamesAndMappings();
						auto icons = LoadFontIconNamesAndMappings();
						auto it = icons.find(icon);
						if (it != icons.end())
						{
							int number = it->second; // This is from json
							wchar_t glyphChar[2] = { static_cast<wchar_t>(number), 0 };
							winrt::Microsoft::UI::Xaml::Controls::Symbol sy;
							sy = static_cast<winrt::Microsoft::UI::Xaml::Controls::Symbol>(number);
							fi.Symbol(sy);
						}
						e.Icon(fi);
					}
				}
			}
		}
	}
}


template <typename T>
void ApplyPropertiesForBackground(T e, std::vector <std::shared_ptr<PROPERTY>> properties)
{
	for (auto& p : properties)
	{
		if (p->n == L"Background")
		{
			auto op = std::dynamic_pointer_cast<COLOR_PROPERTY>(p);
			if (op && op->value != op->def)
			{
				// Create a SolidColorBrush with the color value
				auto solidBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush();
				solidBrush.Color(op->value);
				e.Background(solidBrush);
			}
		}
	}
}


template <typename T>
void ApplyPropertiesForForeground(T e, std::vector <std::shared_ptr<PROPERTY>> properties)
{
	for (auto& p : properties)
	{
		if (p->n == L"Foreground")
		{
			auto op = std::dynamic_pointer_cast<COLOR_PROPERTY>(p);
			if (op && op->value != op->def)
			{
				// Create a SolidColorBrush with the color value
				auto solidBrush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush();
				solidBrush.Color(op->value);
				e.Foreground(solidBrush);
			}
		}
	}
}



template <typename T>
void ApplyPropertiesForFont(T e, std::vector <std::shared_ptr<PROPERTY>> props)
{
	if (!e)
		return;
	for (auto& p : props)
	{
		if (p->n == L"FontSize")
		{
			auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
			if (op)
			{
				e.FontSize(op->value);
			}
		}
		if (p->n == L"FontFamily")
		{
			// If this is a RatingControl, skip
			if (e.try_as<winrt::Microsoft::UI::Xaml::Controls::RatingControl>())
				continue;
			auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
			if (op)
			{
				auto what = op->Items[op->SelectedIndex];
				if (what == L"")
					continue;
				try
				{
					e.FontFamily(Microsoft::UI::Xaml::Media::FontFamily(what));
				}
				catch (...)
				{

				}
			}
		}
	}
}

template <typename T>
std::vector<std::shared_ptr<PROPERTY>> CreatePropertiesForParentGrid(T ee)
{
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!ee)
		return p;

	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->mmin = 0;
		op->mmax = 1000;
		op->def = 0;
		op->smallchange = 1;
		op->largechange = 1;
		op->n = L"Grid.Column";
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->mmin = 0;
		op->mmax = 1000;
		op->def = 0;
		op->smallchange = 1;
		op->largechange = 1;
		op->n = L"Grid.Row";
		p.push_back(op);
	}


	return p;
}

template <typename T>
std::vector<std::shared_ptr<PROPERTY>> CreatePropertiesForParentCanvas(T ee)
{
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!ee)
		return p;

	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->mmin = 0;
		op->mmax = 1000;
		op->def = 0;
		op->smallchange = 1;
		op->largechange = 1;
		op->n = L"Canvas.Left";
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->mmin = 0;
		op->mmax = 1000;
		op->def = 0;
		op->smallchange = 1;
		op->largechange = 1;
		op->n = L"Canvas.Top";
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->mmin = 0;
		op->mmax = 100;
		op->def = 0;
		op->smallchange = 1;
		op->largechange = 1;
		op->n = L"Canvas.ZIndex";
		p.push_back(op);
	}


	return p;
}

template <typename T>
std::shared_ptr<PROPERTY> CreatePropertyForGenericCallback(T e, const wchar_t* func)
{
	std::shared_ptr<PROPERTY> p;
	if (!e)
		return p;
	std::shared_ptr<FUNCTION_PROPERTY> op = std::make_shared<FUNCTION_PROPERTY>();
	op->n = func;
	wchar_t txt[100] = {};
	swprintf_s(txt, 100, s(77), func);
	op->tip = txt;
	return op;
}

template <typename T>
std::vector<std::shared_ptr<PROPERTY>> CreatePropertiesForText(T e, const wchar_t* defv)
{
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;

	if (1)
	{
		std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
		op->n = L"Text";
		op->value = e.Text();
		if (defv)
			op->value = defv;
		p.push_back(op);
	}
	return p;
}


template <typename T>
std::shared_ptr<STRING_PROPERTY> CreatePropertyItemsSource(T e)
{
	if (e)
	{
		std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
		op->n = L"ItemsSource";
		op->BindingAnyway = 1; // This is a binding anyway
		return op;
	}
	return nullptr;
}

template <typename T>
std::vector<std::shared_ptr<PROPERTY>> CreatePropertiesForIcon(T e)
{
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;
	std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
	op->n = L"Icon";
	std::map<std::wstring, int> LoadFontIconNamesAndMappings();
	auto icons = LoadFontIconNamesAndMappings();
	op->Items.push_back(L"");
	auto ic = e.Icon();
	auto tryfc = ic.try_as<winrt::Microsoft::UI::Xaml::Controls::FontIcon>();
	std::wstring glyphname;
	if (tryfc)
	{
		glyphname = tryfc.Glyph();
	}
	int iadd = 1;
	for (const auto& icon : icons)
	{
		op->Items.push_back(icon.first);

		// Check if this is the current 
		if (glyphname.length() && icon.first == glyphname)
			op->SelectedIndex = iadd;
		iadd++;
	}
	p.push_back(op);
	return p;
}

template <typename T>
std::vector<std::shared_ptr<PROPERTY>> CreatePropertiesForFont(T e)
{
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;

	if (1)
	{
		std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
		op->g = L"Control";
		op->n = L"FontFamily";

		std::wstring curr;
		auto exist = e.FontFamily();
		if (exist)
			curr = exist.Source();

		// Enum Font Families
		CComPtr<IDWriteFactory> dwriteFactory;
		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&dwriteFactory);

		CComPtr<IDWriteFontCollection> fontCollection;
		dwriteFactory->GetSystemFontCollection(&fontCollection);

		UINT32 familyCount = fontCollection->GetFontFamilyCount();
		for (UINT32 i = 0; i < familyCount; ++i)
		{
			CComPtr<IDWriteFontFamily> fontFamily;
			fontCollection->GetFontFamily(i, &fontFamily);

			CComPtr<IDWriteLocalizedStrings> names;
			fontFamily->GetFamilyNames(&names);

			UINT32 index = 0;
			BOOL exists = false;
			names->FindLocaleName(L"en-us", &index, &exists);
			if (!exists) index = 0;

			UINT32 length = 0;
			names->GetStringLength(index, &length);

			std::wstring name(length + 1, L'\0');
			names->GetString(index, &name[0], length + 1);
			name.resize(length);

			op->Items.push_back(name);
		}


		// See if "curr" is there
		if (std::find(op->Items.begin(), op->Items.end(), curr) == op->Items.end())
		{
			op->Items.push_back(curr);
		}

		// Sort
		std::sort(op->Items.begin(), op->Items.end());

		// Find select
		for (size_t i = 0; i < op->Items.size(); i++)
		{
			if (op->Items[i] == curr)
			{
				op->SelectedIndex = i;
				op->DefaultIndex = i;
				break;
			}
		}

		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->g = L"Control";
		op->n = L"FontSize";
		op->mmin = 1;
		op->mmax = 100.0;
		op->value = e.FontSize();
		op->def = e.FontSize();
		p.push_back(op);
	}

	return p;
}


template <typename T>
std::vector<std::shared_ptr<PROPERTY>> CreatePropertiesForBackground(T e)
{
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;

	if (1)
	{
		std::shared_ptr<COLOR_PROPERTY> op = std::make_shared<COLOR_PROPERTY>();
		op->n = L"Background";
		try
		{
			auto cont = e.Background();
			auto solidBrush = cont.try_as<winrt::Microsoft::UI::Xaml::Media::SolidColorBrush>();
			if (solidBrush)
			{
				op->value = solidBrush.Color();
				op->def = solidBrush.Color();
			}
		}
		catch (...)
		{
		}
		p.push_back(op);
	}
	return p;
}
template <typename T>
std::vector<std::shared_ptr<PROPERTY>> CreatePropertiesForForeground(T e)
{
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;

	if (1)
	{
		std::shared_ptr<COLOR_PROPERTY> op = std::make_shared<COLOR_PROPERTY>();
		op->n = L"Foreground";
		try
		{
			auto cont = e.Foreground();
			auto solidBrush = cont.try_as<winrt::Microsoft::UI::Xaml::Media::SolidColorBrush>();
			if (solidBrush)
			{
				op->value = solidBrush.Color();
				op->def = solidBrush.Color();
			}
		}
		catch (...)
		{
		}
		p.push_back(op);
	}
	return p;
}



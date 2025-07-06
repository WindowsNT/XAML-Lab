#include <pch.h>

const wchar_t* ttitle = L"Visual WinUI3";
std::shared_ptr<XML3::XML> SettingsX;
std::wstring datafolder;

#include "property.hpp"


bool is_light_theme() {
    // based on https://stackoverflow.com/questions/51334674/how-to-detect-windows-10-light-dark-mode-in-win32-application

    // The value is expected to be a REG_DWORD, which is a signed 32-bit little-endian
    char buffer[4];
    DWORD cbData = 4;
    RegGetValueW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        L"AppsUseLightTheme",
        RRF_RT_REG_DWORD, // expected value type
        nullptr,
        buffer,
        &cbData);

    // convert bytes written to our buffer to an int, assuming little-endian
    auto i = int(buffer[3] << 24 |
        buffer[2] << 16 |
        buffer[1] << 8 |
        buffer[0]);

    return i == 1;
}


CComPtr<IWICImagingFactory> wbfact;

void SaveWic(IWICBitmapSource* src, const wchar_t* out)
{
    if (!wbfact)
        CoCreateInstance(CLSID_WICImagingFactory, 0, CLSCTX_INPROC_SERVER,
            __uuidof(IWICImagingFactory), (void**)&wbfact);

    CComPtr<IWICBitmapEncoder> wicBitmapEncoder;
    wbfact->CreateEncoder(GUID_ContainerFormatPng, 0, &wicBitmapEncoder);
	CComPtr<IWICStream> stream;
	wbfact->CreateStream(&stream);  

	stream->InitializeFromFilename(out, GENERIC_WRITE);
	wicBitmapEncoder->Initialize(stream, WICBitmapEncoderNoCache);
	CComPtr<IWICBitmapFrameEncode> wicFrameEncode;
	wicBitmapEncoder->CreateNewFrame(&wicFrameEncode, 0);
	wicFrameEncode->Initialize(0);
	UINT W = 0, H = 0;
	src->GetSize(&W, &H);
	wicFrameEncode->SetSize(W, H);
    GUID g2 = GUID_WICPixelFormat32bppBGRA;
	wicFrameEncode->SetPixelFormat(&g2);
	wicFrameEncode->WriteSource(src, 0);
	wicFrameEncode->Commit();
	wicBitmapEncoder->Commit();

}
CComPtr<IWICBitmap> LoadWic(const wchar_t* o)
{
	if (!wbfact)
		CoCreateInstance(CLSID_WICImagingFactory, 0, CLSCTX_INPROC_SERVER,
			__uuidof(IWICImagingFactory), (void**)&wbfact);
	CComPtr<IWICBitmapDecoder> pDecoder = NULL;
	CComPtr<IWICBitmapFrameDecode> pSource = NULL;

	auto hr = wbfact->CreateDecoderFromFilename(
		o,
		NULL, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	if (FAILED(hr)) return {};
	hr = pDecoder->GetFrame(0, &pSource);
	if (FAILED(hr)) return {};

    CComPtr<IWICBitmap> bix;
    wbfact->CreateBitmapFromSource(pSource, WICBitmapCacheOnDemand, &bix);
    return bix;
}



#include "english.hpp"


const wchar_t* s(size_t idx)
{
    if (idx > MAX_LANG)
        return L"";
    return z_strings[idx];
}

// **********





void LoadXMLPropertiesfor(XML3::XMLElement& ee, std::vector <std::shared_ptr<PROPERTY>> props)
{
	int sptype1idx = 0;
	int sptype2idx = 0;
	for (auto& p : props)
	{
		auto what = p->xmln;
		if (what == L"*")
			continue; // don't load

		if (p->special_type != 1)
			sptype1idx = 0;
		if (p->special_type != 2)
			sptype2idx = 0;
		if (p->special_type != 0)
		{
			if (p->special_type == 1)
			{
				// Grid ,"RowDefinition"
				auto rd = ee.FindElementZ("Grid.RowDefinitions", false);
				if (rd)
				{
					auto count = rd->GetChildrenNum();
					if (count > sptype1idx)
					{
						auto ch = rd->GetChildren()[sptype1idx];
						auto opx = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
						if (opx)
						{
							auto FindH = ch->FindVariableZ("Height");
							auto value = FindH->GetValue();
							opx->value = XML3::XMLU(value.c_str()).wc();
						}
						sptype1idx++;
					}
				}
			}
			if (p->special_type == 2)
			{
				// Grid ,"ColumnDefinition"
				auto rd = ee.FindElementZ("Grid.ColumnDefinitions", false);
				if (rd)
				{
					auto count = rd->GetChildrenNum();
					if (count > sptype2idx)
					{
						auto ch = rd->GetChildren()[sptype2idx];
						auto opx = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
						if (opx)
						{
							auto FindH = ch->FindVariableZ("Width");
							auto value = FindH->GetValue();
							opx->value = XML3::XMLU(value.c_str()).wc();
						}
						sptype2idx++;
					}
				}
			}
			continue;
		}

		if (what.empty())
			what = p->n;



		if (1)
		{
			auto opx = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
			if (opx)
			{
				auto op = ee.FindVariableZ(XML3::XMLU(what.c_str()).bc(), false);
				opx->value = opx->def;
				if (op)
					opx->value = op->GetValueDouble(opx->def);
			}
		}
		if (1)
		{
			auto opx = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
			if (opx)
			{
				auto op = ee.FindVariableZ(XML3::XMLU(what.c_str()).bc(), false);
				if (op)
				{
					auto str = op->GetWideValue();
					for (size_t i = 0; i < opx->Items.size(); i++)
					{
						if (opx->Items[i] == str)
						{
							opx->SelectedIndex = i;
							break;
						}
					}
				}
			}
		}

		if (1)
		{
			auto opx = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
			if (opx)
			{
				opx->SelectedIndex = opx->DefaultIndex; // Default to false
				auto op = ee.FindVariableZ(XML3::XMLU(what.c_str()).bc(), false);
				if (op)
				{
					auto str = op->GetWideValue();
					std::vector<std::wstring> Items = { L"False", L"True" };
					for (size_t i = 0; i < Items.size(); i++)
					{
						if (Items[i] == str)
						{
							opx->SelectedIndex = i;
							break;
						}
					}
				}
			}
		}

		if (1)
		{
			auto opx = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
			if (opx)
			{
				auto op = ee.FindVariableZ(XML3::XMLU(what.c_str()).bc(), false);
				opx->value = opx->def;
				if (op)
					opx->value = op->GetWideValue();
			}
		}
	}

}

void XMLPropertiesFor(XML3::XMLElement& ee,std::vector <std::shared_ptr<PROPERTY>> props)
{
	std::shared_ptr<XML3::XMLElement> grid_rds = 0;
	std::shared_ptr<XML3::XMLElement> grid_cds = 0;
	for (auto& p : props)
	{
		auto what = p->xmln;
		if (what == L"*")
			continue; // don't save this
		if (p->special_type != 0)
		{
			if (p->special_type == 1)
			{
				// Grid ,"RowDefinition"
				if (!grid_rds)
					grid_rds = ee.InsertElement((size_t)-1,"Grid.RowDefinitions");

				auto& rd = grid_rds->AddElement("RowDefinition");
				auto opx = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
				if (opx)
				{
					rd.vv("Height").SetValue(opx->value);	
				}
			}
			if (p->special_type == 2)
			{
				// Grid ,"ColumnDefinition"
				if (!grid_cds)
					grid_cds = ee.InsertElement((size_t)-1, "Grid.ColumnDefinitions");

				auto& rd = grid_cds->AddElement("ColumnDefinition");
				auto opx = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
				if (opx)
				{
					rd.vv("Width").SetValue(opx->value);
				}
			}

			continue; 
		}
		if (what.empty())
			what = p->n;

		if (1)
		{
			auto opx = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
			if (opx && opx->value != opx->def && !std::isnan(opx->value))
			{
				auto& op = ee.AddVariable(XML3::XMLU(what.c_str()).bc());
				op.SetValueDouble(opx->value);
			}
		}

		if (1)
		{
			auto opx = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
			if (opx && opx->SelectedIndex != opx->DefaultIndex)
			{
				auto& op = ee.AddVariable(XML3::XMLU(what.c_str()).bc());
				op.SetWideValue(opx->Items[opx->SelectedIndex].c_str());
			}
		}

		if (1)
		{
			auto opx = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
			if (opx && opx->SelectedIndex != opx->DefaultIndex)
			{
				auto& op = ee.AddVariable(XML3::XMLU(what.c_str()).bc());
				if (opx->SelectedIndex)
					op.SetWideValue(L"True");
				else
					op.SetWideValue(L"False");
			}
		}

		if (1)
		{
			auto opx = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
			if (opx && opx->value != opx->def)
			{
				auto& op = ee.AddVariable(XML3::XMLU(what.c_str()).bc());
				op.SetWideValue(opx->value.c_str());
			}
		}

		if (1)
		{
			auto opx = std::dynamic_pointer_cast<COLOR_PROPERTY>(p);
			if (opx && opx->value != opx->def)
			{
				auto& op = ee.AddVariable(XML3::XMLU(what.c_str()).bc());
				// #XXXXXXXX
				wchar_t xf[100] = {};
				swprintf_s(xf, 100, L"#%02X%02X%02X%02X", opx->value.R, opx->value.G, opx->value.G, opx->value.A);
				op.SetWideValue(xf);
			}
		}

	}
}




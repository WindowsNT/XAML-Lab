#include <pch.h>

const wchar_t* ttitle = L"XAML Lab";
std::shared_ptr<XML3::XML> SettingsX;
std::wstring datafolder;

#include "property.hpp"
#include <winrt/Microsoft.Graphics.Imaging.h>
#include <winrt/Windows.Graphics.Imaging.h>


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

		// Find b___ any
		auto wh2 = L"b___" + what;
		auto opb = ee.FindVariableZ(XML3::XMLU(wh2.c_str()).bc(), false);
		if (opb)
			p->bindv = opb->GetWideValue();


		if (1)
		{
			auto opx = std::dynamic_pointer_cast<COLOR_PROPERTY>(p);
			if (opx)
			{
				auto op = ee.FindVariableZ(XML3::XMLU(what.c_str()).bc(), false);
				opx->value = opx->def;
				if (op)
				{
					// its #RRGGBBAA
					auto str = op->GetValue();
					if (str.length() >= 9 && str[0] == '#')
					{
						try
						{
							uint32_t color = std::stoul(str.substr(1, 8), nullptr, 16);
							opx->value.R = (color >> 24) & 0xFF; // Red
							opx->value.G = (color >> 16) & 0xFF; // Green
							opx->value.B = (color >> 8) & 0xFF;  // Blue
							opx->value.A = color & 0xFF;         // Alpha
						}
						catch (...)
						{
							opx->value = opx->def; // reset to default if parsing fails
						}
					}
				}
			}
		}

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

		if (1)
		{
			auto opx = std::dynamic_pointer_cast<FUNCTION_PROPERTY>(p);
			if (opx)
			{
				auto op = ee.FindVariableZ(XML3::XMLU(what.c_str()).bc(), false);
				if (op)
					opx->value = op->GetWideValue();
			}
		}

	}

}

void XMLPropertiesFor(XML3::XMLElement& ee, XITEM* xit, std::vector <std::shared_ptr<PROPERTY>> props)
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
			if (p->special_type == 3 || p->special_type == 4)
			{
				// Add <Button.RenderTransform> and <CompositeTransform >
				auto newname = xit->ElementName;
				newname += L".RenderTransform";
				if (p->special_type == 3)
				{
					// Scale
					auto opx = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (opx)
					{
						auto parts = split(opx->value.c_str(), L',');
						if (parts.size() == 3)
						{
							float x = std::stof(parts[0]);
							float y = std::stof(parts[1]);
							float z = std::stof(parts[2]);
							if (x == 1 && y == 1 && z == 1)
								continue; // no need to save
							if (x > 0 && y > 0 && z > 0)
							{
								auto rt = ee.FindElementZ(XML3::XMLU(newname.c_str()).bc(), true);
								auto ct = rt->FindElementZ("CompositeTransform", true);
								ct->vv("ScaleX").SetValueDouble(x);
								ct->vv("ScaleY").SetValueDouble(y);
//								ct->vv("ScaleZ").SetValueDouble(z);
							}
						}
					}
				}
				else
				{
					// Translate
					auto opx = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (opx)
					{
						auto parts = split(opx->value.c_str(), L',');
						if (parts.size() == 3)
						{
							float x = std::stof(parts[0]);
							float y = std::stof(parts[1]);
							float z = std::stof(parts[2]);
							if (x == 0 && y == 0 && z == 0)
								continue; // no need to save
							if (x != 0 || y != 0 || z != 0)
							{
								auto rt = ee.FindElementZ(XML3::XMLU(newname.c_str()).bc(), true);
								auto ct = rt->FindElementZ("CompositeTransform", true);
								ct->vv("TranslateX").SetValueDouble(x);
								ct->vv("TranslateY").SetValueDouble(y);
								//ct->vv("TranslateZ").SetValueDouble(z);
							}
						}
					}
				}

			}
			if (p->special_type == 5)
			{
				// MenuBar Menu
				auto mb = xit->X.as<MenuBar>();
				void MenuBarToXML(winrt::Microsoft::UI::Xaml::Controls::MenuBar & mb, winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem> curitems, winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItemBase> curitems2, XML3::XMLElement * root, XML3::XMLElement * curr);
				MenuBarToXML(mb, mb.Items(), nullptr, &ee, &ee);
			}
			if (p->special_type == 6)
			{
				// DropDownButton Menu
				auto mb = xit->X.as<DropDownButton>();
				void DropDownButtonToXML(winrt::Microsoft::UI::Xaml::Controls::DropDownButton & mb, winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItemBase> curitems, XML3::XMLElement * root, XML3::XMLElement * curr);
				auto fly = mb.Flyout();
				if (fly)
				{
					auto flyout = fly.try_as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyout>();
					if (flyout)
					{
						auto& ee1 = ee["DropDownButton.Flyout"];
						auto& ee2 = ee1["MenuFlyout"];
						DropDownButtonToXML(mb, flyout.Items(), &ee, &ee2);
					}
				}
			}
			if (p->special_type == 7 && ExportForXAML == 1)
			{
				// NV Page
				auto nv = xit->X.as<NavigationView>();
				if (nv)
				{
					auto& e11 = ee.AddElement("NavigationView.MenuItems");
					for(auto ch : nv.MenuItems())
					{
						auto cmd = ch.try_as<winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem>();
						if (cmd)
						{
							auto& e2 = e11.AddElement("NavigationViewItem");
							e2.vv("Content").SetWideValue(winrt::unbox_value<winrt::hstring>(cmd.Content()).c_str());
						}
					}
					auto& e12 = ee.AddElement("NavigationView.FooterMenuItems");
					for (auto ch : nv.FooterMenuItems())
					{
						auto cmd = ch.try_as<winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem>();
						if (cmd)
						{
							auto& e2 = e12.AddElement("NavigationViewItem");
							e2.vv("Content").SetWideValue(winrt::unbox_value<winrt::hstring>(cmd.Content()).c_str());
						}
					}
				}

				// And the page's xaml
				for (auto& p1 : xit->properties)
				{
					if (p1->n == L"Page")
					{
						auto opx = std::dynamic_pointer_cast<LIST_PROPERTY>(p1);
						if (opx)
						{
							void LoadPageNames(std::vector<std::wstring>& names, XML3::XMLElement&);
							std::vector<std::wstring> names;
							XML3::XMLElement menuEl;
							LoadPageNames(names, menuEl);

							std::wstring pagename;
							for(size_t i = 0 ; i < opx->Items.size(); i++)
							{
								if (opx->Items[i] == L"None")
									continue;
								if (i == opx->SelectedIndex)
								{
									pagename = opx->Items[i];
									break;
								}
							}


							// find in pages
							std::optional<XML3::XMLElement> FindPageByName(const std::wstring& name);
							auto page = FindPageByName(pagename);
							if (page && page->GetChildrenNum() > 0)
							{
								ee.AddElement(*page->GetChildren()[0]);
							}
							
						}
					}
				}
				continue;
			}

			if ((p->special_type == 5 || p->special_type == 6 || p->special_type == 7) && ExportForXAML == 0)
			{

			}
			else
				continue; 
		}
		if (what.empty())
			what = p->n;


		if (!p->bindv.empty())
		{
			if (ExportForXAML == 1)
			{
				auto& op = ee.AddVariable(XML3::XMLU(what.c_str()).bc());

				// {x:Bind function, Mode=OneWay}
				wchar_t xf[200] = {};
				swprintf_s(xf, 200, L"{x:Bind %s, Mode=OneWay}", p->bindv.c_str());
				op.SetWideValue(xf);
			}
			else
			{
				auto wh2 = L"b___" + what;
				auto& op = ee.AddVariable(XML3::XMLU(wh2.c_str()).bc());
				op.SetWideValue(p->bindv.c_str());
			}
			continue;
		}

		if (what == L"Content" && dynamic_cast<XITEM_ContentControl*>(xit) && xit->children.size() == 1)
		{
			auto& Content = ee.AddElement("Content");
			auto child = xit->children[0];
			if (child)
			{
				auto childel = child->SaveEl();
				Content.AddElement(childel);
			}
			continue;
		}

		if (1)
		{
			auto opx = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
			if (opx && opx->value != opx->def && !std::isnan(opx->value))
			{
				auto& op = ee.AddVariable(XML3::XMLU(what.c_str()).bc());
				int ivalue = (int)opx->value;
				if ((double)ivalue == opx->value)
					op.SetValueInt(ivalue);
				else
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
			auto opx = std::dynamic_pointer_cast<FUNCTION_PROPERTY>(p);
			if (opx && opx->value.length())
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
				swprintf_s(xf, 100, L"#%02X%02X%02X%02X", opx->value.R, opx->value.G, opx->value.B, opx->value.A);
				op.SetWideValue(xf);
			}
		}

	}

	// TreeView add sample item
	if (ExportForXAML == 1 && xit->ElementName == L"TreeView")
	{
		/*
		        <TreeView.ItemTemplate>
                    <DataTemplate x:DataType="local:FileSystemItem">
                            <TreeViewItem ItemsSource="{x:Bind Children,Mode=OneWay}" IsExpanded="True">
                                <StackPanel Orientation="Horizontal">
                                    <TextBlock Text="{x:Bind Name,Mode=OneWay}" />
                                </StackPanel>
                            </TreeViewItem>
                    </DataTemplate>
                </TreeView.ItemTemplate>

		*/
		auto& e1 = ee.AddElement("TreeView.ItemTemplate");
		auto& e2 = e1.AddElement("DataTemplate");
		e2.vv("x:DataType").SetWideValue(L"local:MyItem");
		auto& e3 = e2.AddElement("TreeViewItem");
		e3.vv("ItemsSource").SetWideValue(L"{x:Bind Children,Mode=OneWay}");
		e3.vv("IsExpanded").SetValue("True");
		auto& e4 = e3.AddElement("StackPanel");
		e4.vv("Orientation").SetWideValue(L"Horizontal");
		auto& e5 = e4.AddElement("TextBlock");
		e5.vv("Text").SetWideValue(L"{x:Bind Name,Mode=OneWay}");

	}
}



void ToClip(HWND MainWindow, const wchar_t* t, bool Empty)
{
	if (OpenClipboard(MainWindow))
	{
		if (Empty)
			EmptyClipboard();
		HGLOBAL hGG = GlobalAlloc(GMEM_FIXED, wcslen(t) * 2 + 100);
		void* pp = GlobalLock(hGG);
		wcscpy_s((wchar_t*)pp, wcslen(t) + 10, t);
		SetClipboardData(CF_UNICODETEXT, hGG);
		GlobalUnlock(hGG);
		CloseClipboard();
	}
}


void ReloadMenu2(XML3::XMLElement* root0,XML3::XMLElement* root1, winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItemBase> items)
{
	if (!root1)
		return;
	auto n = root1->vv("n").GetWideValue();
	int sep = 0;
	auto g = root1->vv("g").GetWideValue();
	auto clicked = root1->vv("clicked").GetWideValue();
	if (root1->FindVariableZ("sep"))
		sep = root1->vv("sep").GetValueInt();
	if (root1->GetChildrenNum() == 0)
	{
		if (sep == 1)
		{
			winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutSeparator item1;
			item1.Tag(winrt::box_value(clicked));
			items.Append(item1);
			return;
		}
		if (g.length())
		{
			winrt::Microsoft::UI::Xaml::Controls::RadioMenuFlyoutItem item1;
			item1.GroupName(g);
			item1.Text(n);
			item1.Tag(winrt::box_value(clicked));
			items.Append(item1);
			return;
		}

		winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem item1;
		item1.Text(n);
		item1.Tag(winrt::box_value(clicked));
		items.Append(item1);
	}
	else
	{
		if (root1 == root0)
		{
			for (size_t i = 0; i < root1->GetChildrenNum(); i++)
			{
				auto e = root1->GetChildren()[i];
				ReloadMenu2(root0,e.get(), items);
			}
		}
		else
		{
			winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutSubItem item1;
			item1.Text(n);
			item1.Tag(winrt::box_value(clicked));
			items.Append(item1);
			for (size_t i = 0; i < root1->GetChildrenNum(); i++)
			{
				auto e = root1->GetChildren()[i];
				ReloadMenu2(root0,e.get(), item1.Items());
			}
		}
	}
}


void ReloadMenu2(XML3::XMLElement* root0, XML3::XMLElement* root1, winrt::Windows::Foundation::Collections::IVector<winrt::Microsoft::UI::Xaml::Controls::MenuBarItem> items)
{
	if (!root1)
		return;
	auto n = root1->vv("n").GetWideValue();
	int sep = 0;
	if (root1->FindVariableZ("sep"))
		sep = root1->vv("sep").GetValueInt();
	auto clicked = root1->vv("clicked").GetWideValue();
	if (root1->GetChildrenNum() == 0)
	{
		if (sep == 1)
		{
//			winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutSeparator item1;
	//		items.Append(item1);
			return;
		}
		winrt::Microsoft::UI::Xaml::Controls::MenuBarItem item1;
		item1.Tag(winrt::box_value(clicked));
		item1.Title(n);
		items.Append(item1);
	}
	else
	{
		if (root1 == root0)
		{
			for (size_t i = 0; i < root1->GetChildrenNum(); i++)
			{
				auto e = root1->GetChildren()[i];
				ReloadMenu2(root0, e.get(), items);
			}
		}
		else
		{
			winrt::Microsoft::UI::Xaml::Controls::MenuBarItem item1;
			item1.Title(n);
			item1.Tag(winrt::box_value(clicked));
			items.Append(item1);
			for (size_t i = 0; i < root1->GetChildrenNum(); i++)
			{
				auto e = root1->GetChildren()[i];
				ReloadMenu2(root0, e.get(), item1.Items());
			}
		}
	}
}

void ProjectXMLElementToMenuBar(winrt::Microsoft::UI::Xaml::Controls::MenuBar mb, XML3::XMLElement& e)
{
	mb.Items().Clear();
	ReloadMenu2(e.GetChildren()[0].get(), e.GetChildren()[0].get(), mb.Items());
}

void ProjectXMLElementToDropDownButton(winrt::Microsoft::UI::Xaml::Controls::DropDownButton mb, XML3::XMLElement& e)
{
	MenuFlyout flyout;
	ReloadMenu2(e.GetChildren()[0].get(), e.GetChildren()[0].get(), flyout.Items());
	mb.Flyout(flyout);
}

HRESULT ExtractResource(HINSTANCE hXX, const TCHAR* Name, const TCHAR* ty, std::vector<char>& data)
{
	HRSRC R = FindResource(hXX, Name, ty);
	if (!R)
		return E_NOINTERFACE;
	HGLOBAL hG = LoadResource(hXX, R);
	if (!hG)
		return E_FAIL;
	DWORD S = SizeofResource(hXX, R);
	char* p = (char*)LockResource(hG);
	if (!p)
	{
		FreeResource(R);
		return E_FAIL;
	}
	data.resize(S);
	memcpy(data.data(), p, S);
	FreeResource(R);
	return S_OK;
}


std::map<std::wstring, int> fmappings = {
	{L"Previous", 57600},
	{L"Next", 57601},
	{L"Play", 57602},
	{L"Pause", 57603},
	{L"Edit", 57604},
	{L"Save", 57605},
	{L"Clear", 57606},
	{L"Delete", 57607},
	{L"Remove", 57608},
	{L"Add", 57609},
	{L"Cancel", 57610},
	{L"Accept", 57611},
	{L"More", 57612},
	{L"Redo", 57613},
	{L"Undo", 57614},
	{L"Home", 57615},
	{L"Up", 57616},
	{L"Forward", 57617},
	{L"Back", 57618},
	{L"Favorite", 57619},
	{L"Camera", 57620},
	{L"Setting", 57621},
	{L"Video", 57622},
	{L"Sync", 57623},
	{L"Download", 57624},
	{L"Mail", 57625},
	{L"Find", 57626},
	{L"Help", 57627},
	{L"Upload", 57628},
	{L"Emoji", 57629},
	{L"TwoPage", 57630},
	{L"LeaveChat", 57631},
	{L"MailForward", 57632},
	{L"Clock", 57633},
	{L"Send", 57634},
	{L"Crop", 57635},
	{L"RotateCamera", 57636},
	{L"People", 57637},
	{L"OpenPane", 57638},
	{L"ClosePane", 57639},
	{L"World", 57640},
	{L"Flag", 57641},
	{L"PreviewLink", 57642},
	{L"Globe", 57643},
	{L"Trim", 57644},
	{L"AttachCamera", 57645},
	{L"ZoomIn", 57646},
	{L"Bookmarks", 57647},
	{L"Document", 57648},
	{L"ProtectedDocument", 57649},
	{L"Page", 57650},
	{L"Bullets", 57651},
	{L"Comment", 57652},
	{L"MailFilled", 57653},
	{L"ContactInfo", 57654},
	{L"HangUp", 57655},
	{L"ViewAll", 57656},
	{L"MapPin", 57657},
	{L"Phone", 57658},
	{L"VideoChat", 57659},
	{L"Switch", 57660},
	{L"Contact", 57661},
	{L"Rename", 57662},
	{L"Pin", 57665},
	{L"MusicInfo", 57666},
	{L"Go", 57667},
	{L"Keyboard", 57668},
	{L"DockLeft", 57669},
	{L"DockRight", 57670},
	{L"DockBottom", 57671},
	{L"Remote", 57672},
	{L"Refresh", 57673},
	{L"Rotate", 57674},
	{L"Shuffle", 57675},
	{L"List", 57676},
	{L"Shop", 57677},
	{L"SelectAll", 57678},
	{L"Orientation", 57679},
	{L"Import", 57680},
	{L"ImportAll", 57681},
	{L"BrowsePhotos", 57685},
	{L"WebCam", 57686},
	{L"Pictures", 57688},
	{L"SaveLocal", 57689},
	{L"Caption", 57690},
	{L"Stop", 57691},
	{L"ShowResults", 57692},
	{L"Volume", 57693},
	{L"Repair", 57694},
	{L"Message", 57695},
	{L"Page2", 57696},
	{L"CalendarDay", 57697},
	{L"CalendarWeek", 57698},
	{L"Calendar", 57699},
	{L"Character", 57700},
	{L"MailReplyAll", 57701},
	{L"Read", 57702},
	{L"Link", 57703},
	{L"Account", 57704},
	{L"ShowBcc", 57705},
	{L"HideBcc", 57706},
	{L"Cut", 57707},
	{L"Attach", 57708},
	{L"Paste", 57709},
	{L"Filter", 57710},
	{L"Copy", 57711},
	{L"Emoji2", 57712},
	{L"Important", 57713},
	{L"MailReply", 57714},
	{L"SlideShow", 57715},
	{L"Sort", 57716},
	{L"Manage", 57720},
	{L"AllApps", 57721},
	{L"DisconnectDrive", 57722},
	{L"MapDrive", 57723},
	{L"NewWindow", 57724},
	{L"OpenWith", 57725},
	{L"ContactPresence", 57729},
	{L"Priority", 57730},
	{L"GoToToday", 57732},
	{L"Font", 57733},
	{L"FontColor", 57734},
	{L"Contact2", 57735},
	{L"Folder", 57736},
	{L"Audio", 57737},
	{L"Placeholder", 57738},
	{L"View", 57739},
	{L"SetLockScreen", 57740},
	{L"SetTile", 57741},
	{L"ClosedCaption", 57744},
	{L"StopSlideShow", 57745},
	{L"Permissions", 57746},
	{L"Highlight", 57747},
	{L"DisableUpdates", 57748},
	{L"UnFavorite", 57749},
	{L"UnPin", 57750},
	{L"OpenLocal", 57751},
	{L"Mute", 57752},
	{L"Italic", 57753},
	{L"Underline", 57754},
	{L"Bold", 57755},
	{L"MoveToFolder", 57756},
	{L"LikeDislike", 57757},
	{L"Dislike", 57758},
	{L"Like", 57759},
	{L"AlignRight", 57760},
	{L"AlignCenter", 57761},
	{L"AlignLeft", 57762},
	{L"Zoom", 57763},
	{L"ZoomOut", 57764},
	{L"OpenFile", 57765},
	{L"OtherUser", 57766},
	{L"Admin", 57767},
	{L"Street", 57795},
	{L"Map", 57796},
	{L"ClearSelection", 57797},
	{L"FontDecrease", 57798},
	{L"FontIncrease", 57799},
	{L"FontSize", 57800},
	{L"CellPhone", 57801},
	{L"ReShare", 57802},
	{L"Tag", 57803},
	{L"RepeatOne", 57804},
	{L"RepeatAll", 57805},
	{L"OutlineStar", 57806},
	{L"SolidStar", 57807},
	{L"Calculator", 57808},
	{L"Directions", 57809},
	{L"Target", 57810},
	{L"Library", 57811},
	{L"PhoneBook", 57812},
	{L"Memo", 57813},
	{L"Microphone", 57814},
	{L"PostUpdate", 57815},
	{L"BackToWindow", 57816},
	{L"FullScreen", 57817},
	{L"NewFolder", 57818},
	{L"CalendarReply", 57819},
	{L"UnSyncFolder", 57821},
	{L"ReportHacked", 57822},
	{L"SyncFolder", 57823},
	{L"BlockContact", 57824},
	{L"SwitchApps", 57825},
	{L"AddFriend", 57826},
	{L"TouchPointer", 57827},
	{L"GoToStart", 57828},
	{L"ZeroBars", 57829},
	{L"OneBar", 57830},
	{L"TwoBars", 57831},
	{L"ThreeBars", 57832},
	{L"FourBars", 57833},
	{L"Scan", 58004},
	{L"Preview", 58005},
	{L"GlobalNavigationButton", 59136},
	{L"Share", 59181},
	{L"Print", 59209},
	{L"XboxOneConsole", 59792}
};

std::map<std::wstring,int> LoadFontIconNamesAndMappings()
{
//	if (fmappings.size())
		return fmappings; // already loaded	

/*	std::map<std::wstring, int> m;
	// Extract resource "FONT_JSON" "DATA"

	std::vector<char> d;
	ExtractResource(GetModuleHandle(0), L"FONT_JSON", L"DATA", d);
	d.resize(d.size() + 1);
	using nlohmann::json;
	json j = json::parse(d.data());
	for (auto& item : j.items())
	{
		auto name = item.key();
		auto value = item.value();
		if (value.is_number_integer())
		{
			std::wstring name2 = XML3::XMLU(name.c_str()).wc();
			// this is like ic_fluent_add_28_regular
			// remove "ic_fluent_" and "_regular"
			if (name2.find(L"ic_fluent_") == 0)
				name2 = name2.substr(10);
			if (name2.find(L"_regular") != std::wstring::npos)
				name2 = name2.substr(0, name2.find(L"_regular"));
			m[name2] = value.get<int>();
		}
	}
	return m;
	*/
}

PIDLIST_ABSOLUTE GetParentPIDL(PCIDLIST_ABSOLUTE pidl)
{
	if (!pidl) return nullptr;

	SIZE_T size = ILGetSize(pidl);
	if (size <= sizeof(USHORT)) return nullptr; // Already root

	PIDLIST_ABSOLUTE parent = ILClone(pidl);
	if (!parent) return nullptr;

	ILRemoveLastID(parent); // Removes last child node
	return parent;
}


PIDLIST_ABSOLUTE DeserializePIDL(winrt::Windows::Storage::Streams::IBuffer const& buffer)
{
	if (!buffer) return nullptr;

	auto size = buffer.Length();
	if (size == 0) return nullptr;

	auto data = buffer.data(); // requires C++/WinRT
	auto pidl = reinterpret_cast<ITEMIDLIST*>(CoTaskMemAlloc(size));
	if (pidl)
	{
		memcpy(pidl, data, size);
	}
	return pidl;
}
winrt::Windows::Storage::Streams::IBuffer SerializePIDL(PCIDLIST_ABSOLUTE pidl)
{
	if (!pidl) return nullptr;

	UINT size = ILGetSize(pidl); // Includes null terminator
	auto buffer = winrt::Windows::Storage::Streams::Buffer(size);
	buffer.Length(size);

	memcpy(buffer.data(), pidl, size); // if not using writer
	return buffer;
}


PIDLIST_ABSOLUTE PidlFromPath(const wchar_t* path)
{
	PIDLIST_ABSOLUTE pidl = nullptr;
	SFGAOF sfgao;

	HRESULT hr = SHParseDisplayName(path, nullptr, &pidl, 0, &sfgao);
	if (SUCCEEDED(hr))
		return pidl;
	return nullptr;
}


winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage ImageFromIcon(HICON hIcon,HBITMAP hBitmap)
{
	using namespace winrt;
	using namespace Windows::Storage::Streams;
	using namespace Windows::Graphics::Imaging;

	CComPtr<IWICImagingFactory> wicFactory;
	CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&wicFactory));

	CComPtr<IWICBitmap> wicBitmap;
	if (hIcon)
		winrt::check_hresult(wicFactory->CreateBitmapFromHICON(hIcon, &wicBitmap));
	if (hBitmap)
		winrt::check_hresult(wicFactory->CreateBitmapFromHBITMAP(hBitmap, nullptr, WICBitmapUseAlpha, &wicBitmap));

	CComPtr<IWICStream> wicStream;
	winrt::check_hresult(wicFactory->CreateStream(&wicStream));

	CComPtr<IStream> stream;
	CreateStreamOnHGlobal(nullptr, TRUE, &stream);
	winrt::check_hresult(wicStream->InitializeFromIStream(stream.p));

	CComPtr<IWICBitmapEncoder> encoder;
	winrt::check_hresult(wicFactory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder));
	winrt::check_hresult(encoder->Initialize(wicStream.p, WICBitmapEncoderNoCache));

	CComPtr<IWICBitmapFrameEncode> frame;
	CComPtr<IPropertyBag2> props;
	winrt::check_hresult(encoder->CreateNewFrame(&frame, &props));
	winrt::check_hresult(frame->Initialize(props.p));
	winrt::check_hresult(frame->WriteSource(wicBitmap.p, nullptr));
	winrt::check_hresult(frame->Commit());
	winrt::check_hresult(encoder->Commit());

	// Rewind stream
	LARGE_INTEGER pos = {};
	stream->Seek(pos, STREAM_SEEK_SET, nullptr);

	// Convert to IRandomAccessStream
	CComPtr<IStream> comStream = stream;
	IRandomAccessStream winrtStream = nullptr;
	CreateRandomAccessStreamOverStream(
		comStream.p, BSOS_DEFAULT, guid_of<IRandomAccessStream>(), winrt::put_abi(winrtStream));

	winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage bmpImg;
	bmpImg.SetSource(winrtStream);
	return bmpImg;
}


HBITMAP GetIconFromPath(const std::wstring& path, int size = 32)
{
	CComPtr<IShellItem> shellItem;
	HRESULT hr = SHCreateItemFromParsingName(path.c_str(), nullptr, IID_PPV_ARGS(&shellItem));
	if (FAILED(hr)) 
		return nullptr;

	CComPtr<IShellItemImageFactory> imageFactory;
	imageFactory = shellItem;
	if (!imageFactory) 
		return nullptr;

	SIZE iconSize = { size, size };
	HBITMAP hBitmap = nullptr;
	SIIGBF flags = SIIGBF_ICONONLY | SIIGBF_BIGGERSIZEOK;
	hr = imageFactory->GetImage(iconSize, flags, &hBitmap);
	if (FAILED(hr) || !hBitmap) 
		return nullptr;

	return hBitmap;

/*	// Convert HBITMAP to HICON
	ICONINFO iconInfo = {};
	iconInfo.fIcon = TRUE;
	iconInfo.hbmColor = hBitmap;
	iconInfo.hbmMask = hBitmap;

	HICON hIcon = CreateIconIndirect(&iconInfo);

	DeleteObject(hBitmap); // Cleanup

	return hIcon;
	*/
}

bool IsPlainFile(IShellFolder* parentFolder, ITEMIDLIST* pidlItem)
{
	DWORD attrs = SFGAO_STREAM | SFGAO_FILESYSTEM;

	if (SUCCEEDED(parentFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlItem, &attrs)))
	{
		return (attrs & SFGAO_STREAM) && (attrs & SFGAO_FILESYSTEM);
	}
	return false;
}



void EnumerateChildren(ITEMIDLIST* root,std::vector<CHILD_ITEM>& r,bool SkipIcon = false)
{
	// Enumerate children
	CComPtr<IShellFolder> pDesktop;
	SHGetDesktopFolder(&pDesktop);

	// Bind to the folder
	CComPtr<IShellFolder> pFolder;
	pDesktop->BindToObject(root, nullptr, IID_PPV_ARGS(&pFolder));
	if (!pFolder)
		pFolder = pDesktop;

	// Enumerate items in the folder
	CComPtr<IEnumIDList> pEnum;
	pFolder->EnumObjects(nullptr, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN, &pEnum);

	LPITEMIDLIST pidlItem;
	ULONG fetched = 0;

	if (!pEnum)
		return;	
	while (pEnum->Next(1, &pidlItem, &fetched) == S_OK) {
		STRRET str;
		pFolder->GetDisplayNameOf(pidlItem, SHGDN_NORMAL, &str);
		CHILD_ITEM ci;

		wchar_t name[MAX_PATH] = {};
		StrRetToBufW(&str, pidlItem, name, MAX_PATH);


	
		MYPIDL fullPidl;
		#pragma warning(disable:4090)
		fullPidl.reset(ILCombine(root, pidlItem));
		wchar_t path[MAX_PATH] = {};
		SHGetPathFromIDListW(fullPidl.get(), path);

		if (IsPlainFile(pFolder, pidlItem))
		{
			// It's a plain file
			ci.Type = 0; // File
		}
		else
		{
			// It's not a plain file, check if it's a folder
			ci.Type = 1; // Folder
		}

	/*	// Is it a folder?
		CComPtr<IShellFolder> pFolder2;
		pFolder->BindToObject(pidlItem, nullptr, IID_PPV_ARGS(&pFolder2));
		if (pFolder2)
		{
			CComPtr<IShellFolder2> pFolder22;
			pFolder22 = pFolder2;
			if (pFolder22)
			{
				// Check if it's a directory
				VARIANT vtIsDir;
				if (SUCCEEDED(pFolder22->GetDetailsEx(pidlItem, &PKEY_FileAttributes, &vtIsDir)))
				{
					if (vtIsDir.vt == VT_UI4)
					{
						bool isDirectory = (vtIsDir.ulVal & FILE_ATTRIBUTE_DIRECTORY) != 0;
						if (isDirectory)
							ci.Type = 1;
						else
							ci.Type = 0;
					}
					VariantClear(&vtIsDir);
				}
			}
		}
		*/
	
		// Icon to BitmapImage
		try
		{
			if (SkipIcon == 0)
			{
				auto bi = GetIconFromPath(path);
				if (bi)
				{
					auto ifi = ImageFromIcon(nullptr, bi);
					if (ifi)
						ci.icon = ifi;
					else
					{
					}
				}
			}
		}
		catch (...)
		{

		}

	

#pragma warning(disable:4090)
		ci.Parent = pFolder;
		ci.pidl.reset(pidlItem);
		ci.abspidl.reset(ILCombine(root, pidlItem));
		ci.displname = name;
		r.push_back(ci);
	}

}

void GetTheTree(ITEMIDLIST* root, ITEMIDLIST* end, XML3::XMLElement& R,int Deep)
{
	if (1)
		return;

	// Get shell folders
	CComPtr<IShellFolder> pDesktop;
	SHGetDesktopFolder(&pDesktop);

	// Get display name of current root
	STRRET str;
	pDesktop->GetDisplayNameOf(root, SHGDN_NORMAL, &str);
	wchar_t name[MAX_PATH] = {};
	StrRetToBufW(&str, root, name, MAX_PATH);
	R.vv("n").SetWideValue(name);

	// Bind to folder
	CComPtr<IShellFolder> pFolder;
	HRESULT hr = pDesktop->BindToObject(root, nullptr, IID_PPV_ARGS(&pFolder));
	if (FAILED(hr) || !pFolder)
		pFolder = pDesktop;

	// Enumerate children
	std::vector<CHILD_ITEM> children;
	EnumerateChildren(root, children, true);

	for (auto& chi : children)
	{
		if (chi.Type != 1) continue; // only folders

		MYPIDL childFull;
		childFull.reset(ILCombine(root, chi.pidl.get()));
		if (!childFull)
			continue;

		// Only descend if child is part of the path to `end`
		if (ILIsEqual(childFull.get(), end) || ILIsParent(childFull.get(), end, FALSE))
		{
			auto& el = R.AddElement("e");
			el.vv("n").SetWideValue(chi.displname.c_str());

			if (ILIsEqual(childFull.get(), end))
			{
				break; // reached target, stop descending
			}

			// recurse
			GetTheTree(childFull.get(), end, el,Deep + 1);

		}
	}
}

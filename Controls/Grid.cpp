#include "pch.h"
#include "property.hpp"

using namespace winrt::Microsoft::UI::Xaml::Controls;
class ITEM_GRID : public XITEM_Panel
{
public:

	ITEM_GRID()
	{
		ElementName = L"Grid";
		X = winrt::Microsoft::UI::Xaml::Controls::Grid();
	}


	virtual void ApplyProperties()
	{
		XITEM_Panel::ApplyProperties();
		try
		{
			auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
			if (!e)
				return;
			UINT Prev_RowCount = e.RowDefinitions().Size();
			UINT Prev_ColCount = e.ColumnDefinitions().Size();
			for (auto& p : properties)
			{
				if (p->n == L"# Rows")
				{
					auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (op)
					{
						while (e.RowDefinitions().Size() < (unsigned int)op->value)
						{
							e.RowDefinitions().Append(winrt::Microsoft::UI::Xaml::Controls::RowDefinition());
						}
						while (e.RowDefinitions().Size() > (unsigned int)op->value)
						{
							e.RowDefinitions().RemoveAt(e.RowDefinitions().Size() - 1);
						}
					}
				}
				if (p->n == L"# Columns")
				{
					auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
					if (op)
					{
						while (e.ColumnDefinitions().Size() < (unsigned int)op->value)
						{
							e.ColumnDefinitions().Append(winrt::Microsoft::UI::Xaml::Controls::ColumnDefinition());
						}
						while (e.ColumnDefinitions().Size() > (unsigned int)op->value)
						{
							e.ColumnDefinitions().RemoveAt(e.ColumnDefinitions().Size() - 1);
						}
					}
				}
			}

			// Again to set the Height and Width of RowDefinitions and ColumnDefinitions
			int sptype1idx = 0;
			int sptype2idx = 0;
			bool MustCompleteReload = 0;
			for (auto& p : properties)
			{
				if (p->special_type != 1)
					sptype1idx = 0;
				if (p->special_type != 2)
					sptype2idx = 0;
				if (p->special_type == 1)
				{
					auto rd = e.RowDefinitions().GetAt((unsigned int)sptype1idx);
					if (!rd)
					{
						MustCompleteReload = 1;
						break;
					}

					sptype1idx++;
					GridLength gl;
					auto opx = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (opx)
					{
						auto value = opx->value;
						if (value == L"Auto")
						{
							gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Auto;
						}
						else if (value.find(L"*") != std::wstring::npos)
						{
							gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Star;
							gl.Value = _wtof(value.substr(0, value.find(L'*')).c_str());
						}
						else
						{
							gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Pixel;
							gl.Value = _wtof(value.c_str());
						}
					}
					else
					{
						gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Auto;
					}
					if (rd)
						rd.Height(gl);

				}
				if (p->special_type == 2)
				{
					auto cd = e.ColumnDefinitions().GetAt((unsigned int)sptype2idx);
					if (!cd)
					{
						MustCompleteReload = 1;
						break;
					}
					sptype2idx++;
					GridLength gl;
					auto opx = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (opx)
					{
						auto value = opx->value;
						if (value == L"Auto")
						{
							gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Auto;
						}
						else if (value.find(L"*") != std::wstring::npos)
						{
							gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Star;
							gl.Value = _wtof(value.substr(0, value.find(L'*')).c_str());
						}
						else
						{
							gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Pixel;
							gl.Value = _wtof(value.c_str());
						}
					}
					else
					{
						gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Auto;
					}
					if (cd)
						cd.Width(gl);
				}
			}


			// again
			if (Prev_RowCount != e.RowDefinitions().Size() || Prev_ColCount != e.ColumnDefinitions().Size() || MustCompleteReload)
			{
				extern bool RefreshFromGrid;
				RefreshFromGrid = 1;
				index_before = 0;
//				properties.clear();
//				CreateProperties(nullptr);
				RemoveRDProperties();
				AddRDProperties();
				void PostRefresh();
				PostRefresh();
			}
		}
		catch (...)
		{

		}
	}
	std::map<std::wstring, std::shared_ptr<PROPERTY>> save_props;
	size_t index_before = 0;
	void RemoveRDProperties()
	{
		save_props.clear();
		auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
		if (!e)
			return;
		for (int i = (int)properties.size() - 1 ; i >= 0; i--)
		{
			auto& p = properties[i];
			if (p->special_type == 1 || p->special_type == 2)
			{
				if (index_before == 0)
					index_before = i - 1;
				save_props[properties[i]->n] = p; // Save it for later
				properties.erase(properties.begin() + i);
			}
		}


	}
	void AddRDProperties()
	{
		auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Grid>();
		for (size_t i = 0; i < e.RowDefinitions().Size(); i++)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"Grid";
			op->n = L"RowDefinition " + std::to_wstring(i + 1) + L" Height";

			auto it = save_props.find(op->n);
			if (it != save_props.end())
			{
				if (index_before > 0)
				{
					index_before++;
					if ((properties.begin() + index_before) >= properties.end())
						properties.push_back(it->second);
					else
						properties.insert(properties.begin() + index_before, it->second);
				}
				else
					properties.push_back(it->second);
				continue;
			}

			op->xmln = L"Height";
			op->special_type = 1;
			auto ch = e.RowDefinitions().GetAt((unsigned int)i).Height();
			if (ch.GridUnitType == GridUnitType::Pixel)
			{
				op->value = std::to_wstring(ch.Value);
			}
			if (ch.GridUnitType == GridUnitType::Star)
			{
				op->value = std::to_wstring((int)ch.Value) + L"*";
			}
			else
			{
				op->value = L"Auto";
			}
			// Find it in save_props perhaps
			properties.push_back(op);
		}
		for (size_t i = 0; i < e.ColumnDefinitions().Size(); i++)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"Grid";
			op->n = L"ColumnDefinition " + std::to_wstring(i + 1) + L" Width";

			auto it = save_props.find(op->n);
			if (it != save_props.end())
			{
				if (index_before > 0)
				{
					index_before++;
					properties.insert(properties.begin() + index_before, it->second);
				}
				else
					properties.push_back(it->second);
				continue;
			}

			op->xmln = L"Width";
			op->special_type = 2;
			auto ch = e.ColumnDefinitions().GetAt((unsigned int)i).Width();
			if (ch.GridUnitType == GridUnitType::Pixel)
			{
				op->value = std::to_wstring(ch.Value);
			}
			if (ch.GridUnitType == GridUnitType::Star)
			{
				op->value = std::to_wstring((int)ch.Value) + L"*";
			}
			else
			{
				op->value = L"Auto";
			}
			properties.push_back(op);
		}

	}

	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		if (!properties.empty())
			return properties;
		auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::Grid>();

		// Check el 
		if (el)
		{
			auto FindRD = el->FindElementZ("Grid.RowDefinitions");
			if (FindRD)
			{
				auto count = FindRD->GetChildrenNum();
				while (e.RowDefinitions().Size() < (unsigned int)count)
				{
					e.RowDefinitions().Append(winrt::Microsoft::UI::Xaml::Controls::RowDefinition());
				}
				while (e.RowDefinitions().Size() > (unsigned int)count)
				{
					e.RowDefinitions().RemoveAt(e.RowDefinitions().Size() - 1);
				}

				for (int i = 0; i < count; i++)
				{
					auto ch = FindRD->GetChildren()[i];
					auto rd = e.RowDefinitions().GetAt((unsigned int)i);
					auto FindH = ch->FindVariableZ("Height");
					if (FindH)
					{
						auto value = FindH->GetValue();
						GridLength gl;
						if (value == "Auto")
						{
							gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Auto;
						}
						else if (value.find("*") != std::wstring::npos)
						{
							gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Star;
							gl.Value = atof(value.substr(0, value.find('*')).c_str());
						}
						else
						{
							gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Pixel;
							gl.Value = atof(value.c_str());
						}
						rd.Height(gl);
					}
				}
			}

			auto FindCD = el->FindElementZ("Grid.ColumnDefinitions");
			if (FindCD)
			{
				auto count = FindCD->GetChildrenNum();
				while (e.ColumnDefinitions().Size() < (unsigned int)count)
				{
					e.ColumnDefinitions().Append(winrt::Microsoft::UI::Xaml::Controls::ColumnDefinition());
				}
				while (e.ColumnDefinitions().Size() > (unsigned int)count)
				{
					e.ColumnDefinitions().RemoveAt(e.ColumnDefinitions().Size() - 1);
				}
				for(int i = 0; i < count; i++)
				{
					auto ch = FindCD->GetChildren()[i];
					auto cd = e.ColumnDefinitions().GetAt((unsigned int)i);
					auto FindW = ch->FindVariableZ("Width");
					if (FindW)
					{
						auto value = FindW->GetValue();
						GridLength gl;
						if (value == "Auto")
						{
							gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Auto;
						}
						else if (value.find("*") != std::wstring::npos)
						{
							gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Star;
							gl.Value = atof(value.substr(0, value.find('*')).c_str());
						}
						else
						{
							gl.GridUnitType = winrt::Microsoft::UI::Xaml::GridUnitType::Pixel;
							gl.Value = atof(value.c_str());
						}
						cd.Width(gl);
					}
				}
			}
		}

		if (1)
		{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->S = 1;
			op->g = L"Grid";
			op->n = L"# Rows";
			op->xmln = L"*";
			op->mmin = 0.0;
			op->mmax = 20.0;
			op->smallchange = 1;
			op->largechange = 1;
			op->value = (double)e.RowDefinitions().Size();	
			op->def = 0.0;
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
			op->g = L"Grid";
			op->n = L"# Columns";
			op->xmln = L"*";
			op->mmin = 0.0;
			op->mmax = 20.0;
			op->smallchange = 1;
			op->largechange = 1;
			op->value = (double)e.ColumnDefinitions().Size();
			op->def = 0.0;
			properties.push_back(op);
		}
		AddRDProperties();

		auto p2 = XITEM_Panel::CreateProperties(el);
		for (auto& p : p2)
			properties.push_back(p);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = Grid();
		the_par = par;

		

		auto b = X.try_as<Grid>();
/*		b.SizeChanged([this](winrt::Windows::Foundation::IInspectable const& grd, winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const&)
		{
			auto b = grd.try_as<Grid>();
			auto cw = b.ActualWidth();
			auto ch = b.ActualHeight();
			
			// check if any child is WebView2 in which case we need to resize it
			for (auto child : b.Children())
			{
				auto webview = child.try_as<winrt::Microsoft::UI::Xaml::Controls::WebView2>();
				if (webview)
				{
					webview.Width(cw);
					webview.Height(ch);
				}
			}

		});
		*/

		// add a default 2x2
		b.ColumnDefinitions().Append(winrt::Microsoft::UI::Xaml::Controls::ColumnDefinition());
		b.ColumnDefinitions().Append(winrt::Microsoft::UI::Xaml::Controls::ColumnDefinition());
		b.RowDefinitions().Append(winrt::Microsoft::UI::Xaml::Controls::RowDefinition());
		b.RowDefinitions().Append(winrt::Microsoft::UI::Xaml::Controls::RowDefinition());
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		b.Tag(box_value((long long)this));
		if (ForWhat == 0)
			b.Background(SolidColorBrush(Colors::Transparent()));
		return b;
	}

	void Select()
	{
		auto b = X.try_as<Grid>();
		if (!b)
			return;
		b.BorderBrush(SolidColorBrush(Colors::Red()));
		b.BorderThickness(ThicknessHelper::FromLengths(2.0, 2.0, 2.0, 2.0));
	}
	void Unselect()
	{
		auto b = X.try_as<Grid>();
		if (!b)
			return;
		b.BorderBrush(nullptr);
		b.BorderThickness(ThicknessHelper::FromLengths(0, 0, 0, 0));

	}

};

std::shared_ptr<XITEM> CreateXItemGrid()
{
	return std::make_shared< ITEM_GRID>();
}

bool ISXItemGrid(XITEM* xit)
{
	if (!xit)
		return false;
	return dynamic_cast<ITEM_GRID*>(xit) != nullptr;
}

#include "pch.h"
#include "property.hpp"

void XITEM_UIElement::ApplyProperties()
{
	XITEM::ApplyProperties();
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::UIElement>();
	if (!e)
		return;
	try
	{
		for (auto& p : properties)
		{
			if (p->n == L"CanDrag")
			{
				auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
				if (op)
				{
					e.CanDrag(op->SelectedIndex);
				}
			}
			if (p->n == L"AllowDrop")
			{
				auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
				if (op)
				{
					e.AllowDrop(op->SelectedIndex);
				}
			}
			if (p->n == L"Opacity")
			{
				auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
				if (op)
				{
					e.Opacity(op->value);
				}
			}
			if (p->n == L"Rotation")
			{
				// Exclude if RatingControl or NVItem
				if (e.try_as<winrt::Microsoft::UI::Xaml::Controls::RatingControl>())
					continue;
				if (e.try_as<winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem>())
					continue;
				auto op = std::dynamic_pointer_cast<DOUBLE_PROPERTY>(p);
				if (op)
				{
					auto wi = e.ActualSize();
					e.CenterPoint({ wi.x/2, wi.y/2, 0 });
					e.Rotation((float)op->value);
				}
			}
			if (p->n == L"Scale")
			{
				if (e.try_as<winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem>())
					continue;
				auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
				if (op)
				{
					auto parts = split(op->value.c_str(), L',');
					if (parts.size() == 3)
					{
						float x = std::stof(parts[0]);
						float y = std::stof(parts[1]);
						float z = std::stof(parts[2]);
						if (x > 0 && y > 0 && z > 0)
							e.Scale({ x, y, z });
					}
				}
			}
			if (p->n == L"Translation")
			{
				if (e.try_as<winrt::Microsoft::UI::Xaml::Controls::NavigationViewItem>())
					continue;
				auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
				if (op)
				{
					auto parts = split(op->value.c_str(), L',');
					if (parts.size() == 3)
					{
						float x = std::stof(parts[0]);
						float y = std::stof(parts[1]);
						float z = std::stof(parts[2]);
						if (x > 0 && y > 0 && z > 0)
							e.Translation({ x, y, z });
					}
				}
			}
		}
	}
	catch (...)
	{
	}
}



std::vector<std::shared_ptr<PROPERTY>> XITEM_UIElement::CreateProperties(XML3::XMLElement* el)
{
	el;
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::UIElement>();
	if (!e)
		return {};
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->g = L"UIElement";
		op->n = L"Opacity";
		op->mmin = 0.0;
		op->mmax = 1.0;
		op->smallchange = 0.1;
		op->largechange = 0.1;
		op->value = e.Opacity();
		op->def = 1.0;
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<DOUBLE_PROPERTY> op = std::make_shared<DOUBLE_PROPERTY>();
		op->g = L"UIElement";
		op->n = L"Rotation";
		op->mmin = 0.0;
		op->mmax = 360.0;
		op->smallchange = 1;
		op->largechange = 10;
		op->value = e.Rotation();
		op->def = 0.0;
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
		op->g = L"UIElement";
		op->n = L"Scale";
		op->special_type = 3;
		wchar_t buf[100];
		auto scl = e.Scale();
		swprintf_s(buf, 100, L"%.2f,%.2f,%.2f", scl.x, scl.y,scl.z);
		op->value = buf;
		op->def = L"1,1,1";
		p.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
		op->g = L"UIElement";
		op->n = L"Translation";
		op->special_type = 4;
		wchar_t buf[100];
		auto scl = e.Translation();
		swprintf_s(buf, 100, L"%.2f,%.2f,%.2f", scl.x, scl.y, scl.z);
		op->value = buf;
		op->def = L"1,1,1";
		p.push_back(op);
	}
	if (1)
	{

		std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
		op->g = L"UIElement";
		op->n = L"AllowDrop";
		op->SelectedIndex = e.AllowDrop();
		p.push_back(op);
	}
	if (1)
	{

		std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
		op->g = L"UIElement";
		op->n = L"CanDrag";
		op->SelectedIndex = e.CanDrag();
		p.push_back(op);
	}



	return p;
}

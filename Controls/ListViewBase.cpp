#include "pch.h"
#include "property.hpp"


void XITEM_ListViewBase::ApplyProperties()
{
	XITEM_Selector::ApplyProperties();
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::ListViewBase>();
	if (!e)
		return;

	for (auto& p : properties)
	{
		if (p->n == L"CanDragItems")
		{
			auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
			if (op)
			{
				e.CanDragItems(op->SelectedIndex);
			}
		}
		if (p->n == L"CanReorderItems")
		{
			auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
			if (op)
			{
				e.CanReorderItems(op->SelectedIndex);
			}
		}
		if (p->n == L"IsItemClickEnabled")
		{
			auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
			if (op)
			{
				e.IsItemClickEnabled(op->SelectedIndex);
			}
		}
		if (p->n == L"Header")
		{
			auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
			if (op)
			{
				e.Header(winrt::box_value(op->value));
			}
		}
		if (p->n == L"Footer")
		{
			auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
			if (op)
			{
				e.Footer(winrt::box_value(op->value));
			}
		}
	}
}



std::vector<std::shared_ptr<PROPERTY>> XITEM_ListViewBase::CreateProperties(XML3::XMLElement* el)
{
	el;
	auto e = X.try_as<winrt::Microsoft::UI::Xaml::Controls::ListViewBase>();
	std::vector<std::shared_ptr<PROPERTY>> p;
	if (!e)
		return p;

	if (1)
	{

		std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
		op->g = L"ListViewBase";
		op->S = 1; // Since items deriving from this don't have any other properties
		op->n = L"CanDragItems";
		op->SelectedIndex = e.CanDragItems();
		properties.push_back(op);
	}

	if (1)
	{

		std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
		op->g = L"ListView";
		op->n = L"CanReorderItems";
		op->SelectedIndex = e.CanReorderItems();
		properties.push_back(op);
	}

	if (1)
	{
		std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
		op->g = L"ListView";
		op->n = L"IsItemClickEnabled";
		op->SelectedIndex = e.IsItemClickEnabled();
		properties.push_back(op);
	}


	if (1)
	{
		std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
		op->g = L"ListViewBase";
		op->n = L"Header";
		try
		{
			op->value = winrt::unbox_value<winrt::hstring>(e.Header());
		}
		catch (...)
		{

		}
		properties.push_back(op);
	}
	if (1)
	{
		std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
		op->g = L"ListViewBase";
		op->n = L"Footer";
		try
		{
			op->value = winrt::unbox_value<winrt::hstring>(e.Footer());
		}
		catch (...)
		{

		}
		properties.push_back(op);
	}

	// Add properties from Selector
	auto p2 = XITEM_Selector::CreateProperties(el);
	for (auto& pp : p2)
		p.push_back(pp);

	return p;
}

#include "pch.h"
#include "property.hpp"

class ITEM_ColorPicker : public XITEM_Control
{
public:

	ITEM_ColorPicker()
	{
		ElementName = L"ColorPicker";
		X = ColorPicker();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();
		auto e = X.as<ColorPicker>();
		if (!e)
			return;
		for (auto& p : properties)
		{
			if (p->n == L"ColorSpectrumShape")
			{
				auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
				if (op)
				{
					e.ColorSpectrumShape((winrt::Microsoft::UI::Xaml::Controls::ColorSpectrumShape)op->SelectedIndex);
				}
			}
			if (p->n == L"IsAlphaEnabled")
			{
				auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
				if (op)
				{
					e.IsAlphaEnabled(op->SelectedIndex);
				}
			}
			if (p->n == L"Color")
			{
				auto op = std::dynamic_pointer_cast<COLOR_PROPERTY>(p);
				if (op)
				{
					e.Color(op->value);
				}
			}
			if (p->n == L"IsColorPreviewVisible")
			{
				auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
				if (op)
				{
					e.IsColorPreviewVisible(op->SelectedIndex);
				}
			}
			if (p->n == L"IsColorSliderVisible")
			{
				auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
				if (op)
				{
					e.IsColorSliderVisible(op->SelectedIndex);
				}
			}
			if (p->n == L"IsColorSpectrumVisible")
			{
				auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
				if (op)
				{
					e.IsColorSpectrumVisible(op->SelectedIndex);
				}
			}
			if (p->n == L"IsMoreButtonVisible")
			{
				auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
				if (op)
				{
					e.IsMoreButtonVisible(op->SelectedIndex);
				}
			}
			if (p->n == L"IsHexInputVisible")
			{
				auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
				if (op)
				{
					e.IsHexInputVisible(op->SelectedIndex);
				}
			}
			if (p->n == L"Orientation")
			{
				auto op = std::dynamic_pointer_cast<LIST_PROPERTY>(p);
				if (op)
				{
					e.Orientation((winrt::Microsoft::UI::Xaml::Controls::Orientation)op->SelectedIndex);
				}
			}
		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		auto e = X.as<ColorPicker>();
		if (!properties.empty())
			return properties;

		if (1)
		{
			std::shared_ptr<COLOR_PROPERTY> op = std::make_shared<COLOR_PROPERTY>();
			op->g = L"ColorPicker";
			op->S = 1;
			op->n = L"Color";
			try
			{
				auto c = e.Color();
				op->value = c;
			}
			catch (...)
			{
			}
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->S = 1;
			op->g = L"ColorPicker";
			op->n = L"ColorSpectrumShape";
			op->Items = { L"Box", L"Ring" };
			op->DefaultIndex = 0;
			op->SelectedIndex = (size_t)e.ColorSpectrumShape();
			properties.push_back(op);

		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"ColorPicker";
			op->n = L"IsAlphaEnabled";
			op->DefaultIndex = 1;
			op->SelectedIndex = e.IsAlphaEnabled();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"ColorPicker";
			op->n = L"IsColorPreviewVisible";
			op->DefaultIndex = 1;
			op->SelectedIndex = e.IsColorPreviewVisible();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"ColorPicker";
			op->n = L"IsColorSliderVisible";
			op->DefaultIndex = 1;
			op->SelectedIndex = e.IsColorSliderVisible();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"ColorPicker";
			op->n = L"IsColorSpectrumVisible";
			op->DefaultIndex = 1;
			op->SelectedIndex = e.IsColorSpectrumVisible();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"ColorPicker";
			op->n = L"IsMoreButtonVisible";
			op->DefaultIndex = 1;
			op->SelectedIndex = e.IsMoreButtonVisible();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"ColorPicker";
			op->n = L"IsHexInputVisible";
			op->DefaultIndex = 1;
			op->SelectedIndex = e.IsHexInputVisible();
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<LIST_PROPERTY> op = std::make_shared<LIST_PROPERTY>();
			op->S = 1;
			op->g = L"ColorPicker";
			op->n = L"Orientation";
			op->Items = { L"Vertical", L"Horizontal" };
			op->DefaultIndex = 0;
			op->SelectedIndex = (size_t)e.Orientation();
			properties.push_back(op);

		}


		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = ColorPicker();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<ColorPicker>();
		b.Tag(box_value((long long)this));
		return b;
	}
};


std::shared_ptr<XITEM> CreateXItemColorPicker()
{
	return std::make_shared< ITEM_ColorPicker>();
}

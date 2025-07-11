// 
#pragma once
#include "property.hpp"
class PROJECT : public SERIALIZABLE
{
	public:

		std::shared_ptr<XML3::XML> xfile;
		std::wstring file;
		winrt::VisualWinUI3::MainPage mp;
		winrt::VisualWinUI3::BlankWindow bw;
		std::shared_ptr<XITEM> root;

		virtual void Ser(XML3::XMLElement& el) override
		{
			if (root)
			{
				XML3::XMLElement& rootEl = el.AddElement("root");
				root->Ser(rootEl);
			}
		}
		virtual void Unser(XML3::XMLElement& el) override
		{
			auto eln = el.GetElementName();
			if (eln == "StackPanel")	
				root = CreateXItemStackPanel();
			if (eln == "SwapChainPanel")
				root = CreateXItemSwapChainPanel();
			if (eln == "Grid")
				root = CreateXItemGrid();
			if (eln == "Canvas")
				root = CreateXItemCanvas();
			if (eln == "RelativePanel")
				root = CreateXItemRelativePanel();
			if (eln == "NavigationView")
				root = CreateXItemNavigationView();
			if (root)
				root->Unser(el);
		}

};
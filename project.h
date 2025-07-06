// 
#include "property.hpp"
class PROJECT : public SERIALIZABLE
{
	public:

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
			if (eln == "Grid")
				root = CreateXItemGrid();
			if (root)
				root->Unser(el);
		}

};
#include "pch.h"
#include "property.hpp"

class ITEM_MediaPlayerElement : public XITEM_Control
{
public:

	ITEM_MediaPlayerElement()
	{
		ElementName = L"MediaPlayerElement";
		X = MediaPlayerElement();
	}


	virtual void ApplyProperties()
	{
		XITEM_Control::ApplyProperties();

		using namespace winrt;
		using namespace Windows::Media::Playback;
		using namespace Windows::Media::Core;
		using namespace Microsoft::UI::Xaml::Controls;
		try
		{
			auto e = X.try_as<MediaPlayerElement>();
			for (auto& p : properties)
			{
				if (p->n == L"AutoPlay")
				{
					auto op = std::dynamic_pointer_cast<BOOL_PROPERTY>(p);
					if (op)
					{
						e.AutoPlay(op->SelectedIndex);
					}
				}

				if (p->n == L"Source")
				{
					auto op = std::dynamic_pointer_cast<STRING_PROPERTY>(p);
					if (op)
					{
						if (op->value.length())
						{
							winrt::Windows::Foundation::Uri uri{ op->value };
							MediaSource source = MediaSource::CreateFromUri(uri);
							e.Source(source);
						}
					}
				}
			}

		}
		catch (...)
		{

		}
	}


	virtual std::vector<std::shared_ptr<PROPERTY>> CreateProperties(XML3::XMLElement* el) override
	{
		properties.clear();
		auto e = X.as<MediaPlayerElement>();
		if (1)
		{
			std::shared_ptr<BOOL_PROPERTY> op = std::make_shared<BOOL_PROPERTY>();
			op->g = L"MediaPlayerElement";
			op->n = L"AutoPlay";
			op->DefaultIndex = 0;
			op->SelectedIndex = (e.AutoPlay()) ? 1 : 0;
			properties.push_back(op);
		}
		if (1)
		{
			std::shared_ptr<STRING_PROPERTY> op = std::make_shared<STRING_PROPERTY>();
			op->g = L"MediaPlayerElement";
			op->n = L"Source";
			properties.push_back(op);

		}
		auto p2 = XITEM_Control::CreateProperties(el);
		for (auto& pp : p2)
			properties.push_back(pp);
		return properties;
	}

	virtual winrt::Microsoft::UI::Xaml::UIElement Create(int ForWhat, XITEM* par) override
	{
		X = MediaPlayerElement();
		the_par = par;
		if (properties.empty())
			properties = CreateProperties(nullptr);

		if (ForWhat == 0)
		{
			AllTap(X);
		}

		auto b = X.as<MediaPlayerElement>();
		b.MinWidth(200);
		b.MinHeight(200);
		b.Tag(box_value((long long)this));

		winrt::Windows::Foundation::Uri uri{ L"https://videos.pexels.com/video-files/14952575/14952575-uhd_1440_2560_30fps.mp4" }; 

		using namespace winrt;
		using namespace Windows::Media::Playback;
		using namespace Windows::Media::Core;
		using namespace Microsoft::UI::Xaml::Controls;

		MediaSource source = MediaSource::CreateFromUri(uri);
		b.Source(source);
		return b;
	}


};


std::shared_ptr<XITEM> CreateXItemMediaPlayerElement()
{
	return std::make_shared< ITEM_MediaPlayerElement>();
}

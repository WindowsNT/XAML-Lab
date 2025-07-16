#pragma once
#define DISABLE_XAML_GENERATED_MAIN
#include <windows.h>
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>
#include <queue>
#include <any>
#include <optional>
#include <stack>
#include <mutex>
#include <vector>
#include <dwrite.h>
#include <set>
#include <unordered_set>
#include <stack>
#include <functional>
#include <memory>
#include <limits>
#include <vector>
#include <shlobj.h>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <filesystem>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <random>
#include <dwmapi.h>
#include <uxtheme.h>
#include <atlbase.h>
#include <wincodec.h>


// Undefine GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#undef GetCurrentTime

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <microsoft.ui.xaml.window.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <microsoft.ui.xaml.media.dxinterop.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.Windows.AppNotifications.h>
#include <winrt/Microsoft.Windows.AppNotifications.Builder.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <wil/cppwinrt_helpers.h>
#include <appmodel.h>
#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Foundation.h>

#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>

#include "xml3all.h"
#include "json.hpp"

extern const wchar_t* ttitle;
extern std::shared_ptr<XML3::XML> SettingsX;
extern std::wstring datafolder;
extern std::wstring ToOpenFile;
const wchar_t* s(size_t);
winrt::Windows::Foundation::IInspectable PageFromHWND(HWND hh);
winrt::Windows::Foundation::IInspectable PageFromWindow(winrt::Windows::Foundation::IInspectable);


inline std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}


inline std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
inline std::vector<std::wstring>& split(const std::wstring& s, wchar_t delim, std::vector<std::wstring>& elems) {
    std::wstringstream ss(s);
    std::wstring item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}


inline std::vector<std::wstring> split(const std::wstring& s, wchar_t delim)
{
    std::vector<std::wstring> elems;
    split(s, delim, elems);
    return elems;
}

inline bool ExportForXAML = 0;


winrt::Windows::Storage::Streams::IBuffer SerializePIDL(PCIDLIST_ABSOLUTE pidl);
PIDLIST_ABSOLUTE DeserializePIDL(winrt::Windows::Storage::Streams::IBuffer const& buffer);
PIDLIST_ABSOLUTE GetParentPIDL(PCIDLIST_ABSOLUTE pidl);
PIDLIST_ABSOLUTE PidlFromPath(const wchar_t* path);


#pragma warning(disable:4090)
class MYPIDL {
public:
    // Default constructor
    MYPIDL() noexcept : m_pidl(nullptr) {}

    // Constructor from raw pointer (takes ownership)
    explicit MYPIDL(ITEMIDLIST* pidl) noexcept : m_pidl(pidl) {}

    // Copy constructor (deep copy)
    MYPIDL(const MYPIDL& other) {
        m_pidl = other.m_pidl ? ILClone(other.m_pidl) : nullptr;
    }

    // Move constructor
    MYPIDL(MYPIDL&& other) noexcept : m_pidl(other.m_pidl) {
        other.m_pidl = nullptr;
    }

    // Copy assignment
    MYPIDL& operator=(const MYPIDL& other) {
        if (this != &other) {
            Free();
            m_pidl = other.m_pidl ? ILClone(other.m_pidl) : nullptr;
        }
        return *this;
    }

    // Move assignment
    MYPIDL& operator=(MYPIDL&& other) noexcept {
        if (this != &other) {
            Free();
            m_pidl = other.m_pidl;
            other.m_pidl = nullptr;
        }
        return *this;
    }

    // Destructor
    ~MYPIDL() {
        Free();
    }

    // Get the raw pointer
    ITEMIDLIST* get() const noexcept {
        return m_pidl;
    }

    // Release ownership and return the raw pointer
    ITEMIDLIST* release() noexcept {
        ITEMIDLIST* temp = m_pidl;
        m_pidl = nullptr;
        return temp;
    }

    // Reset to a new PIDL (takes ownership)
    void reset(ITEMIDLIST* pidl = nullptr) {
        if (m_pidl != pidl) {
            Free();
            m_pidl = pidl;
        }
    }

    // Check if valid
    explicit operator bool() const noexcept {
        return m_pidl != nullptr;
    }

private:
    void Free() {
        if (m_pidl) {
            ILFree(m_pidl);  // CoTaskMemFree also works, but ILFree is safer
            m_pidl = nullptr;
        }
    }

    ITEMIDLIST* m_pidl;
};
#pragma warning(default:4090)

struct CHILD_ITEM
{
    MYPIDL pidl;
    int Type = 0; // 0 file 1 folder etc
    std::wstring displname;
    unsigned long long sz = 0;
    FILETIME ft = {};
    std::wstring type;
    std::any icon;
};



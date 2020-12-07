
#include <functional>
#include <string>
#include <wtypes.h>
#include <minwinbase.h>

namespace Traverse
{
  using Callback = std::function<bool(const std::wstring& path, const WIN32_FIND_DATA& data, bool children)>;

  void ProcessTree(const std::wstring& path, Callback f);

  bool TraverseTest(const std::wstring& path, const WIN32_FIND_DATA& data, bool children); // dummy for testing
  inline Callback CB{ &TraverseTest };

}

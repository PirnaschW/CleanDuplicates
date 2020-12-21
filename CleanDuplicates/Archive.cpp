
#include "pch.h"

CArchive& operator<<(CArchive& ar, const std::wstring& s)
{
  size_t len = s.size() * sizeof s[0];
  ar << len;
  ar.Write(&s[0], len);
  return ar;
}
CArchive& operator>>(CArchive& ar, std::wstring& s)
{
  size_t len;
  ar >> len;
  s.resize(len / sizeof s[0]);
  ar.Read(&s[0], len);
  return ar;
}

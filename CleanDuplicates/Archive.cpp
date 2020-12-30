
#include "pch.h"

CArchive& operator<<(CArchive& ar, const std::wstring& s)
{
  size_t len64 = s.size() * sizeof s[0];
  if (len64 > INT_MAX) throw;
  unsigned int len = static_cast<unsigned int>(len64);
  ar << len;   // save only an unsigned int, larger is not possible anyway
  ar.Write(&s[0], len);
  return ar;
}
CArchive& operator>>(CArchive& ar, std::wstring& s)
{
  unsigned int len;
  ar >> len;
  s.resize(len / sizeof s[0]);
  ar.Read(&s[0], len);
  return ar;
}

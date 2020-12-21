
template<class T>
CArchive& operator<<(CArchive& ar, const std::vector<T>& v)
{
  size_t n = v.size();
  ar << n;
  for (size_t i = 0; i < n; ++i) ar << v[i];
  return ar;
}

template<class T>
CArchive& operator>>(CArchive& ar, std::vector<T>& v)
{
  size_t n{};
  ar >> n;
  v.resize(n);
  for (size_t i = 0; i < n; ++i) ar >> v[i];
  return ar;
}

CArchive& operator<<(CArchive& ar, const std::wstring& s);
CArchive& operator>>(CArchive& ar, std::wstring& s);

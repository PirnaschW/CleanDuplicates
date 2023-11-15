
namespace FileMap
{

  using MD5Hash = std::wstring;

  struct FileKey
  {
    std::uintmax_t size;
    MD5Hash hash;
  };

  struct FileData
  {
    size_t rank;
    std::filesystem::directory_entry d;
  };

  struct FileKeyHash { std::size_t operator()(const FileKey& k) const noexcept { return static_cast<size_t>(k.size); } };

  struct FileKeyEquality {
    bool operator()(const FileKey& Lhs, const FileKey& Rhs) const noexcept {
      if (Lhs.size != Rhs.size) return false;
      if (Lhs.hash == L"" || Rhs.hash == L"") return true;
      return Lhs.hash == Rhs.hash;
    }
  };

  using FMap = std::unordered_multimap<FileMap::FileKey, FileMap::FileData, FileMap::FileKeyHash, FileMap::FileKeyEquality>;

  FMap::const_iterator Insert(FMap& fmap, const std::filesystem::directory_entry& d, bool saveHash);

}


#include "pch.h"

namespace FileMap
{

  namespace
  {
    MD5Hash GetMD5Hash(const std::filesystem::directory_entry& d)
    {
      MD5::MD5 md5;
      std::wstringstream cls{};
      cls << md5.digestFile(d.path().wstring().c_str());
      return cls.str();
    }

    void UpdateKey(FMap& fmap, const FileMap::FileKey& fk)
    {
      auto nh = fmap.extract(fk);                                            // extract the (one and only) already existing file with that size
      nh.key() = { nh.mapped().d.file_size(),GetMD5Hash(nh.mapped().d) };    // update the key with the MD5hash
      fmap.insert(move(nh));                                                 // and put it back in
    }
  }

  FMap::const_iterator Insert(FMap& fmap, const std::filesystem::directory_entry& d)
  {
    
    if (d.file_size() < 3LL) return fmap.cend(); // simply ignore all empty and tiny files
    std::error_code ec{};

    // try with FileKey containing only the size - only same-size files need to compare their MD5 Hashes
    const FileMap::FileKey fk1{ d.file_size(), L"" };  
    const size_t rank = fmap.count(fk1) + 1;
    FileMap::FileData fd{ rank, d };
    switch (rank)  // check how many files with this size are already there
    {
      case 1: // new entry, firstfile found with this size
        return fmap.insert({ fk1,fd });

      case 2: // duplicate entry; need to go back and calculate MD5 hash for BOTH old and new file
        UpdateKey(fmap, fk1);   // update the key with the MD5hash for the (one and only!) already existing file with that size
        [[fallthrough]];

      default: // second, third or more - calculate and insert the new file's MD5 hash
        FileMap::FileKey fk2{ d.file_size(), GetMD5Hash(d) };
        fd.rank = fmap.count(fk2) + 1;    // recalc the rank, now with the MD5 Hash
        return fmap.insert({ fk2,fd });   // add the new file (which is a potential duplicate)
    }
  }

}


#include "pch.h"

namespace FileMap
{

  namespace
  {
    MD5Hash GetMD5Hash(const std::filesystem::directory_entry& d)
    {
      MD5::MD5 md5;
      std::wstringstream cls{};
      cls << md5.digestFile(d.path().string().c_str());
      return cls.str();
    }

    void UpdateKey(FMap& fmap, FileMap::FileKey& fk)
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
    FileMap::FileKey fk0{ d.file_size(), L"" };  
    FileMap::FileData fd{ d };
    switch (fmap.count(fk0))  // check how many files with this size are already there
    {
      case 0: // new entry, firstfile found with this size
        return fmap.insert({ fk0,fd });

      case 1: // duplicate entry; need to go back and calculate MD5 hash for BOTH old and new file
        UpdateKey(fmap, fk0);   // update the key with the MD5hash for the (one and only) already existing file with that size
        [[fallthrough]];

      default: // calculate and insert the new file's MD5 hash
        return fmap.insert({ {d.file_size(),GetMD5Hash(d)},fd });   // add the new file (which is a potential duplicate)
    }
  }

}


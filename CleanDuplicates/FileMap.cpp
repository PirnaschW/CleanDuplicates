#include "pch.h"

namespace FileMap
{

  namespace
  {
    constexpr size_t lenHash{ 32*2 };
    std::filesystem::path GetHashFile(std::filesystem::path p)
    {
      return p += HashExtension;
    }
    bool ReadMD5File(std::filesystem::path p, MD5Hash& hash)
    {
      p += HashExtension;
      if (std::filesystem::exists(p))
      {
        std::wstring buffer;
        buffer.resize(lenHash);
        FILE* file = nullptr;
        if ((file = _wfopen(p.wstring().c_str(), L"rb")) == NULL)
        {
          printf("%Ls can't be opened\n", p.wstring().c_str());
        }
        else
        {
          size_t found = fread(buffer.data(), 1, lenHash, file);
          fclose(file);
          if (found != lenHash)
          {
            printf("%Ls has incomplete MD5 hash\n", p.wstring().c_str());
          }
          else
          {
            hash = buffer;
            return true;
          }
        }
      }
      return false;
    }
    void WriteMD5File(std::filesystem::path p, const MD5Hash& hash)
    {
      p += HashExtension;
      FILE* file = nullptr;
      if ((file = _wfopen(p.wstring().c_str(), L"wb")) == NULL)
        printf("%Ls can't be written\n", p.wstring().c_str());
      else
      {
        fwrite(hash.data(), lenHash, 1, file);
        fclose(file);
      }
    }

    MD5Hash GetMD5Hash(const std::filesystem::directory_entry& d, bool saveHash)
    {
      MD5Hash hash{};
      if (!saveHash || !ReadMD5File(d.path(), hash))
      {
        MD5::MD5 md5;
        std::wstringstream cls{};
        cls << md5.digestFile(d.path().wstring().c_str());
        hash = cls.str();

        if (saveHash)
        {
          WriteMD5File(d.path(), hash);
        }
      }
      return hash;
    }

    void UpdateKey(FMap& fmap, const FileMap::FileKey& fk, bool saveHash)
    {
      auto nh = fmap.extract(fk);                                            // extract the (one and only) already existing file with that size
      nh.key() = { nh.mapped().d.file_size(),GetMD5Hash(nh.mapped().d,saveHash) };    // update the key with the MD5hash
      fmap.insert(move(nh));                                                 // and put it back in
    }
  }

  FMap::const_iterator Insert(FMap& fmap, const std::filesystem::directory_entry& d, bool saveHash)
  {
    
    if (d.file_size() < 3LL) return fmap.cend(); // simply ignore all empty and tiny files
    std::error_code ec{};

    // try with FileKey containing only the size - only same-size files need to compare their MD5 Hashes
    const FileMap::FileKey fk1{ d.file_size(), L"" };  
    const size_t rank = fmap.count(fk1) + 1;
    FileMap::FileData fd{ rank, d };
    switch (rank)  // check how many files with this size are already there
    {
      case 1: // new entry, first file found with this size
        return fmap.insert({ fk1,fd });

      case 2: // duplicate entry; need to go back and calculate MD5 hash for BOTH old and new file
        UpdateKey(fmap, fk1, saveHash);   // update the key with the MD5hash for the (one and only!) already existing file with that size
        [[fallthrough]];

      default: // second, third or more - calculate and insert the new file's MD5 hash
        FileMap::FileKey fk2{ d.file_size(), GetMD5Hash(d, saveHash) };
        fd.rank = fmap.count(fk2) + 1;    // recalc the rank, now with the MD5 Hash
        return fmap.insert({ fk2,fd });   // add the new file (which is a potential duplicate)
    }
  }

}


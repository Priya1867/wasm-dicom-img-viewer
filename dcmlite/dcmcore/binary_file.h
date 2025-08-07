#pragma once

#include <fstream>
#include <stdint.h>
#include <string>
#include <string_view>

namespace dcmcore {

class BinaryFile {
  public:
  enum Mode {
    READ,
    WRITE,
  };

  public:
  BinaryFile(const BinaryFile& rhs) = delete;
  BinaryFile& operator=(const BinaryFile& rhs) = delete;

  BinaryFile();

  ~BinaryFile()
  {
    Close();
  }

  bool Open(std::string_view filename, Mode mode);
  bool OpenFromStream(std::istream& stream);  // ✅ NEW


  void Close();

  inline bool IsOk() const
  {
     return from_external_stream_ ? active_stream_ && active_stream_->good() : m_file.good();

  }

  inline Mode mode() const
  {
    return m_mode;
  }

  inline bool Seek(long offset, std::ios_base::seekdir origin = std::ios::beg)
  {
	  
	if (from_external_stream_) {
		if (active_stream_ && active_stream_->good()) {
      active_stream_->seekg(offset, origin);
      return active_stream_->good();
    }
    return false;
  } else {
    m_file.seekg(offset, origin);
    return IsOk();
  }

  }

  std::size_t ReadBytes(void* bytes, std::size_t count)
  {
	std::istream& in = from_external_stream_ ? *active_stream_ : m_file;

    in.read(reinterpret_cast<char*>(bytes), count);
    return in.gcount();
  }

  bool UndoRead(std::size_t byte_count)
  {
    return Seek(-(long)byte_count, std::ios::cur);
  }

  bool ReadUint8(std::uint8_t& value)
  {
    return ReadBytes(&value, 1) == 1;
  }

  // NOTE: Byte order is not considered.
  bool ReadUint16(std::uint16_t& value)
  {
    return ReadBytes(&value, 2) == 2;
  }

  // NOTE: Byte order is not considered.
  bool ReadUint32(std::uint32_t& value)
  {
    return ReadBytes(&value, 4) == 4;
  }

  bool ReadString(std::string& value, std::size_t count)
  {
    value.resize(count);
    // NOTE: ReadBytes(value, count) doesn't work!
    return ReadBytes(value.data(), count) == count;
  }

  inline void WriteBytes(const void* bytes, std::size_t count)
  {
    m_file.write(reinterpret_cast<const char*>(bytes), count);
  }

  inline bool WriteUint8(const std::uint8_t value)
  {
    WriteBytes(&value, 1);
    return IsOk();
  }

  // NOTE: Byte order is not considered.
  inline bool WriteUint16(const std::uint16_t value)
  {
    WriteBytes(&value, 2);
    return IsOk();
  }

  // NOTE: Byte order is not considered.
  inline bool WriteUint32(const std::uint32_t value)
  {
    WriteBytes(&value, 4);
    return IsOk();
  }

  inline bool WriteString(std::string_view value)
  {
    WriteBytes(value.data(), value.size());
    return IsOk();
  }

  private:
  std::fstream m_file;
  std::istream* active_stream_ = nullptr;  // ✅ NEW
  Mode m_mode;
  bool from_external_stream_ = false;      // ✅ NEW
};

} // namespace dcmcore
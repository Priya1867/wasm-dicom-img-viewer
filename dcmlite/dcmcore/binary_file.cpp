#include "dcmcore/binary_file.h"

namespace dcmcore {

BinaryFile::BinaryFile()
    : m_mode(Mode::READ)
{
}

bool BinaryFile::Open(std::string_view filename, Mode mode)
{
  Close();

  m_mode = mode;

  if (mode == Mode::READ) {
    m_file.open(filename.data(), std::ios::binary | std::ios::in);
  } else {
    m_file.open(filename.data(), std::ios::binary | std::ios::out);
  }

  return IsOk();
}

bool BinaryFile::OpenFromStream(std::istream& stream) {
    Close(); // reset

    active_stream_ = &stream;
    from_external_stream_ = true;
    m_mode = Mode::READ;

    return true;
}

void BinaryFile::Close()
{
  m_file.close();
  
  active_stream_ = nullptr;
  from_external_stream_ = false;
}

} // namespace dcmcore

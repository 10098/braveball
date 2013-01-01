/// @file util/resource_file.cpp
/// @author grigoryj

#include "util/resource_file.h"
#include <boost/asio.hpp>

namespace util
{
   ResourceFile::ResourceFile(const std::string& name)
   {
      // Try to open the given file. Throw exception if it fails.
      m_mappedFile.open(name);
      if(!m_mappedFile.is_open())
      {
         throw ResourceFileNotOpenException(name);
      }

      // The resource file consists of head and body sections.
      // The head section has the following format:
      //  resource_1_name <NUL> resource_1_offset resource_1_size
      //  resource_2_name <NUL> resource_2_offset resource_2_size
      //  ...
      //  resource_n_name <NUL> resource_n_offset resource_n_size
      //  <ETX>
      // Resource names are ASCII strings and resource offsets and
      // sizes are 32-bit integers using network byte order.
      // The body section is just concatenated resource data.

      const char* file_start = m_mappedFile.data();
      size_t resource_name_offset = 0;
      while(resource_name_offset < m_mappedFile.size() && // prevent getting out of bounds
            file_start[resource_name_offset] != 0x03) // 0x03 is the ASCII code for ETX
      {
         const char* resource_name = &file_start[resource_name_offset];
         ResourceData data;
         data.name = resource_name;
         int name_length = data.name.length();
         data.offset = boost::asio::detail::socket_ops::network_to_host_long(*(reinterpret_cast<const int*>(resource_name + name_length + 1)));
         data.size = boost::asio::detail::socket_ops::network_to_host_long(*(reinterpret_cast<const int*>(resource_name + name_length + 1 + sizeof(data.offset))));
         data.data = file_start + data.offset;
         m_map.insert(std::make_pair(data.name, data));
         resource_name_offset += name_length + 1 + 2 * 4;
      }
   }

   ResourceFile::~ResourceFile()
   {
      m_mappedFile.close();
   }

   ResourceFile::iterator ResourceFile::begin()
   {
      return m_map.begin();
   }

   ResourceFile::iterator ResourceFile::end()
   {
      return m_map.end();
   }

   ResourceFile::const_iterator ResourceFile::begin() const
   {
      return m_map.begin();
   }

   ResourceFile::const_iterator ResourceFile::end() const
   {
      return m_map.end();
   }

   const ResourceData& ResourceFile::getResource(const std::string& name) const
   {
      ResourceFileMap::const_iterator i = m_map.find(name);
      if(i != m_map.end())
      {
         return i -> second;
      }
      else
      {
         throw ResourceNotFoundException(name);
      }
   }
}
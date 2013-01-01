/// @file util/resource_file.h
/// @author grigoryj

#ifndef RESOURCE_FILE_H
#define RESOURCE_FILE_H

#include <unordered_map>
#include <string>
#include <boost/iostreams/device/mapped_file.hpp>

namespace util
{
   /// Data associated with a resource
   struct ResourceData
   {
      std::string name; /// Resource name
      uint32_t size;    /// Size of the data
      uint32_t offset;  /// Offset of the data from the start of the resource file
      const void* data; /// Pointer to the actual data
   };

   /// A resource file. Individual resources can be addressed by their names. 
   /// Supports iterating over all resources contained in the file.
   class ResourceFile
   {
   public:
      typedef std::unordered_map<std::string, ResourceData> ResourceFileMap;

      /// A wrapper over the hashtable's iterator.
      /// We could just use ResourceFileMap::iterator directly, 
      /// but it would be pretty inconvenient for the client code.
      struct iterator : public ResourceFileMap::iterator
      {
         iterator(const ResourceFileMap::iterator& i)
         { static_cast<ResourceFileMap::iterator&>(*this) = i; }

         ResourceData& operator*()
         { return static_cast<ResourceFileMap::iterator&>(*this) -> second; } // need the ugly cast because of the overloaded ->

         ResourceData* operator->()
         { return &(**this); }
      };

      /// Const version of previous iterator
      struct const_iterator : public ResourceFileMap::const_iterator
      {
         const_iterator(const ResourceFileMap::const_iterator& i)
         { static_cast<ResourceFileMap::const_iterator&>(*this) = i; }

         const ResourceData& operator*()
         { return static_cast<ResourceFileMap::const_iterator&>(*this) -> second; }

         const ResourceData* operator->()
         { return &(**this); }
      };

      /// Loads the resource file from the specified location on
      /// the file system.
      /// @throw runtime_error if the file cannot be opened.
      explicit ResourceFile(const std::string& file_name);

      /// Closes the underlying memory-mapped file
      ~ResourceFile();


      /// Iterator pointing to the first resource in the file.
      iterator begin();

      /// Iterator pointing to the end of the resources list.
      iterator end();

      /// Same as begin(), but returns a const-iterator.
      const_iterator begin() const;

      /// Same as end(), but returns a const-iterator.
      const_iterator end() const;

      /// Access a resource by name
      /// @param name The name of the resource to be accessed
      /// @throw ResourceNotFoundException if a resource with the given name does not exist
      const ResourceData& getResource(const std::string& name) const;

   private:
      boost::iostreams::mapped_file_source m_mappedFile; /// Memory-mapped file
      ResourceFileMap m_map; /// Maps resource names to corresponding ResourceData structures
   };

   struct ResourceFileException : public std::runtime_error
   {
      explicit ResourceFileException(const std::string& msg) : std::runtime_error(msg){}
   };

   struct ResourceFileNotOpenException : public ResourceFileException
   {
      explicit ResourceFileNotOpenException(const std::string& file_name) : ResourceFileException("Resource file [" + file_name + "] not found"){}
   };

   struct ResourceNotFoundException : public ResourceFileException
   {
      explicit ResourceNotFoundException(const std::string& resource_name) : ResourceFileException("Resource [" + resource_name + "] not found"){}
   };

   struct ResourceInvalidException : public ResourceFileException
   {
      ResourceInvalidException(const std::string& resource_name, const std::string& reason) : 
         ResourceFileException("Resource [" + resource_name + "] is invalid. Reason: [" + reason + "]"){}
   };

}

#endif // RESOURCE_FILE_H
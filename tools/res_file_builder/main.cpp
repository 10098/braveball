#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <string>
#include <numeric>
#include <queue>
#include <sstream>

int main(int argc, char* argv[])
{
   std::string root_dir_path;
   std::string out_file_path;

   boost::program_options::options_description cmdline_opt_desc;
   cmdline_opt_desc.add_options()
     ("help", "Output help message.")
     ("root,R", boost::program_options::value<std::string>(&root_dir_path), "Root of the directory tree")
     ("ouput,O", boost::program_options::value<std::string>(&out_file_path) -> default_value("out.res"), "Output resource file");

   boost::program_options::variables_map opt_vm;
   boost::program_options::store(boost::program_options::parse_command_line(argc, argv, cmdline_opt_desc), opt_vm);
   boost::program_options::notify(opt_vm);

   if(opt_vm.count("help"))
   {
      std::cout << cmdline_opt_desc << std::endl;
   }
   else
   {
      if(opt_vm.count("root") == 0)
      {
         std::cout << "Need to specify root folder" << std::endl;
      }
      else
      {
         try
         {
            std::vector<char> body;
            std::vector<std::pair<std::string, int> > map;
            std::queue<std::string> directories;
            if(root_dir_path.back() == '\\' || root_dir_path.back() == '/')
               root_dir_path.pop_back();
            directories.push(root_dir_path);
            boost::filesystem::path root_path = boost::filesystem::absolute(root_dir_path);
            while(!directories.empty())
            {
               std::string dir_name = directories.front();
               directories.pop();
               for(boost::filesystem::directory_iterator di(dir_name); di != boost::filesystem::directory_iterator(); ++di)
               {
                  if(boost::filesystem::is_directory(di -> status()))
                  {
                     directories.push(di -> path().string());
                  }
                  else if(boost::filesystem::is_regular_file(di -> status()))
                  {
                     std::ifstream in(di -> path().string(), std::ios::binary);
                     char c;
                     int resource_size = 0;
                     while(in.get(c))
                     {
                        body.push_back(c);
                        ++resource_size;
                     }
                     std::string res_name = boost::filesystem::absolute(di -> path()).string().substr(root_path.string().length() + 1);
                     boost::replace_all(res_name, "\\", "/");
                     map.push_back(std::make_pair(res_name, resource_size));
                  }
               }
            }
         
            unsigned header_size = std::accumulate(map.begin(), map.end(), 0,  [](unsigned i, const std::pair<std::string, int>& a){ return i + a.first.length() + 1 + 2*4; }) + 1;
            
            std::ofstream output(out_file_path, std::ios::binary);
         
            if(!output.is_open())
            {
               std::cerr << "Failed to open output file " << out_file_path << std::endl;
               return 1;
            }

            int pos = 0;
            for(auto i = map.begin(); i != map.end(); ++i)
            {
               output << i -> first;
               output << '\0';
               uint32_t offset = pos + header_size;
               uint32_t size = i -> second;
               pos += size;
               std::cout << "Adding file " << i -> first << " offset " << offset << " size " << size << std::endl;
               offset = boost::asio::detail::socket_ops::host_to_network_long(offset);
               size = boost::asio::detail::socket_ops::host_to_network_long(size);
               output.write(reinterpret_cast<const char*>(&offset), sizeof(uint32_t));
               output.write(reinterpret_cast<const char*>(&size), sizeof(uint32_t));
            }
            output << static_cast<char>(0x03);
            output.write(&body[0], body.size());
         }
         catch(const boost::filesystem::filesystem_error& e)
         {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
         }
      }
   }
   return 0;
}

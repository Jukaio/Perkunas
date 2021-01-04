// FileReader.hpp

#ifndef FILEREADER_H_INCLUDED
#define FILEREADER_H_INCLUDED

#include <string>
#include <map>
#include <optional>
#include <exception>
#include <filesystem>
#include <type_traits>
#include <concepts>






namespace ini 
{





    struct OutOfRange : std::out_of_range
    {
        OutOfRange(std::string message)
            : std::out_of_range(message) {};
    };
    struct OpenFile : std::runtime_error
    {
        OpenFile(std::string message)
            : std::runtime_error(message) {};
    };



    template<typename Type>
    concept Arithmethic = std::is_arithmetic<Type>::value;

    template <class T,class... Args>
    concept constructible_from =
            std::is_destructible<T>::value && std::is_constructible_v<T, Args...>;
    template <class T>
    concept string_constructible = 
            constructible_from<T, std::string>;


    typedef std::runtime_error Exception;

   struct FileReaderException final : Exception // This exception can be in the FileReader class scope
   {
      FileReaderException(std::string message) : Exception(message){};
      ~FileReaderException(){};
      // You can override what to do some special kind of stuff if you want to
   };

   struct FileReader
   {
      FileReader(std::string_view filePath); // std::path or something like that C++20 
      ~FileReader() noexcept; // default

      typedef std::filesystem::path Path;
      struct Pair // Ini not required, it is in the ini namespace
      {
          typedef std::string Name;
          typedef std::string Value;

          Name m_name;
          Value m_value;
      };






      FileReader() noexcept; 
      void load_data(Path target);


      






      // std::string_view is getting used 4 times so far
      // Think about a typedef to be more prone to future changes
      // i.e. variable_name_t, name_t, name_string, etc
      typedef std::string_view variable_name_t;
      using variable_name = std::string_view;
      // I am somewhat edgy who thinks c-style is cool, so i prefer the typedef


      // In the case of not existing a throw seems radical 
      // Make them generic through cocnept
      // cocnept IniTypes = ... add your type_traits here
      // If you need a specific kind of conversion, declare it explicitly 
      int GetInt(std::string_view variableName);
      float GetFloat(std::string_view variableName);
      std::string GetString(std::string_view variableName);
      bool GetBool(std::string_view variableName);


      template<Arithmethic Type>
      Type get(Pair::Name name)
      {
          float to_return = std::stof(m_map[name]);
          return static_cast<Type>(to_return);
      }

      template<>
      float get(Pair::Name name)
      {
          float to_return = std::stof(m_map[name]);
          return to_return;
      }


   private:
      // Deleted default constructors etc
      // Should we delete them or should we just copy the map?
      // The aquired resource in this case is the file handle and opening it
      // We wanna avoid opening the same file multiple times
      // Instead of deleting, we should just copy/move that.variableMap_ to this.
      // FileHandle is not a FileReader
      FileReader(const FileReader &fileHandle) = delete;
      FileReader(FileReader &&fileHandle) = delete;
      FileReader &operator=(const FileReader &fileHandle) = delete;
      FileReader &operator=(FileReader &&fileHandle) = delete;

      // It is very unspecific what is the index and what is stored
      // Typedefs/using like above
      // i.e.





      typedef std::map<Pair::Name, Pair::Value> Map;
      Map m_map; // We know it is a Map in the ini::FileReader






      std::map<std::string, std::string> variableMap_;
   };
}// !ini

#endif // !FILEREADER_H_INCLUDED

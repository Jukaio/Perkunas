// FileReader.cc
#include "FileReader.hpp"

#include <fstream>

// Helper functions
namespace ini
{
   std::string RemoveWhitespace(std::string_view string) noexcept
   {
      std::string returnString{string};
      returnString.erase(std::remove_if(returnString.begin(), returnString.end(), iswspace), returnString.end());
      
      return returnString;
   }
   

   std::map<std::string, std::string> CreateMapFromStream(std::ifstream &stream){
      std::map<std::string, std::string> returnMap;
   
      while (stream){
         std::string variableName = "";
         std::string variableString = "";
      
         if (stream.peek() == '#'){
            std::getline(stream, variableName, '\n');// clears the commented line from the stream
            continue;
         }
         std::getline(stream, variableName, '=');
         variableName = RemoveWhitespace(variableName);
         std::getline(stream, variableString);
         variableString = RemoveWhitespace(variableString);
         
         if(!stream){
            break;
         }
         returnMap.emplace(variableName, variableString);
      }
      return returnMap;
   }
   
   // FileHandle functions
   FileReader::FileReader(std::string_view filePath)
   {
      std::string string{filePath};
      std::ifstream stream;
   
      stream.open(string);
      // You can make ifstream throwable through flags 
      if (!stream.is_open()){
         throw FileReaderException("File could not be opened");
      }
      //Populate map with arguments from stream
      variableMap_ = CreateMapFromStream(stream);
      
      stream.close();
   }
   
   FileReader::~FileReader() noexcept
   {
   }
   FileReader::FileReader() noexcept
   {
   }
   void FileReader::load_data(Path target)
   {
   }
   int FileReader::GetInt(std::string_view variableName)
   {
      std::string string = RemoveWhitespace(variableName);
      if (variableMap_.find(string) == variableMap_.end()){
         throw FileReaderException("Requested variable was not found!");
      }
      std::string variableString = variableMap_.find(string)->second;

      int returnValue = 0;
      try {
         returnValue = std::stoi(variableString);
      } catch (std::invalid_argument){
         throw FileReaderException("Requested variable could not be read as int!");
      } catch (std::out_of_range){
         throw FileReaderException("std::stoi() threw out of range exception");
      }
      return returnValue;
   }
   float FileReader::GetFloat(std::string_view variableName)
   {
      std::string string = RemoveWhitespace(variableName);
      if (variableMap_.find(string) == variableMap_.end()){
         throw FileReaderException("Requested variable was not found!");
      }
      std::string variableString = variableMap_.find(string)->second;

      float returnValue = .0f;
      try {
         returnValue = std::stof(variableString);
      } catch (std::invalid_argument){
         throw FileReaderException("Requested variable could not be read as float!");
      } catch (std::out_of_range){
         throw FileReaderException("std::stof() threw out of range exception");
      }
      return returnValue;
   }
   std::string FileReader::GetString(std::string_view variableName)
   {
      std::string string = RemoveWhitespace(variableName);
      if (variableMap_.find(string) == variableMap_.end()){
         throw FileReaderException("Requested variable was not found!");
      }
      std::string variableString = variableMap_.find(string)->second;

      return variableString;
   }
   bool FileReader::GetBool(std::string_view variableName)
   {
      std::string string = RemoveWhitespace(variableName);
      if (variableMap_.find(string) == variableMap_.end()){
         throw FileReaderException("Requested variable was not found!");
      }
      std::string_view variableString = variableMap_.find(string)->second;
      if (variableString == "true"){
         return true;
      }
      else if(variableString == "false"){
         return false;
      }
      else {
         throw FileReaderException("Requested variable could not be read as bool");
      }

   }
}//!ini
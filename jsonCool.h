#ifndef JSONCOOLTTTT
#define JSONCOOLTTTT

	#include <iostream>
// Uncomment the next line to force the usage of the C++98 standard
// #define FORCE_OLD_STANDARD

#if __cplusplus == 201103L
	#ifndef FORCE_OLD_STANDARD
		#define USE_CPP11
	#endif
#endif

#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#ifndef USE_CPP11
	#include <math.h>
#endif

namespace jsonCool{
	class jsonException{
		public:
			friend class jsonStream;
			friend class jsonData;
			friend class jsonGenericContainer;
			friend class jsonNumberContainer;
			friend class jsonStringContainer;
			friend class jsonBooleanContainer;
			friend class jsonArrayContainer;
			friend class jsonObjectContainer;
			const std::string& what() const;
			const std::string& source() const;
			
		protected:
			const std::string _source;
			const std::string _description;
			jsonException(const std::string &source, const std::string &description);
	};
	
	const std::string& jsonException::what() const{
		return _description;
	}
	
	const std::string& jsonException::source() const{
		return _source;
	}
	
	jsonException::jsonException(const std::string &source, const std::string &description):
		_source(source),
		_description(description)
	{}
	
	class jsonData{
		public:
			jsonData();
			jsonData(const std::string &json);
			jsonData(const jsonData &reference);
			~jsonData();
			jsonData& operator[](const std::string &parameter);
			const jsonData& operator[](const std::string &parameter) const;
			jsonData& operator[](const char* parameter);
			const jsonData& operator[](const char* parameter) const;
			jsonData& operator[](const std::vector<jsonData>::size_type index);
			const jsonData& operator[](const std::vector<jsonData>::size_type index) const;
			jsonData& operator()();
			bool isNull() const;
			bool isBoolean() const;
			bool isNumber() const;
			bool isArray() const;
			bool isObject() const;
			bool isEmpty() const;
			jsonData& operator= (const double value);
			jsonData& operator= (const int value);
			jsonData& operator= (const std::string &value);
			jsonData& operator= (const char* value);
			jsonData& operator= (const jsonData &value);
			jsonData& operator= (const bool value);
			operator std::string() const;
			bool getBooleanValue() const;
			double getNumericValue() const;
			std::vector<std::string> getMemberNames() const;
			std::vector<jsonData>::size_type size() const;
			friend std::ostream& operator<< (std::ostream &out, jsonData &data);
    		friend std::istream& operator>> (std::istream &in, jsonData &data);
			
		protected:
			struct jsonGenericContainer{virtual ~jsonGenericContainer(){};};
			
			struct jsonBooleanContainer:	public jsonGenericContainer{bool								_data;};
			struct jsonNumberContainer:		public jsonGenericContainer{long double							_data;};
			struct jsonStringContainer:		public jsonGenericContainer{std::string							_data;};
			struct jsonArrayContainer:		public jsonGenericContainer{std::vector<jsonData>				_data;};
			struct jsonObjectContainer:		public jsonGenericContainer{std::map<std::string, jsonData>		_data;};
			
			jsonGenericContainer*		_data;
			
			void parse(const std::string &json);
	};
	
	jsonData::jsonData(){
		_data=0;
	}
	
	jsonData::jsonData(const std::string &json){
		_data=0;
		parse(json);
	}
	
	jsonData::jsonData(const jsonData &reference){
		_data=0;
		*this = reference;
	}
	
	jsonData::~jsonData(){
		if(_data)
			delete _data;
	}
	
	jsonData& jsonData::operator[](const std::string &parameter){
		if(typeid(*_data)!=typeid(jsonObjectContainer))
			throw jsonException("jsonData::operator[](const std::string &parameter)","value is not of object type");
		jsonObjectContainer* data = reinterpret_cast<jsonObjectContainer*>(_data);
		if(data->_data.find(parameter)==data->_data.end())
			data->_data[parameter] = jsonData();
		return data->_data.at(parameter);
	}
	
	const jsonData& jsonData::operator[](const std::string &parameter) const{
		if(typeid(*_data)!=typeid(jsonObjectContainer))
			throw jsonException("jsonData::operator[](const std::string &parameter)","value is not of object type");
		jsonObjectContainer* data = reinterpret_cast<jsonObjectContainer*>(_data);
		if(data->_data.find(parameter)==data->_data.end())
			throw jsonException("jsonData::operator[](const std::string &parameter)","invalid parameter name");
		return data->_data.at(parameter);
	}
	
	jsonData& jsonData::operator[](const char* parameter){
		if(typeid(*_data)!=typeid(jsonObjectContainer))
			throw jsonException("jsonData::operator[](const std::string &parameter)","value is not of object type");
		jsonObjectContainer* data = reinterpret_cast<jsonObjectContainer*>(_data);
		if(data->_data.find(parameter)==data->_data.end())
			data->_data[parameter] = jsonData();
		return data->_data.at(parameter);
	}
	
	const jsonData& jsonData::operator[](const char* parameter) const{
		if(typeid(*_data)!=typeid(jsonObjectContainer))
			throw jsonException("jsonData::operator[](const std::string &parameter)","value is not of object type");
		jsonObjectContainer* data = reinterpret_cast<jsonObjectContainer*>(_data);
		if(data->_data.find(parameter)==data->_data.end())
			throw jsonException("jsonData::operator[](const std::string &parameter)","invalid parameter name");
		return data->_data.at(parameter);
	}
	
	jsonData& jsonData::operator[](const std::vector<jsonData>::size_type index){
		if(typeid(*_data)!=typeid(jsonArrayContainer))
			throw jsonException("jsonData::operator[](const std::vector<jsonData>::size_type index)","value is not of array type");
		jsonArrayContainer* data = reinterpret_cast<jsonArrayContainer*>(_data);
		if(index>=data->_data.size())
			throw jsonException("jsonData::operator[](const std::vector<jsonData>::size_type index)","array index out of bounds");
		return data->_data[index];
	}
	
	const jsonData& jsonData::operator[](const std::vector<jsonData>::size_type index) const{
		if(typeid(*_data)!=typeid(jsonArrayContainer))
			throw jsonException("jsonData::operator[](const std::vector<jsonData>::size_type index)","value is not of array type");
		jsonArrayContainer* data = reinterpret_cast<jsonArrayContainer*>(_data);
		if(index>=data->_data.size())
			throw jsonException("jsonData::operator[](const std::vector<jsonData>::size_type index)","array index out of bounds");
		return data->_data[index];
	}
	
	jsonData& jsonData::operator()(){
		if(typeid(*_data)!=typeid(jsonArrayContainer))
			throw jsonException("jsonData::operator[]()","can not add an entry to a non-array container");
		jsonArrayContainer* data = reinterpret_cast<jsonArrayContainer*>(_data);
		data->_data.push_back(jsonData());
		return data->_data[data->_data.size()-1];
	}
			
	bool jsonData::isNull() const{
		return _data==0;
	}
	
	bool jsonData::isBoolean() const{
		return (typeid(*_data)==typeid(jsonBooleanContainer));
	}
	
	bool jsonData::isNumber() const{
		return (typeid(*_data)==typeid(jsonNumberContainer));
	}
	
	bool jsonData::isArray() const{
		return (typeid(*_data)==typeid(jsonArrayContainer));
	}
	
	bool jsonData::isObject() const{
		return (typeid(*_data)==typeid(jsonObjectContainer));
	}
	
	bool jsonData::isEmpty() const{
		if(typeid(*_data)==typeid(jsonArrayContainer)){
			jsonArrayContainer* data = reinterpret_cast<jsonArrayContainer*>(_data);
			return data->_data.size()==0;
		}
		else if(typeid(*_data)==typeid(jsonObjectContainer)){
			jsonObjectContainer* data = reinterpret_cast<jsonObjectContainer*>(_data);
			return data->_data.size()==0;
		}
		else return _data==0;
	}
	
	jsonData& jsonData::operator= (const double value){
		if(_data)
			delete _data;
		_data = new jsonNumberContainer;
		reinterpret_cast<jsonNumberContainer*>(_data)->_data = value;
		return *this;
	}
	
	jsonData& jsonData::operator= (const int value){
		if(_data)
			delete _data;
		_data = new jsonNumberContainer;
		reinterpret_cast<jsonNumberContainer*>(_data)->_data = value;
		return *this;
	}
			
	jsonData& jsonData::operator= (const bool value){
		if(_data)
			delete _data;
		_data = new jsonBooleanContainer;
		reinterpret_cast<jsonBooleanContainer*>(_data)->_data = value;
		return *this;
	}
	
	jsonData& jsonData::operator= (const std::string &value){
		if(_data){
			delete _data;
			_data = 0;
		}
		try{
			parse(value);
		}
		catch(jsonException &e){
			_data = new jsonStringContainer;
			reinterpret_cast<jsonStringContainer*>(_data)->_data = value;
		}
		return *this;	
	}
	
	jsonData& jsonData::operator= (const char* value){
		return operator=(std::string(value));
	}
	
	jsonData& jsonData::operator= (const jsonData &value){
		if(this!=&value){
			if(_data){
				delete _data;
				_data=0;
			}
			if(value._data == 0)
				return *this;
			else if(typeid(*value._data)==typeid(jsonBooleanContainer)){
				_data = new jsonBooleanContainer();
				reinterpret_cast<jsonBooleanContainer*>(_data)->_data = reinterpret_cast<jsonBooleanContainer*>(value._data)->_data;
			}
			else if(typeid(*value._data)==typeid(jsonNumberContainer)){
				_data = new jsonNumberContainer();
				reinterpret_cast<jsonNumberContainer*>(_data)->_data = reinterpret_cast<jsonNumberContainer*>(value._data)->_data;
			}
			else if(typeid(*value._data)==typeid(jsonArrayContainer)){
				_data = new jsonArrayContainer();
				reinterpret_cast<jsonArrayContainer*>(_data)->_data = reinterpret_cast<jsonArrayContainer*>(value._data)->_data;
			}
			else if(typeid(*value._data)==typeid(jsonObjectContainer)){
				_data = new jsonObjectContainer();
				reinterpret_cast<jsonObjectContainer*>(_data)->_data = reinterpret_cast<jsonObjectContainer*>(value._data)->_data;
			}
			else if(typeid(*value._data)==typeid(jsonStringContainer)){
				_data = new jsonStringContainer();
				reinterpret_cast<jsonStringContainer*>(_data)->_data = reinterpret_cast<jsonStringContainer*>(value._data)->_data;
			}
		}
		return *this;
	}
	
	jsonData::operator std::string() const{
		if(_data == 0)
			return "null";
		else if(typeid(*_data)==typeid(jsonBooleanContainer)){
			if(reinterpret_cast<jsonBooleanContainer*>(_data)->_data == false)
				return "false";
			else return "true";
		}
		else if(typeid(*_data)==typeid(jsonNumberContainer)){
		#ifdef USE_CPP11
			return std::to_string(reinterpret_cast<jsonNumberContainer*>(_data)->_data);
		#else
			std::stringstream parser;
			parser << reinterpret_cast<jsonNumberContainer*>(_data)->_data;
			return parser.str();
		#endif
		}
		else if(typeid(*_data)==typeid(jsonStringContainer)){
			return std::string("\""+reinterpret_cast<jsonStringContainer*>(_data)->_data+"\"");
		}
		else if(typeid(*_data)==typeid(jsonArrayContainer)){
			std::string output = "[";
		#ifdef USE_CPP11
			for(jsonData arrayEntry: reinterpret_cast<jsonArrayContainer*>(_data)->_data){
		#else
			for(unsigned int i=0;i<size();i++){
				jsonData arrayEntry = reinterpret_cast<jsonArrayContainer*>(_data)->_data[i];
		#endif
				output += (std::string)arrayEntry + ",";
			}
			if(output!="[") output = output.substr(0,output.size()-1);
			output += "]";
			return output;
		}
		else if(typeid(*_data)==typeid(jsonObjectContainer)){
			std::string output = "{";
		#ifdef USE_CPP11
			for(auto parameter: reinterpret_cast<jsonObjectContainer*>(_data)->_data){
				output += "\"" + parameter.first + "\":";
				output += (std::string)parameter.second + ",";
		#else
			std::map<std::string, jsonData>::iterator parameter = reinterpret_cast<jsonObjectContainer*>(_data)->_data.begin();
			for(;parameter!=reinterpret_cast<jsonObjectContainer*>(_data)->_data.end();parameter++){
				output += "\"" + parameter->first + "\":";
				output += (std::string)parameter->second + ",";
		#endif
			}
			if(output!="{") output = output.substr(0,output.size()-1);
			output += "}";
			return output;
		}
		else return "";
	}
	
	bool jsonData::getBooleanValue() const{
		if(_data==0)
			return false;
		if(isBoolean())
			return reinterpret_cast<jsonBooleanContainer*>(_data)->_data;
		if(isNumber())
			return reinterpret_cast<jsonNumberContainer*>(_data)->_data != 0.0;
		if(isArray() || isObject())
			return true;
		return reinterpret_cast<jsonStringContainer*>(_data)->_data != "";
	}
	
	double jsonData::getNumericValue() const{
		if(_data==0)
			return 0.0;
		if(isBoolean()){
			if(reinterpret_cast<jsonBooleanContainer*>(_data)->_data) return 1.0;
			else return 0.0;
		}
		if(isNumber())
			return reinterpret_cast<jsonNumberContainer*>(_data)->_data;
		if(isArray()){
			if(size()==0)
				return 0.0;
			if(size()==1)
				return reinterpret_cast<jsonArrayContainer*>(_data)->_data[0].getNumericValue();
		#ifdef USE_CPP11
			return std::nan("");
		#else
			return nan("");
		#endif
		}
		if(isObject())
		#ifdef USE_CPP11
			return std::nan("");
		#else
			return nan("");
		#endif
		std::stringstream parser(reinterpret_cast<jsonStringContainer*>(_data)->_data);
		if(parser.str()=="") return 0.0;
		double value;
		if(parser >> value)
			return value;
		#ifdef USE_CPP11
			return std::nan("");
		#else
			return nan("");
		#endif
	}
	
	
	#ifndef USE_CPP11
	std::string extractName(const std::map<std::string,jsonData>::value_type &pair){
		return pair.first;
	}
	#endif
	
	std::vector<std::string> jsonData::getMemberNames() const{
		if(!isObject()) throw jsonException("jsonData::getMemberNames()","data is not of object type");
		std::vector<std::string> names;
		std::transform(
	    	reinterpret_cast<jsonObjectContainer*>(_data)->_data.begin(),
	    	reinterpret_cast<jsonObjectContainer*>(_data)->_data.end(),
	    	std::back_inserter(names),
	    #ifdef USE_CPP11
	    	[](const std::map<std::string,jsonData>::value_type &pair){return pair.first;});
	    #else
	    	extractName);
	    #endif
	    return names;
	}
	
	std::vector<jsonData>::size_type jsonData::size() const{
		if(isArray())
			return reinterpret_cast<jsonArrayContainer*>(_data)->_data.size();
		else if(isObject())
			return reinterpret_cast<jsonObjectContainer*>(_data)->_data.size();
		else throw jsonException("jsonData::size()","data is not of array/object type");
	}
	
	std::ostream& operator<< (std::ostream &out, jsonData &data){
		out << (std::string)data;
		return out;
	}

	std::istream& operator>> (std::istream &in, jsonData &data){
		std::string stringData;
		std::getline(in,stringData);
		data = stringData;
		return in;
	}
	void jsonData::parse(const std::string &json){
		if(_data){
			delete _data;
			_data=0;
		}
		if(json=="") return;
		std::size_t position=json.find_first_of("{[\"");
		if(position!=std::string::npos){
			if(json[position]=='{'){
				this->_data = new jsonObjectContainer;
				jsonObjectContainer &data = *reinterpret_cast<jsonObjectContainer*>(_data);
				std::size_t closingBrace = json.find_last_of("}");
				if(closingBrace == std::string::npos)
					throw jsonException("jsonData::parse(const std::string &json)","data is ill-formed, could not find enclosing '}'");
				if(closingBrace == json.find_first_not_of(" \t\f\v\n\r",position+1))
					return;
				bool isLastParameter = false;
				while(position!=std::string::npos && position<closingBrace){
					std::string parameterValue="";
					bool wholeParameter = false;
					if(isLastParameter) return;
					unsigned long arrayLevels = 0, objectLevels = 0;
					std::size_t parameterNameStart = json.find('\"',position+1);
					std::size_t parameterNameEnd = json.find('\"',parameterNameStart+1);
					while(json[parameterNameEnd-1]=='\\' && parameterNameEnd!=std::string::npos)
						parameterNameEnd = json.find("\"",parameterNameEnd+1);
					if(parameterNameStart==std::string::npos || parameterNameEnd==std::string::npos)
						throw jsonException("jsonData::parse(const std::string &json)","data is ill-formed, object has to be a set of name/value pairs");
					std::string parameterName = json.substr(parameterNameStart+1,parameterNameEnd-parameterNameStart-1);
					std::size_t separatorPosition = json.find(":",parameterNameEnd+1);
					if(separatorPosition==std::string::npos)
						throw jsonException("jsonData::parse(const std::string &json)","data is ill-formed, no separator found after an object parameter identifier");
					std::size_t currentPosition = separatorPosition;
					std::size_t lastFound = currentPosition;
					while(!wholeParameter){
						isLastParameter = (json.find(',',currentPosition)==std::string::npos);
						lastFound = json.find_first_of("{[\"]},",currentPosition+1);
						if(json[lastFound]==',' && arrayLevels==0 && objectLevels==0){
							wholeParameter = true;
							parameterValue = json.substr(separatorPosition+1, lastFound-separatorPosition-1);
							break;
						}
						else if(isLastParameter && arrayLevels==0 && objectLevels==0){
							wholeParameter = true;
							parameterValue = json.substr(separatorPosition+1, json.find_last_of("{[\"]}")-separatorPosition-1);
							break;
						}
						else if(lastFound == std::string::npos)						
							throw jsonException("jsonData::parse(const std::string &json)","data is ill-formed, parameter value missing");
						else if(json[lastFound]=='{')
							objectLevels++;
						else if(json[lastFound]=='}')
							objectLevels--;
						else if(json[lastFound]=='[')
							arrayLevels++;
						else if(json[lastFound]==']')
							arrayLevels--;
						else if(json[lastFound]=='\"'){
							std::size_t closingQuotation = json.find('\"',lastFound+1);
							while(closingQuotation!=std::string::npos && json[closingQuotation-1]=='\\')
								closingQuotation = json.find('\"',closingQuotation+1);
							if(closingQuotation == std::string::npos)
								throw jsonException("jsonData::parse(const std::string &json)","data is ill-formed, could not find terminating quotation mark of an object parameter value");
							currentPosition = lastFound = closingQuotation;
							continue;
						}
						currentPosition = lastFound;
					}
					jsonData value(parameterValue);
					data._data[parameterName] = value;
					position = currentPosition+1;
				}
			}
			else if(json[position]=='['){
				this->_data = new jsonArrayContainer;
				jsonArrayContainer &data = *reinterpret_cast<jsonArrayContainer*>(_data);
				std::size_t closingBrace = json.find_last_of("]");
				if(closingBrace == std::string::npos)
					throw jsonException("jsonData::parse(const std::string &json)","data is ill-formed, could not find enclosing ']'");
				if(closingBrace == json.find_first_not_of(" \t\f\v\n\r",position+1))
					return;
				bool isLastParameter = false;
				std::size_t separatorPosition = json.find("[");
				while(position!=std::string::npos && position<closingBrace){
					std::string parameterValue="";
					bool wholeParameter = false;
					if(isLastParameter) return;
					unsigned long arrayLevels = 0, objectLevels = 0;
					std::size_t currentPosition = separatorPosition;
					std::size_t lastFound = currentPosition;
					while(!wholeParameter){
						isLastParameter = (json.find(',',currentPosition)==std::string::npos);
						lastFound = json.find_first_of("{[\"]},",currentPosition+1);
						if(json[lastFound]==',' && arrayLevels==0 && objectLevels==0){
							wholeParameter = true;
							parameterValue = json.substr(separatorPosition+1, lastFound-separatorPosition-1);
							break;
						}
						else if(isLastParameter && arrayLevels==0 && objectLevels==0){
							wholeParameter = true;
							parameterValue = json.substr(separatorPosition+1, json.find_last_of("{[\"]}")-separatorPosition-1);
							break;
						}
						else if(lastFound == std::string::npos)						
							throw jsonException("jsonData::parse(const std::string &json)","data is ill-formed, parameter value missing");
						else if(json[lastFound]=='{')
							objectLevels++;
						else if(json[lastFound]=='}')
							objectLevels--;
						else if(json[lastFound]=='[')
							arrayLevels++;
						else if(json[lastFound]==']')
							arrayLevels--;
						else if(json[lastFound]=='\"'){
							std::size_t closingQuotation = json.find('\"',lastFound+1);
							while(closingQuotation!=std::string::npos && json[closingQuotation-1]=='\\')
								closingQuotation = json.find('\"',closingQuotation+1);
							if(closingQuotation == std::string::npos)
								throw jsonException("jsonData::parse(const std::string &json)","data is ill-formed, could not find terminating quotation mark of an object parameter value");
							currentPosition = lastFound = closingQuotation;
							continue;
						}
						currentPosition = lastFound;
					}
					if(!isLastParameter) separatorPosition = json.find(",",currentPosition+1);
					jsonData value(parameterValue);
					data._data.push_back(value);
					position = currentPosition+1;
				}
			}
			else{
				std::size_t closingQuotation = json.find("\"",position+1);
				while(json[closingQuotation-1]=='\\' && closingQuotation!=std::string::npos)
					closingQuotation = json.find("\"",closingQuotation+1);
				if(closingQuotation == std::string::npos)
					throw jsonException("jsonData::parse(const std::string &json)","data is ill-formed, could not find terminating quotation mark");
				this->_data = new jsonStringContainer;
				jsonStringContainer &data = *(reinterpret_cast<jsonStringContainer*>(_data));
				data._data = json.substr(position+1, closingQuotation-position-1);
			}
		}
		else
		{
			std::stringstream parser(json);
			if(json.substr(json.find_first_not_of(" \t\f\v\n\r"))=="true"){
				this->_data = new jsonBooleanContainer;
				jsonBooleanContainer &data = *reinterpret_cast<jsonBooleanContainer*>(_data);
				data._data = true;
				return;
			}
			else if(json.substr(json.find_first_not_of(" \t\f\v\n\r"))=="false"){
				this->_data = new jsonBooleanContainer;
				jsonBooleanContainer &data = *reinterpret_cast<jsonBooleanContainer*>(_data);
				data._data = false;
				return;
			}
			else if(json.substr(json.find_first_not_of(" \t\f\v\n\r"))=="null"){
				_data = 0;
				return;
			}
			this->_data = new jsonNumberContainer;
			jsonNumberContainer &data = *reinterpret_cast<jsonNumberContainer*>(_data);
			if(parser >> data._data){
				return;
			}
			else{
				delete _data;
				_data = 0;
				throw jsonException("jsonData::parse(const std::string &json)","data is ill-formed, unknown value format");
			}		
		}
	}
	
	class jsonStream{
		public:
			jsonStream(const std::string &fileName);
			jsonStream& open(const std::string &fileName);
			jsonStream& save();
			jsonStream& saveAs(const std::string &fileName);
			bool isOpen() const;
			jsonStream& operator>>(std::string &target);
			jsonStream& operator>>(jsonData &target);
			jsonData& data();
			
		private:
			std::string			_fileName;
			jsonData			_root;
	};
	
	jsonStream::jsonStream(const std::string &fileName){
		open(fileName);
	}
	
	jsonStream& jsonStream::open(const std::string &fileName){
		_fileName = "";
		 
		std::ifstream input(fileName.c_str());
		if(input.is_open()){
			_fileName = fileName;
			
			std::string contents;
		    input.seekg(0, std::ios::end);
		    contents.resize(input.tellg());
		    input.seekg(0, std::ios::beg);
		    input.read(&contents[0], contents.size()-1);
		    
		    if(input.fail())
		    	throw jsonException("jsonStream::open(const std::string &fileName)","error reading file");
		    	
		    _root = contents;
		    
		    if(!_root.isArray() && !_root.isObject())
		    	throw jsonException("jsonStream::open(const std::string &fileName)","the root of a JSON file needs to be either an object or an array");
			
			input.close();
		}
		else
			throw jsonException("jsonStream::open(const std::string &fileName)","could not open file");
		return *this;
	}
	
	jsonStream& jsonStream::save(){
		if(_fileName=="")
			throw jsonException("jsonStream::save()","could not save a file, since no file is opened. Use 'open' or 'saveAs' first");
		std::ofstream output(_fileName.c_str());
		if(!output.is_open())
			throw jsonException("jsonStream::save()","could not open file");
		if(output << _root){
			output.close();
			return *this;
		}
		throw jsonException("jsonStream::save()","error writing to file");
	}
	
	jsonStream& jsonStream::saveAs(const std::string &fileName){
		std::ofstream output(fileName.c_str(),std::ios::out);
		if(!output.is_open())
			throw jsonException("jsonStream::saveAs()","could not open file");
		_fileName = fileName;
		output.close();
		return save();
	}
	
	bool jsonStream::isOpen() const{
		return _fileName!="";
	}
	
	jsonStream& jsonStream::operator>>(std::string &target){
		target = (std::string)_root;
		return *this;
	}
	
	jsonStream& jsonStream::operator>>(jsonData &target){
		target = _root;
		return *this;
	}
	
	jsonData& jsonStream::data(){
		return _root;
	}
	
	namespace presets{
		const jsonData null;
		const jsonData array("[]");
		const jsonData object("{}");
	}
}
#endif

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

/**
 * @mainpage About jsonCool
 * @section reasoning					What is jsonCool?
 *
 * The idea behind creating the jsonCool library, was to provide a portable, lightweight 
 * and object oriented C++ JSON API. In fact, the suffix "cool" means "compact, object
 * oriented library". JsonCool consists of exactly one header file, which can be easily
 * included into any project, in any environment, and is designed in a way, to make the
 * code similar to the code you have been used to, while working with the standard library.
 *
 * JsonCool is able to parse both JSON files and strings containing JSON code. The same applies
 * to creating JSON from your data.
 *
 * Next page: [Working with jsonCool \>\>](@ref examples)
 */
 
/**
 * @page examples						Working with jsonCool
 * 
 * Previous page: [\<\< About jsonCool](@ref index)
 * Next page: [Legal information \>\>](@ref legal)
 *
 * @section examples_main			 	How to install jsonCool?
 *
 * For starters - include the file containing the library. This step is as simple as putting
 * the file into your project directory and typing:
 * 
 *     #include "jsonStream.h"
 *
 * ...in any of the files in your project, that will use the library classes and functions.
 *
 * NOTE: some IDEs (Intergrated Development Environments) require the user to include the
 * file in the project. For an explanation on how to do that, consult your IDEs manual.
 *
 * @subsection examples_creating		Creating JSON data from scratch
 *
 * If you have some data - a class, an array, anything else - that you want to convert into
 * JSON data, you should use the jsonCool::jsonData class. For example, if you have a structure
 * that is defined as follows:
 *
 *     struct book{
 *     	bool available;
 *     	int pageNumber;
 *     	std::string title;
 *     	std::vector<std::string> chapterNames;
 *     };
 *
 * JsoonCool allows you to convert into Json by doing:
 *
 *     try{
 *     	extern book exampleBook; // defined somewhere else
 *     	jsonCool::jsonData bookInJSON = jsonCool::presets::object;
 *     	bookInJSON["available"] = exampleBook.available;
 *     	bookInJSON["pageNumber"] = exampleBook.pageNumber;
 *     	bookInJSON["title"] = exampleBook.title;
 *     	bookInJSON["chapterNames"] = jsonCool::presets::array;
 *     	for(unsigned int i=0; i<exampleBook.chapterNames.size(); i++)
 *     		bookInJSON["chapterNames"]() = exampleBook.chapterNames[i];
 *     }
 *     catch(const jsonCool::jsonException &e){
 *     	std::cout << e.source() << ": " << e.what() << std::endl;
 *     }
 *
 * Obviously: you are free to put the code in a member function of the \c book class, if that
 * is what you wish. The same rules apply to any other \c class or \c structure . After converting
 * the data into a jsonCool::jsonData class, serializing it is as simple as writing:
 *
 *     std::string serializedBook = bookInJSON;
 *
 * Or, straight using the overloaded stream output operator:
 *
 *     std::cout << bookInJSON << std::endl;
 *
 * @subsection examples_working			Working with existing JSON data
 *
 * While the previous examples describe the process of *serializing* the data to the JSON format, one
 * might also need to *deserialize* JSON data. JsonCool also offers a simple and natural way to do that.
 * 
 * Let us assume that we added a \c book to our library, and serialized it using the [example above](@ref examples_creating).
 * We have ended up with a JSON string which looks something like this:
 *
 *     std::string serialized = "{\"available\": true, \"chapterNames\": [\"Genesis\", \"The little glass slipper\"], \"pageNumber\": 12.000000, \"title\": \"Cinderella\"}";
 *
 * Deserializing it to a jsonCool::jsonData object can be done in just one line of code:
 *
 *     jsonCool::jsonData bookInJSON(serialized);
 *
 * Then, converting it into a \c book is done by doing the opposite of what we did while serializing:
 *
 *     try{
 *      book cindarella;
 *     	cindarella.available = bookInJSON["available"].getBooleanValue();
 *     	cindarella.pageNumber = bookInJSON["pageNumber"].getNumericValue();
 *     	cindarella.title = bookInJSON["title"];
 *     	for(unsigned int i=0; i<bookInJSON["chapterNames"].size(); i++)
 *     		cindarella.chapterNames.push_back(bookInJSON["chapterNames"][i]);
 *     }
 *     catch(const jsonCool::jsonException &e){
 *     	std::cout << e.source() << ": " << e.what() << std::endl;
 *     }
 *
 * @subsection examples_presets			Preset values - array, object, null
 *
 * The library defines three presets, that you might have seen in the previous example. All of them
 * are defined in the jsonCool::presets namespace:
 * - jsonCool::presets::array
 *   Defines an empty array.
 * - jsonCool::presets::object
 *   Defines an object without any properties
 * - json::presets::null
 *   Defines a \c null value.
 *
 * @subsection examples_filesin			Reading files
 *
 * While serializing and deserializing JSON data is undoubtedly useful, jsonCool also works with while
 * JSON files. The class designated to be used in such cases is jsonCool::jsonStream . First let us assume
 * that we wrote our JSON book to a file called 'library.json', by using the standard library file stream:
 *
 *     std::string serialized = "{\"available\": true, \"chapterNames\": [\"Genesis\", \"The little glass slipper\"], \"pageNumber\": 12.000000, \"title\": \"Cinderella\"}";
 *     std::ofstream output("library.json");
 *     output << serialized << std::endl;
 *     output.close();
 *
 * We can then open the file and interpret it with just one line:
 *
 *     try{
 *     	jsonCool::jsonStream jsonParser("library.json");
 *     }
 *     catch(const jsonCool::jsonException &e){
 *     	std::cout << e.source() << ": " << e.what() << std::endl;
 *     }
 *
 * After that, retrieving a jsonCool::jsonData object is straightforward:
 * 
 *     try{
 *     	jsonCool::jsonStream jsonParser("library.json");
 *     	jsonCool::jsonData bookInJSON;
 *     	jsonParser >> bookInJSON;
 *     }
 *     catch(const jsonCool::jsonException &e){
 *     	std::cout << e.source() << ": " << e.what() << std::endl;
 *     }
 *
 * @subsection examples_filesout		Writing files
 *
 * When we know how to [create JSON data](@ref examples_creating), [interpret it](@ref examples_working), and [read JSON files](@ref examples_filesin), all we need to learn is
 * how to create new JSON files. While the example in the last chapter showed how to do that using
 * the standard library classes, jsonCool provides a simpler way.
 *
 * First, let us take our book and turn it into a whole book library (so the name of the file actually
 * has some sense):
 *
 *     try{
 *     	jsonCool::jsonStream jsonParser("library.json");
 *     	jsonCool::jsonData bookInJSON;
 *     	jsonParser >> bookInJSON;
 *     	jsonCool::jsonData jsonLibrary = jsonCool::presets::array;
 *     	jsonCool::jsonData bookInJSON2 = bookInJSON;
 *     	
 *     	bookInJSON2["title"] = "Cinderella - ilustrated edition";
 *     	bookInJSON2["pageNumber"] = 18;
 *     	
 *     	jsonLibrary() = bookInJSON;
 *     	jsonLibrary() = bookInJSON2;
 *     }
 *     catch(const jsonCool::jsonException &e){
 *     	std::cout << e.source() << ": " << e.what() << std::endl;
 *     }
 *
 * Now we can save the file:
 *
 *     try{
 *     	jsonCool::jsonStream jsonParser("library.json");
 *     	jsonCool::jsonData bookInJSON;
 *     	jsonParser >> bookInJSON;
 *     	jsonCool::jsonData jsonLibrary = jsonCool::presets::array;
 *     	jsonCool::jsonData bookInJSON2 = bookInJSON;
 *     	
 *     	bookInJSON2["title"] = "Cinderella - ilustrated edition";
 *     	bookInJSON2["pageNumber"] = 18;
 *     	
 *     	jsonLibrary() = bookInJSON;
 *     	jsonLibrary() = bookInJSON2;
 *
 *     	jsonParser.data() = jsonLibrary;
 *     	jsonParser.save();
 *     }
 *     catch(const jsonCool::jsonException &e){
 *     	std::cout << e.source() << ": " << e.what() << std::endl;
 *     }
 *
 * Previous page: [\<\< About jsonCool](@ref index)
 * Next page: [Legal information \>\>](@ref legal)
 */
 
 /**
 * @page legal						Legal information
 * 
 * Previous page: [\<\< Working with jsonCool](@ref examples)
 * Next page: [FAQ \>\>](@ref faq)
 *
 * @section legal_main			 	Software License
 *
 * This software is licensed under the [MIT license](https://opensource.org/licenses/MIT).
 *
 * Copyright (c) 2015 Pawel Stawarz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section legal_links				Useful links
 *
 * - [jsoncool GIT repository](https://github.com/PawelStawarz/jsoncool),
 * - [jsoncool online documentation](http://jsoncool.stawarz.me),
 * - [JSON standard  - RFC 7159](http://www.rfc-editor.org/rfc/rfc7159.txt),
 * - [JSON description](http://www.rfc-editor.org/rfc/rfc7159.txt),
 * - [doxygen main page](http://www.stack.nl/~dimitri/doxygen/)
 *
 * Previous page: [\<\< About jsonCool](@ref index)
 * Next page: [FAQ \>\>](@ref faq)
 */
 
 /**
 * @page faq						Frequently asked questions
 * 
 * Previous page: [\<\< Legal information](@ref legal)
 *
 * @section faq_legal			 	Legal problems
 *
 * 1. **Question**: Can I use \c jsonCool in my commercial product?
 *
 *  **Answer**: Yes, you can use jsonCool in both commercial and free products, provided you
 * include the [license](@ref legal_main) with your finished product.
 *
 * 2. **Question**: Do I have to pay any fee to use this library commercially?
 *
 *  **Answer**: No. You can use it for free. That being said - it would be really nice to
 * hear from you, if you did. Be sure to contact me, if you have some free time after
 * finishing your application. I'll gladly check it out, whenever I can.
 *
 * 3. **Question**: I want to distribute my application in one file. Can I _not_ include the license?
 *
 *  **Answer**: You're still required to include the permission and copyright notice. However - you don't
 * have to include it in a separate file. You may add it on some sort of 'credits' screen, if that fits
 * your particular needs.
 *
 * 4. **Question**: I'm building my own API/library nad I wish to distribute is as a single C++ header file.
 *    Is it sufficient if I include the notice in the file itself?
 *
 *  **Answer**: Yes, as long as you include the both, full notices.
 *
 * 5. **Question**: I extended the \c jsonCool code. Can I distribute is as my own library?
 *
 *  **Answer**: Yes, bu you are still required to add both notices with it.
 *
 * @section faq_tech				Technical questions
 *
 * 1. **Question**: I found and fixed a few bugs. If my changes are accepted and merged
 * into the GIT repository, will I be noted as a creator?
 *
 *  **Answer**: Every large enough contribution aimed towards fixing the library, will
 * be noted.
 *
 * 2. **Question**: The library doesn't work.
 *
 *  **Answer**: You have to be a _lot_ more specific. If you're sure the bug is inside the
 * library (for example - it only occurs on a particular IDE), then suggest a change via the
 * GIT service. If you're not sure, provide a Short, Self Contained, Correct Example and try
 * searching for help on [Stack Overflow](http://stackoverflow.com).
 *
 * 3. **Question**: In my opinion, this library is bad. "X" is faster/better/harder/stronger.
 *
 *  **Answer**: \c jsonCool is only an alternative. If you find out that any other JSON library
 * suits your needs better, just use it. The point is to make _your_ life easier.
 */

 /**
  * @namespace	jsonCool
  * @author		mgr inz. Pawel Stawarz
  * @brief		Namespace containing all of the libraries classes, constants and other members
  * 
  * @date		04 november 2015
  *
  * The \c jsonCool namespace contains all classes, constants and functions defined by the library.
  * If you do not wish to use the `jsonCool\::` prefix before the library functions and classes,
  * be sure to call `using namespace jsonCool` in your source files.
  *
  * Please note that it is considered bad practice to call the `using` directive in header files.
  * If you're working in a group - consult your project manager before doing os.
  */
namespace jsonCool{
	/* =======================================================
	 * jsonException
	 * =======================================================
	 */
	 /**
	  * @class		jsonException
	  * @author		mgr inz. Pawel Stawarz
	  * @brief		Exception class
	  * 
	  * @date		04 november 2015
	  *
	  * This class is thrown by all other classes of this library, when
	  * an exception occurs. The class constructor is protected, which assures
	  * that all instances of this class are created by the library itself.
	  */
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
			
			/**
			 * @brief		Provides a brief description of the exception
			 *
			 * @return		a constant reference to a \c std\::string containing a brief description of the exception
			 *
			 * The function returns a brief description of the exception. For an example on how
			 * to handle \c jsonCool library exceptions, check jsonException::source(). 
			 */
			const std::string& what() const;
			
			/**
			 * @brief		Provides a source of the exception
			 *
			 * @return		a constant reference to a \c std\::string containing the source of the exception
			 *
			 * The function returns a string containing the name of the function, which threw the exception.
			 * When using the library in your project, it is safer to do everything in a try-catch block. The
			 * following code is a good example on how to utilize the jsonCool::jsonException member functions:
			 *
			 *     try{
			 *     	std::string invalidJson = "{\"parameter\"=unquotedStringValue}";
			 *     	jsonCool::jsonData parser(invalidJson);
			 *     }
			 *     catch(jsonCool::jsonException &e){
			 *     	std::cout << "Exception thrown at \'" << e.source() << "': " << e.what() << endl;
			 *     } 
			 */
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
	
	/* =======================================================
	 * jsonData
	 * =======================================================
	 */
	 /**
	  * @class		jsonData
	  * @author		mgr inz. Pawel Stawarz
	  * @brief		JSON data container
	  * 
	  * @date		05 november 2015
	  *
	  * This class stores JSON structures of all possible types, supported by the JSON
	  * format. The class is used both for literal values (\c true, \c false and \c null ), strings,
	  * numbers, arrays and objects.
	  *
	  * The value \c null is recognized as an object without any internal value. All other
	  * types of data have their own, appriopriate containers. One can retrieve the information
	  * about the data type by using the 'is*' function family.
	  *
	  * @section jsonDatas01 null
	  *
	  * To check whether the data is \c null , use the jsonData::isNull() function. When calling the
	  * jsonData::getNumericValue() function, \c null will be converted into '0'. When casting the object
	  * to a std\::string, the result will be the string '"null"'. Trying to convert \c null into an array
	  * or object, will result in an exception being thrown. For exception handling consult jsonCool::jsonException
	  *
	  * @section jsonDatas02 boolean values
	  *
	  * To check whether the data is a boolean value, use the jsonData::isBoolean method. When calling the
	  * jsonData::getBooleanValue() function, standard JavaScript conversion rules apply:
	  * + \c null will be converted into \c false ,
	  * + \c true and \c false will have their appriopriate values,
	  * + A numeric value of \c 0 will yield \c false , and all other numeric values will be recognized as \c true ,
	  * + An empty string will yield \c false , and all other strigs will be recognized as \c true ,
	  * + An array is converted to \c true , no matter if, and what its members are,
	  * + An object is converted to \c true , no matter whether it has any parameters.
	  *
	  * @section jsonDatas03 numeric values
	  *
	  * To check whether the data is a numeric value, use the jsonData::isNumber method. When calling the
	  * jsonData::getNumericValue() function, standard JavaScript conversion rules apply:
	  * + \c null will be converted into \c 0 ,
	  * + \c false will be converted into \c 0 and \c true into \c 1 ,
	  * + A string containing only a number (for example '"20"'), will be converted into the appriopriate numeric value.
	  *   An empty string will be converted into \c 0 and any other string will yield \c NaN ,
	  * + An empty array is converted into \c 0 . An array containing exactly one element is converted into the numeric
	  *   value of that element. An array with more elements than one, yields \c NaN ,
	  * + An object is converted into \c NaN .
	  *
	  * The NaN value is generated using the standard library nan() function, located within \<cmath\>. To test
	  * whether a value is NaN, use the isnan() function, from the standard C++ library.
	  *
	  * @section jsonDatas04 strings
	  *
	  * Since the whole idea behind JSON is for it, to be serializable, all values can be converted to a string.
	  * In order to convert data into a std\::string, just use the overloaded conversion operator.
	  *
	  * @section jsonDatas05 arrays
	  *
	  * To check whether the data is an array, use the jsonData::isArray method. If the data is an array, jsonData::size()
	  * can be used to check its size. Iterating an array can be done by calling the overloaded jsonData::operator[](const std::vector<jsonData>::size_type index) .
	  * All array elements are of type jsonCool::jsonData , since JSON arrays are allowed to store values of mixed
	  * types. If you want to make a particular value into an empty array, use jsonCool::array (see the documentation of that
	  * object, for an example). You can add new elements into an existing array by calling the overloaded
	  * jsonData::operator()() .
	  *
	  * @section jsonDatas06 objects
	  *
	  * To check whether the data in an object, use the jsonData::isObject method. If the data is an object, jsonData::size()
	  * can be used to check its number of the object properties. In order to retrieve a list of names of the objects properties,
	  * call the jsonData::getMemberNames() function. The value of a particular property, can be retrieved by calling the overloaded
	  * jsonData::operator[](const std::string &parameter) or jsonData::operator[](const char* parameter) operators.
	  * You can add a new property by calling the above methods, and passing an unexisting property name as the function parameter.
	  */
	class jsonData{
		public:
			/**
			 * @brief		Default class constructor
			 *
			 * Creates a new instance of the class. The value of the object is set to \c null .
			 */
			jsonData();
			
			/**
			 * @brief		Deserializing constructor
			 *
			 * @param [in]	json	string containing data in the JSON format to be deserialized
			 *
			 * Creates a new instance of the class. The type and value of the data is described by the
			 * constructor parameter. If the passed data is ill-formed, an jsonCool::jsonException is
			 * thrown, which includes the description of the problem. Exceptions are thrown when:
			 * 
			 * 1. The data is an object (starts with the control character '{'), but the enclosing
			 *    '}' character is not found within the scope:
			 * 
			 *    - Source: "jsonData::parse(const std::string &json)",
			 *    - Description: "data is ill-formed, could not find enclosing '}'",
			 * 
			 * 2. The data is an object (starts with the control character '{'), but does not consist
			 *    of a list of named properties with their respective values:
			 * 
			 *    - Source: "jsonData::parse(const std::string &json)",
			 *    - Description: "data is ill-formed, object has to be a set of name/value pairs",
			 * 
			 * 3. The data is an object (starts with the control character '{'), but the property
			 *    name is not separated from its value by the ':' character:
			 * 
			 *    - Source: "jsonData::parse(const std::string &json)",
			 *    - Description: "data is ill-formed, no separator found after an object parameter identifier",
			 * 
			 * 4. The data is an object (starts with the control character '{'), but there is
			 *    no value present after a property name and the ':' character:
			 * 
			 *    - Source: "jsonData::parse(const std::string &json)",
			 *    - Description: "data is ill-formed, parameter value missing",
			 * 
			 * 5. The data is an object (starts with the control character '{'), and the property
			 *    is a string, but lacks an enclosing '"' character:
			 * 
			 *    - Source: "jsonData::parse(const std::string &json)",
			 *    - Description: "data is ill-formed, could not find terminating quotation mark of an object parameter value",
			 * 
			 * 6. The data is an array (starts with the control character '['), but the enclosing
			 *    ']' character is not found within the scope:
			 * 
			 *    - Source: "jsonData::parse(const std::string &json)",
			 *    - Description: "data is ill-formed, could not find enclosing ']'",
			 * 
			 * 7. The data is a string, but the enclosing '"' character could not be found
			 *    within the scope:
			 * 
			 *    - Source: "jsonData::parse(const std::string &json)",
			 *    - Description: "data is ill-formed, could not find terminating quotation mark",
			 * 
			 * 8. The data is of unknown format:
			 * 
			 *    - Source: "jsonData::parse(const std::string &json)",
			 *    - Description: "data is ill-formed, unknown value format",
			 */
			jsonData(const std::string &json);
			
			/**
			 * @brief		Copy constructor
			 *
			 * @param [in]	reference	A reference object, from which the data will be copied
			 *
			 * Creates a new instance of the class. The type and value of the data is the same, as
			 * the type and value of data, that are stored by the reference object.
			 */
			jsonData(const jsonData &reference);
			
			/**
			 * @brief		Default destructor
			 *
			 * Deallocates any previously allocated memory, and destroys the object.
			 */
			~jsonData();
			
			/**
			 * @brief		Property access operator
			 *
			 * @param [in]	parameter	Name of the property
			 * 
			 * @return		Value of the choosen property
			 *
			 * Returns the value of the given property. If the data is not an object, a jsonCool::jsonException
			 * is thrown, with the following parameters:
			 * 
			 * - Source: "jsonData::operator[](const std::string &parameter)",
			 * - Description: "value is not of object type",
			 *
			 * If a property with a given name, does not exist, a new property is created and added to the object.
			 * The value of the new property is \c null .
			 *
			 * Example usage:
			 *
			 *     std::string json = "{\"a\"=\"v1\", \"b\"=\"v2\"}";
			 *     jsonCool::jsonData exampleData(json);
			 *     if(exampleData.isObject()){
			 *     	exampleData["c"] = "v3";
			 *     	std::vector<std::string> names = exampleData.getMemberNames();
			 *     	for(unsigned int i=0; i<names.size(); i++)
			 *     		std::cout << exampleData[names[i]] << std::endl;
			 *     }
			 */
			jsonData& operator[](const std::string &parameter);
			
			/**
			 * @brief		Property access operator
			 *
			 * @param [in]	parameter	Name of the property
			 * 
			 * @return		Value of the choosen property
			 *
			 * Returns the value of the given property. If the data is not an object, a jsonCool::jsonException
			 * is thrown, with the following parameters:
			 * 
			 * - Source: "jsonData::operator[](const std::string &parameter)",
			 * - Description: "value is not of object type",
			 *
			 * If a property with a given name, does not exist, a jsonCool::jsonException is thrown, with the following parameters:
			 *
			 * - Source: "jsonData::operator[](const std::string &parameter)",
			 * - Description: "invalid parameter name",
			 *
			 * This version of the overloaded operator [] will be called only if the jsonCool::jsonDatan object is defined
			 * as \c const . Otherwise the [non-constant version](operator[](const std::string &parameter);) is called, which doesn't throw exceptions.
			 *
			 * For a code example, refer to [the first overload of this operator](operator[](const std::string &parameter)).
			 */
			const jsonData& operator[](const std::string &parameter) const;
			
			/**
			 * @brief		Property access operator
			 *
			 * @param [in]	parameter	Name of the property
			 * 
			 * @return		Value of the choosen property
			 *
			 * This function works exactly the same as [the string version](operator[](const std::string &parameter);). Please check
			 * the documentation of that method for details.
			 *
			 * For a code example, refer to [the first overload of this operator](operator[](const std::string &parameter)).
			 */
			jsonData& operator[](const char* parameter);
			
			/**
			 * @brief		Property access operator
			 *
			 * @param [in]	parameter	Name of the property
			 * 
			 * @return		Value of the choosen property
			 *
			 * This function works exactly the same as [the string version](operator[](const std::string &parameter) const). Please check
			 * the documentation of that method for details.
			 *
			 * For a code example, refer to [the first overload of this operator](operator[](const std::string &parameter)).
			 */
			const jsonData& operator[](const char* parameter) const;
			
			/**
			 * @brief		Array element access operator
			 *
			 * @param [in]	index	Index of the element
			 * 
			 * @return		Value of the array entry
			 *
			 * This function returns the value of a particular array entry. To check the array size, use
			 * jsonData::size() . To add a new array element, use jsonData::operator() . If the data is not
			 * an array, a jsonCool::jsonException object is thrown, with the following parameters:
			 * 
			 * - Source: "jsonData::operator[](const std::vector<jsonData>::size_type index)",
			 * - Description: "value is not of array type",
			 *
			 * If the index is invalid (i.e. greater than the array size), a jsonCool::jsonException object
			 * is thrown, with the following parameters:
			 *
			 * - Source: "jsonData::operator[](const std::vector<jsonData>::size_type index)",
			 * - Description: "array index out of bounds",
			 *
			 * Example usage:
			 *
			 *     std::string json = "[\"a\", \"b\", \"c\", \"d\", \"e\"]";
			 *     jsonCool::jsonData exampleData(json);
			 *     if(exampleData.isArray()){
			 *     	exampleData() = "f";
			 *     	for(unsigned int i=0; i<exampleData.size(); i++)
			 *     		std::cout << exampleData[i] << std::endl;
			 *     }
			 */
			jsonData& operator[](const std::vector<jsonData>::size_type index);
			
			/**
			 * @brief		Array element access operator
			 *
			 * @param [in]	index	Index of the element
			 * 
			 * @return		Value of the array entry
			 *
			 * This function returns the value of a particular array entry. To check the array size, use
			 * jsonData::size() . To add a new array element, use jsonData::operator() . If the data is not
			 * an array, a jsonCool::jsonException object is thrown, with the following parameters:
			 * 
			 * - Source: "jsonData::operator[](const std::vector<jsonData>::size_type index)",
			 * - Description: "value is not of array type",
			 *
			 * If the index is invalid (i.e. greater than the array size), a jsonCool::jsonException object
			 * is thrown, with the following parameters:
			 *
			 * - Source: "jsonData::operator[](const std::vector<jsonData>::size_type index)",
			 * - Description: "array index out of bounds",
			 *
			 * This version of the overloaded operator [] will be called only if the jsonCool::jsonDatan object is defined
			 * as \c const . Otherwise the [non-constant version](operator[](const std::vector<jsonData>::size_type index)) is called.
			 *
			 * For a code example, refer to [the first overload of this operator](operator[](const std::vector<jsonData>::size_type index)).
			 */
			const jsonData& operator[](const std::vector<jsonData>::size_type index) const;
			
			/**
			 * @brief		Adds a new element to an JSON array
			 * 
			 * @return		Reference to the newly added element
			 *
			 * If the data is an array, calling this function adds a new entry to it and returns the reference to the
			 * newly added element. If the stored data is not an array, a jsonCool::jsonException object is thrown,
			 * with the following parameters:
			 *
			 * - Source: "jsonData::operator[]()",
			 * - Description: "can not add an entry to a non-array container",
			 *
			 * The example documenting the usage of [operator[]](operator[](const std::vector<jsonData>::size_type index)) also presents the usage of this method.
			 */
			jsonData& operator()();
			
			/**
			 * @brief		Test whether the stored value is \c null
			 * 
			 * @return		\c true if value is \c null , \c false otherwise
			 *
			 * Returns \c true if, and only if, the stored value is equal to \c null . If the container holds _any_
			 * data, \c false will be returned.
			 */
			bool isNull() const;
			
			/**
			 * @brief		Test whether the stored value is boolean
			 * 
			 * @return		\c true if value is boolean, \c false otherwise
			 *
			 * Returns \c true if, and only if, the stored value is boolean. Note that this function returns \c false
			 * even, when the stored value can be _converted_ into a boolean value. In order to extract the value from
			 * the object, use jsonData::getBooleanValue() .
			 */
			bool isBoolean() const;
			
			/**
			 * @brief		Test whether the stored value is a number
			 * 
			 * @return		\c true if value is a number, \c false otherwise
			 *
			 * Returns \c true if, and only if, the stored value is a number. This includes both integer and floating
			 * point numbers (since both are stored within a doubl precision floating point variable in the library itself).
			 * Note that this function returns \c false even, when the stored value can be _converted_ into a numeric value.
			 * If you want to extract the exact value of the object, use jsonData::getNumericValue() .
			 */
			bool isNumber() const;
			
			/**
			 * @brief		Test whether the stored value is an array
			 * 
			 * @return		\c true if value is an array, \c false otherwise
			 *
			 * Returns \c true if, and only if, the stored value is an array. If the value is an array, its elements
			 * can be accessed with one of the overloaded jsonData::operator[] versions. New elements may be added
			 * using the overloaded jsonData::operator() . The current size of the array may be retrieved by calling
			 * jsonData::size() .
			 */
			bool isArray() const;
			
			/**
			 * @brief		Test whether the stored value is an object
			 * 
			 * @return		\c true if value is an object, \c false otherwise
			 *
			 * Returns \c true if, and only if, the stored value is an object. If the value is an object, the names of
			 * its properties may be retreated by calling jsonData::getMemberNames() . The number of properties can
			 * be retrieved by calling jsonData::size() . You can get the value of a property, or add a new one, using
			 * the [overloaded array operator](operator[](const std::string &parameter)).
			 */
			bool isObject() const;
			
			/**
			 * @brief		Test whether the stored data is either \c null or has no entries
			 * 
			 * @return		\c true if value is a \c null or has no entries, \c false otherwise
			 *
			 * Returns \c true if the container is _empty_. The container is considered _empty_ if:
			 *
			 * - it holds a \c null value, or...
			 * - it is an array with no entries, or...
			 * - it is an object with no properties
			 *
			 */
			bool isEmpty() const;
		
			/**
			 * @brief		Assign a new value to the object
			 *
			 * @param [in]	value	New value
			 * 
			 * @return		Reference to this object
			 *
			 * Assigns a new value to the object. The old value is lost in the process.
			 */
			jsonData& operator= (const double value);
		
			/**
			 * @brief		Assign a new value to the object
			 *
			 * @param [in]	value	New value
			 * 
			 * @return		Reference to this object
			 *
			 * Assigns a new value to the object. The old value is lost in the process.
			 */
			jsonData& operator= (const int value);
		
			/**
			 * @brief		Assign a new value to the object
			 *
			 * @param [in]	value	New value
			 * 
			 * @return		Reference to this object
			 *
			 * Assigns a new value to the object. The old value is lost in the process. Since a string can contain
			 * both literal data, and data to parse, the library first tries to process it, just like standard
			 * JSON data. If that fails - the data is parsed as a regular string.
			 *
			 * Example:
			 *
			 *    	std::string validJson = "{\"property\": \"value\"}";
			 *    	std::string invalidJson = "{property: value}";
			 *    	
			 *    	jsonCool::jsonData exampleData;
			 *    	exampleData = validJson;
			 *    	
			 *    	std::cout << exampleData.isObject() << std::endl;		// yields: 'true'
			 *    	
			 *    	exampleData = invalidJson;
			 *    	
			 *    	std::cout << exampleData.isObject() << std::endl;		// yields 'false'
			 *    	std::cout << exampleData << std::endl;					// outputs '"{property: value}"'
			 *    	
			 */
			jsonData& operator= (const std::string &value);
		
			/**
			 * @brief		Assign a new value to the object
			 *
			 * @param [in]	value	New value
			 * 
			 * @return		Reference to this object
			 *
			 * Assigns a new value to the object. The old value is lost in the process. Since a string can contain
			 * both literal data, and data to parse, the library first tries to process it, just like standard
			 * JSON data. If that fails - the data is parsed as a regular string.
			 *
			 * Example:
			 *
			 *    	std::string validJson = "{\"property\": \"value\"}";
			 *    	std::string invalidJson = "{property: value}";
			 *    	
			 *    	jsonCool::jsonData exampleData;
			 *    	exampleData = validJson;
			 *    	
			 *    	std::cout << exampleData.isObject() << std::endl;		// yields: 'true'
			 *    	
			 *    	exampleData = invalidJson;
			 *    	
			 *    	std::cout << exampleData.isObject() << std::endl;		// yields 'false'
			 *    	std::cout << exampleData << std::endl;					// outputs '"{property: value}"'
			 *    	
			 */
			jsonData& operator= (const char* value);
		
			/**
			 * @brief		Assign a new value to the object
			 *
			 * @param [in]	value	New value
			 * 
			 * @return		Reference to this object
			 *
			 * Assigns a new value to the object. The old value is lost in the process.
			 */
			jsonData& operator= (const jsonData &value);
		
			/**
			 * @brief		Assign a new value to the object
			 *
			 * @param [in]	value	New value
			 * 
			 * @return		Reference to this object
			 *
			 * Assigns a new value to the object. The old value is lost in the process.
			 */
			jsonData& operator= (const bool value);
		
			/**
			 * @brief		Serialize the data
			 * 
			 * @return		Data in JSON format
			 *
			 * Produces valid JSON from the stored data. 
			 */
			operator std::string() const;
			
			/**
			 * @brief		Get boolean value of data
			 * 
			 * @return		Boolean representation of the data
			 *
			 * Returns the boolean representation of the data. Standard JavaScript conversions
			 * apply:
			 * + \c null will be converted into \c false ,
			 * + \c true and \c false will have their appriopriate values,
			 * + A numeric value of \c 0 will yield \c false , and all other numeric values will be recognized as \c true ,
			 * + An empty string will yield \c false , and all other strigs will be recognized as \c true ,
			 * + An array is converted to \c true , no matter if, and what its members are,
			 * + An object is converted to \c true , no matter whether it has any parameters.
			 */	
			bool getBooleanValue() const;
			
			/**
			 * @brief		Get numeric value of data
			 * 
			 * @return		Numeric representation of the data
			 *
			 * Returns the numeric representation of the data. Standard JavaScript conversion
			 * rules apply:
			 * + \c null will be converted into \c 0 ,
			 * + \c false will be converted into \c 0 and \c true into \c 1 ,
			 * + A string containing only a number (for example '"20"'), will be converted into the appriopriate numeric value.
			 *   An empty string will be converted into \c 0 and any other string will yield \c NaN ,
			 * + An empty array is converted into \c 0 . An array containing exactly one element is converted into the numeric
			 *   value of that element. An array with more elements than one, yields \c NaN ,
			 * + An object is converted into \c NaN .
			 */	
			double getNumericValue() const;
		
			/**
			 * @brief		Get property names of an object
			 * 
			 * @return		Vector of strings containing property names
			 *
			 * If the data is an object, this function returns a vector of property names.
			 * In order to check whether the data is of object type, call jsonData::isObject() .
			 * In order to get the number of properties, call jsonData::size() .
			 *
			 * If the data is not an object, a jsonCool::jsonException is thrown with the following
			 * arguments:
			 *
			 * - Source: "jsonData::getMemberNames()",
			 * - Description: "data is not of object type",
			 *
			 */			
			std::vector<std::string> getMemberNames() const;
		
			/**
			 * @brief		Get number of entries in an array or properties of an object
			 * 
			 * @return		Number of entries in an array or properties of an object
			 *
			 * If the data is an object, this function returns the number of the entries. If the data
			 * is an object, the function returns the number of its properties. In every other case,
			 * a jsonCool::jsonException is thrown with the following arguments:
			 *
			 * - Source: "jsonData::size()",
			 * - Description: "data is not of array/object type",
			 *
			 */		
			std::vector<jsonData>::size_type size() const;
		
			/**
			 * @brief		Overloaded stream output operator
			 *
			 * @param [in]	out			Output stream reference
			 * @param [in]	data		Object containing JSON data
			 * 
			 * @return					Reference to the stream passed as the first argument
			 *
			 * Overloaded stream output operator. Calling this function is equal
			 * to doing:
			 *     out << (std::string)data;
			 */
			friend std::ostream& operator<< (std::ostream &out, jsonData &data);
		
			/**
			 * @brief		Overloaded stream input operator
			 *
			 * @param [in]	in			Input stream reference
			 * @param [in]	data		Object containing JSON data
			 * 
			 * @return					Reference to the stream passed as the first argument
			 *
			 * Overloaded stream input operator. Calling this function is equal
			 * to doing:
			 *     std::string stringData;
			 *     std::getline(in,stringData);
			 *     data = stringData;
			 */
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
	
	/* =======================================================
	 * jsonStream
	 * =======================================================
	 */
	 /**
	  * @class		jsonStream
	  * @author		mgr inz. Pawel Stawarz
	  * @brief		JSON input/output stream
	  * 
	  * @date		07 november 2015
	  *
	  * This class provides a simplified interface to interact with JSON files. A file can be opened by calling
	  * the jsonStream::open() function, or using the constructor. Data can then be accessed in various ways - either
	  * as a \c std\::string or as an instance of jsonCool::jsonData .
	  *
	  * Example:
	  *
	  *     try{
	  *     	jsonCool::jsonStream input("input.json");
	  *     	if(input.data().isObject())
	  *     		input.data()["someNewProperty"] = presets::null;
	  *     	else
	  *     		input.data()() = presets::null;
	  *     	input.saveAs("inputModified.json");
	  *     }
	  *     catch(jsonException &e){
	  *     	std::cout << "Exception thrown at \'" << e.source() << "': " << e.what() << std::endl;
	  *     }
	  *
	  */
	class jsonStream{
		public:
			/**
			 * @brief		Create a new JSON stream and open a file
			 *
			 * @param [in]	fileName	file to open
			 *
			 * This function creates a new object of the jsonStream class, opens a specified file (which
			 * _should_ contain data in the JSON format) and interprets it. Since reading a file may cause
			 * exceptions to be thrown, please see the documentation of jsonStream::open() and
			 * jsonData::jsonData(const std::string &json) .
			 *
			 * Also please note, that valid JSON files are required to store *only* arrays and objects.
			 * Any other data types should be members of the root. For example, a file which has the below
			 * contents:
			 *
			 * > [3.1415]
			 *
			 * Is a valid JSON file, but a file which has the below contents:
			 *
			 * > 3.1415
			 *
			 * is *not* a valid JSON file.
			 *
			 * For an example on how to use this method, see the documentation of jsonCool::jsonStream
			 */
			jsonStream(const std::string &fileName);
			
			/**
			 * @brief		Open a file and interpret its contents
			 *
			 * @param [in]	fileName	file to open
			 *
			 * @return		Reference to this object
			 *
			 * This function opens a specified file, and tries to parse its contents as JSON. During the
			 * process, a number of exceptions can be thrown:
			 * 
			 * 1. The file does not exist or is corrupt:
			 * 
			 *    - Source: "jsonStream::open(const std::string &fileName)",
			 *    - Description: "could not open file",
			 * 
			 * 2. An error occured while trying to read the file:
			 * 
			 *    - Source: "jsonStream::open(const std::string &fileName)",
			 *    - Description: "error reading file",
			 *
			 * 3. The root of the file is neither an object, nor an array:
			 * 
			 *    - Source: "jsonStream::open(const std::string &fileName)",
			 *    - Description: "the root of a JSON file needs to be either an object or an array",
			 *
			 * Also, a whole lot of exceptions can occur while parsing the data stored in the file.
			 * For a full list, see the documentation of jsonData::jsonData(const std::string &json) .
			 */
			jsonStream& open(const std::string &fileName);
			
			/**
			 * @brief		Save all modifications
			 *
			 * @return		Reference to this object
			 *
			 * This function saves all changes done to the previously opened file. If no file has
			 * been opened, use jsonStream::saveAs() instead. If no file has been opened, a jsonCool::jsonException
			 * is thrown with the following parameters:
			 * 
			 *    - Source: "jsonStream::save()",
			 *    - Description: "could not save a file, since no file is opened. Use 'open' or 'saveAs' first",
			 * 
			 * If a file was opened, but for some reason can not be accessed at the moment, a
			 * jsonCool::jsonException is thrown with the following parameters:
			 * 
			 *    - Source: "jsonStream::save()",
			 *    - Description: "could not open file",
			 *
			 * If there is an error during the writing of the new contents, a jsonCool::jsonException
			 * is thrown with the following parameters:
			 * 
			 *    - Source: "jsonStream::save()",
			 *    - Description: "error writing to file",
			 *
			 * For an example on how to use this method, see the documentation of jsonCool::jsonStream
			 */
			jsonStream& save();
			
			/**
			 * @brief		Save all contents to a new file
			 *
			 * @param [in]	fileName	file to save to
			 *
			 * @return		Reference to this object
			 *
			 * This function saves all contents to a new file and sets it as the current
			 * file.
			 * 
			 * For a list of possible exceptions, see jsonStream::save().
			 * 
			 * For an example on how to use this method, see the documentation of jsonCool::jsonStream
			 */
			jsonStream& saveAs(const std::string &fileName);
			
			/**
			 * @brief		Checks whether a file has been opened
			 *
			 * @return		\c true if a file was opened, \c false otherwise
			 *
			 * Tests whether any JSON file was opened. Since both the constructor
			 * and jsonStream::open() function throw exceptions whenever something
			 * goes wrong, this function will always return \c true , if one of the
			 * previously mentioned functions are called.
			 */
			bool isOpen() const;
			
			/**
			 * @brief		Puts the data in a \c std\::string object
			 *
			 * @param [in]	target	string to which the data will be saved
			 *
			 * @return		Reference to this object
			 *
			 * Outputs the current contents to a \c std\::string .
			 */
			jsonStream& operator>>(std::string &target);
			
			/**
			 * @brief		Puts the data in a jsonCool::jsonData object
			 *
			 * @param [in]	target	a jsonCool::jsonData object to which the data will be copied
			 *
			 * @return		Reference to this object
			 *
			 * Outputs the current contents to a jsonCool::jsonData . Works the same way
			 * as the jsonStream::data() function, but was provided to make the class act
			 * like standard C++ IO streams.
			 */
			jsonStream& operator>>(jsonData &target);
			
			/**
			 * @brief		Returns a refernce to the underlaying jsonCool::jsonData object
			 *
			 * @return		Reference to the underlaying jsonCool::jsonData object
			 *
			 * Returns a reference to the underlaying jsonCool::jsonData object. 
			 *
			 * For an example on how to use this method, see the documentation of jsonCool::jsonStream
			 */
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
	
	/**
	 * @namespace	jsonCool::presets
	 * @author		mgr inz. Pawel Stawarz
	 * @brief		Namespace containing presets of the jsonCool::jsonData class
	 * 
	 * @date		07 november 2015
	 *
	 * The \c presets namespace contains instances of the jsonCool::jsonData that
	 * may be used to make the code look more like the JavaScript code.
	 *
	 * Example:
	 * 
	 *     // Creates an array with 3 nulls:
	 *     jsonCool::jsonData someData;
	 *     someData = jsonCool::presets::array;
	 *     for(unsigned int i=0; i<3; i++)
	 *     	someData() = jsonCool::presets::null;
	 *     std::cout << someData << std::endl;
	 */
	namespace presets{
		const jsonData null;
		const jsonData array("[]");
		const jsonData object("{}");
	}
}
#endif

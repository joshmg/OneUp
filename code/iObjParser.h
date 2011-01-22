/*! @file iObjParser.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of the Object File Parser.
*/

#ifndef iOBJPARSER_H
#define iOBJPARSER_H

#include "ResultCodes.h"
#include <vector>
#include <string>

class Word;

/*! @brief A simple encoding of a "record".
    
    @par
    The format of this component is dependent upon
    the kind of record it is representing.
    @par
    @arg Header Record (type = 'H')
    <ul>
    <li>data.size() = 3</li>
    <ul><li> data[0] = [Segment Name]</li>
    <li> data[1] = [Initial Load Address (as a hex string)]</li>
    <li> data[2] = [Segment Length (as a hex string)]</li></ul>
    </ul>
    @arg Text Records (type = 'T')
    <ul>
    <li>data.size() = 2</li>
    <ul><li>data[0] = [Address of Data (as a hex string)]</li>
    <li>data[1] = [Data (as a hex string)]</li></ul>
    </ul>
    @arg End Records (type = 'E')
    <ul>
    <li>data.size() = 1</li>
    <ul><li>data[0] = [Initial PC Address (as a hex string)]</li></ul>
    </ul>
*/
struct ObjectData {
  //! The type of record: 'H', 'T', or 'E'
  char type;
  //! The segments of the record.
  std::vector<std::string> data;
};

/*! @brief Defines how object files are processed.
*/
class iObjParser {
  private:
  
  public:
    /*! @brief Attempts to open th object file.
        @param[in] filename The name of the object file to be opened.
        @return SUCCESS or, if something went wrong, an appropriate error code.
        
        If another file is open, closes that file first before
        attempting to open the new one.
    */
    virtual Codes::RESULT Initialize(const char* filename) = 0;


    /*! @brief Pre-processes the next line of the object file.
        @pre Initialize must have successfully opened a file.
        @return The encoding of the next instruction.

        If there is an error parsing the entry:
        @arg ObjectData.type = 0;
        @arg ObjectData.data = [the faulty encoding]
    */
    virtual ObjectData GetNext() = 0;
};

#endif


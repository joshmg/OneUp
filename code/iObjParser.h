<<<<<<< HEAD
// File: iObjParser.h
// Written by Joshua Green

#ifndef iOBJPARSER_H
#define iOBJPARSER_H

#include "ResultCodes.h"
#include <vector>
#include <string>

class Word;

struct ObjectData {
  /*  Purpose:
        The ObjectData data class serves the iLoader component to easily
        identify the intended Object file data entry--be it a header entry,
        a Text entry, or an End entry. The type of entry determines the
        size of ObjectData's data vector:
          Header Entries (type = 'H')
            -> data.size() = 3
              -> data[0] = [Segment Name]
              -> data[1] = [Initial Load Address (as a hex string)]
              -> data[2] = [Segment Length (as a hex string)]
          Text Entries (type = 'T')
            -> data.size() = 2
              -> data[0] = [Address of Data (as a hex string)]
              -> data[1] = [Data (as a hex string)]
          Text Entries (type = 'E')
            -> data.size() = 1
              -> data[0] = [Initial PC Address (as a hex string)]
  */

  char type;
  std::vector<std::string> data;
};

class iObjParser {
  private:
  
  public:
    virtual ~iObjParser() = 0;
    /*  If a file was opened successfully, it must be closed. This
        deconstructor ensures that happens.
    */

    virtual Codes::Result Initialize(const char*) = 0;
    /*  Attempts to open the input file. Returns the SUCCESS code if there
        was no problem opening the file. If a file was previously opened,
        the old file must be closed before the next one is opened.
    */

    virtual ObjectData GetNext() = 0;
    /*  Requires that Initialize() was invoked successfully. Returns the
        next entry within the *.obj file. See the purpose of ObjEntry
        to ensure the object is defined appropriately. If there is an error
        parsing the entry:
        ObjectData.type = 0
        ObjectData.data = [the text of the entry until whitespace
                            (if available)]
    */
};

#endif
=======
// File: iObjParser.h
// Written by Joshua Green

#ifndef iOBJPARSER_H
#define iOBJPARSER_H

#include "ResultCodes.h"
#include <vector>
#include <string>

class Word;

struct ObjectData {
  /*  Purpose:
        The ObjectData data class serves the iLoader component to easily
        identify the intended Object file data entry--be it a header entry,
        a Text entry, or an End entry. The type of entry determines the
        size of ObjectData's data vector:
          Header Entries (type = 'H')
            -> data.size() = 3
              -> data[0] = [Segment Name]
              -> data[1] = [Initial Load Address (as a hex string)]
              -> data[2] = [Segment Length (as a hex string)]
          Text Entries (type = 'T')
            -> data.size() = 2
              -> data[0] = [Address of Data (as a hex string)]
              -> data[1] = [Data (as a hex string)]
          Text Entries (type = 'E')
            -> data.size() = 1
              -> data[0] = [Initial PC Address (as a hex string)]
  */

  char type;
  std::vector<std::string> data;
};

class iObjParser {
  private:

  public:
    virtual ~iObjParser() = 0;
    /*  If a file was opened successfully, it must be closed. This
        deconstructor ensures that happens.
    */

    virtual Codes::Result Initialize(const char*) = 0;
    /*  Attempts to open the input file. Returns the SUCCESS code if there
        was no problem opening the file. If a file was previously opened,
        the old file must be closed before the next one is opened.
    */

    virtual ObjectData GetNext() = 0;
    /*  Requires that Initialize() was invoked successfully. Returns the
        next entry within the *.obj file. See the purpose of ObjEntry
        to ensure the object is defined appropriately. If there is an error
        parsing the entry:
        ObjectData.type = 0
        ObjectData.data = [the text of the entry until whitespace
                            (if available)]
    */
};

#endif
>>>>>>> 35e2f6496f63a032200aa3774f689bf71acdf682

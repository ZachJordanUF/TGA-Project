#include <vector>
#include <string>
#include <fstream>
using namespace std;

                                //Declare a Header struct
struct Header {
                                //Declare each Header element
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};

                            // Declare a Pixel struct
struct Pixel {
                            // Declare each Pixel element
    unsigned char blue;     // unsigned char is a 8bit integer 0-255
    unsigned char green;    // 0 (dark) - 255 (bright)
    unsigned char red;
};


class Image {                           // Declare private data member of the Image class
private:
    string name;
    ifstream stream;                    // Declare a stream (ifstream)
public:
    Header header;                      // Declare a header for the Image
    vector<Pixel> pixelVect;            // Declare a vector of pixels
    Image();                            // Declare an Image constructor
    void read(const string& inName);    // Declare a read function
    void write(const string& outName);  // Declare a write function
    void readPixel();
    void writePixel();

};







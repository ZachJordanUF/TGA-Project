#include "Image.h"
//#pragma once


Image::Image() {}

void Image::read(const string& inName) {
    //Read in the .tga file
    this->stream = ifstream(inName, ios_base::binary);

    //Check if the file is opening
    if(!stream.is_open())
        throw runtime_error("Could not open file.");

    //Read in each element of the .tga file
    stream.read((char*)&header.idLength , sizeof(header.idLength));
    stream.read((char*)&header.colorMapType , sizeof(header.colorMapType));
    stream.read((char*)&header.dataTypeCode , sizeof(header.dataTypeCode));
    stream.read((char*)&header.colorMapOrigin , sizeof(header.colorMapOrigin));
    stream.read((char*)&header.colorMapLength , sizeof(header.colorMapLength));
    stream.read((char*)&header.colorMapDepth , sizeof(header.colorMapDepth));
    stream.read((char*)&header.xOrigin , sizeof(header.xOrigin));
    stream.read((char*)&header.yOrigin, sizeof(header.yOrigin));
    stream.read((char*)&header.width , sizeof(header.width));
    stream.read((char*)&header.height , sizeof(header.height));
    stream.read((char*)&header.bitsPerPixel , sizeof(header.bitsPerPixel));
    stream.read((char*)&header.imageDescriptor , sizeof(header.imageDescriptor));


    int imageSize = (int)header.height * header.width;  //Get the image size by multiplying the image height and width


    for(int i = 0; i < imageSize; i++) {    //Create a for loop to read in all the pixels
        Pixel pixel;                        //Create a new pixel object for each iteration

        stream.read((char*)&pixel.blue, sizeof(pixel.blue));     //Read in all the pixels
        stream.read((char*)&pixel.green, sizeof(pixel.green));   //.tga files are read in reverse order rgb -> bgr
        stream.read((char*)&pixel.red, sizeof(pixel.red));
        pixelVect.push_back(pixel);                                     //Push in each pixel to the pixel vector
    }
}

void Image::write(const string& outName) {
    ofstream stream(outName, ios_base::binary);      //Create a .tga file
    stream.write((char*)&header.idLength , sizeof(header.idLength));        //Write out each .tga file header element
    stream.write((char*)&header.colorMapType , sizeof(header.colorMapType));
    stream.write((char*)&header.dataTypeCode , sizeof(header.dataTypeCode));
    stream.write((char*)&header.colorMapOrigin , sizeof(header.colorMapOrigin));
    stream.write((char*)&header.colorMapLength , sizeof(header.colorMapLength));
    stream.write((char*)&header.colorMapDepth , sizeof(header.colorMapDepth));
    stream.write((char*)&header.xOrigin , sizeof(header.xOrigin));
    stream.write((char*)&header.yOrigin, sizeof(header.yOrigin));
    stream.write((char*)&header.width , sizeof(header.width));
    stream.write((char*)&header.height , sizeof(header.height));
    stream.write((char*)&header.bitsPerPixel , sizeof(header.bitsPerPixel));
    stream.write((char*)&header.imageDescriptor , sizeof(header.imageDescriptor));

    for(int i = 0; i < pixelVect.size(); i++) {     //Create a for loop to write out each pixel
        Pixel pixel = pixelVect[i];
        //(25) Write out each image pixel
        stream.write((char*)&pixel.blue, sizeof(pixel.blue));
        stream.write((char*)&pixel.green, sizeof(pixel.green));
        stream.write((char*)&pixel.red, sizeof(pixel.red));
    }
    stream.close();
}
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include "Image.h"
#include <cstring> // This for strcmp
#include <stdexcept> // Error handling

using namespace std;


float Clamp(float value) {
    if(value > 255) {value = 255;}
    if(value < 0) {value = 0;}
    return value;
}

float Normalize(int value) {
    float holder = value;
    return (holder/255.0f);
}

void Multiply(Image& topLayer, const Image& bottomLayer) {
    vector<Pixel>& topPixels = topLayer.pixelVect;
    const vector<Pixel>& bottomPixels = bottomLayer.pixelVect;

    for (unsigned int i = 0; i < topPixels.size(); i++) {
        topPixels[i].red = static_cast<unsigned char>(Normalize(topPixels[i].red * bottomPixels[i].red) + 0.5f);
        topPixels[i].green = static_cast<unsigned char>(Normalize(topPixels[i].green * bottomPixels[i].green) + 0.5f);
        topPixels[i].blue = static_cast<unsigned char>(Normalize(topPixels[i].blue * bottomPixels[i].blue) + 0.5f);
    }
}

void Subtract(Image& topLayer, Image& bottomLayer) {
    vector<Pixel>& topPixels = topLayer.pixelVect;
    vector<Pixel>& bottomPixels = bottomLayer.pixelVect;

    for (unsigned int i = 0; i < topPixels.size(); i++) {
        int newRed = Clamp(bottomPixels[i].red - topPixels[i].red);
        int newGreen = Clamp(bottomPixels[i].green - topPixels[i].green);
        int newBlue = Clamp(bottomPixels[i].blue - topPixels[i].blue);

        bottomPixels[i].red = static_cast<unsigned char>(newRed);
        bottomPixels[i].green = static_cast<unsigned char>(newGreen);
        bottomPixels[i].blue = static_cast<unsigned char>(newBlue);
    }
}

void Screen(Image& topLayer, Image& bottomLayer) {
    vector<Pixel>& topPixels = topLayer.pixelVect;
    vector<Pixel>& bottomPixels = bottomLayer.pixelVect;

    for (unsigned int i = 0; i < topPixels.size(); i++) {

        float screenRed = 1.0f - (1.0f - Normalize(topPixels[i].red)) * (1.0f - Normalize(bottomPixels[i].red));
        float screenBlue = 1.0f - (1.0f - Normalize(topPixels[i].blue)) * (1.0f - Normalize(bottomPixels[i].blue));
        float screenGreen = 1.0f - (1.0f - Normalize(topPixels[i].green)) * (1.0f - Normalize(bottomPixels[i].green));

        bottomPixels[i].red = static_cast<unsigned char>(Clamp((screenRed * 255.0f) + 0.5f));
        bottomPixels[i].green = static_cast<unsigned char>(Clamp((screenGreen * 255.0f) + 0.5f));
        bottomPixels[i].blue = static_cast<unsigned char>(Clamp((screenBlue * 255.0f) + 0.5f));

    }
}

float OverlayTask5(float A, float B) {
    if(B <= 0.5) {
        B = Clamp((2 * B * A * 255)+0.5f);
    }
    else {
        B = Clamp((255 * (1 - (2 * (1 - A) * (1 - B)))) + 0.5f);
    }

    return B;
}

void Overlay(Image& topLayer, Image& patternLayer) {
    vector<Pixel>& patternPixels = patternLayer.pixelVect;
    const vector<Pixel>& topPixels = topLayer.pixelVect;

    for (unsigned int i = 0; i < patternPixels.size(); i++) {
        float aRed = Normalize(patternPixels[i].red);
        float aGreen = Normalize(patternPixels[i].green);
        float aBlue = Normalize(patternPixels[i].blue);
        float bRed = Normalize(topPixels[i].red);
        float bGreen = Normalize(topPixels[i].green);
        float bBlue = Normalize(topPixels[i].blue);

        float newRed = OverlayTask5(aRed, bRed);
        float newGreen = OverlayTask5(aGreen, bGreen);
        float newBlue = OverlayTask5(aBlue, bBlue);

        patternPixels[i].red = static_cast<unsigned char>(newRed);
        patternPixels[i].green = static_cast<unsigned char>(newGreen);
        patternPixels[i].blue = static_cast<unsigned char>(newBlue);
    }
}

void AddGreen(Image& image, int amount) {
    vector<Pixel>& pixels = image.pixelVect;
    for (unsigned int i = 0; i < pixels.size(); i++) {
        int newGreen = Clamp(pixels[i].green + amount);
        pixels[i].green = static_cast<unsigned char>(newGreen);
    }
}

void AddRed(Image& image, int amount) {
    vector<Pixel>& pixels = image.pixelVect;
    for (unsigned int i = 0; i < pixels.size(); i++) {
        int newRed = Clamp(pixels[i].red + amount);
        pixels[i].red = static_cast<unsigned char>(newRed);
    }
}

void AddBlue(Image& image, int amount) {
    vector<Pixel>& pixels = image.pixelVect;
    for (unsigned int i = 0; i < pixels.size(); i++) {
        int newBlue = Clamp(pixels[i].blue + amount);
        pixels[i].blue = static_cast<unsigned char>(newBlue);
    }
}

void ScaleRed(Image& image, float amount) {
    vector<Pixel>& pixels = image.pixelVect;

    for (unsigned int i = 0; i < pixels.size(); i++) {
        pixels[i].red = Clamp(pixels[i].red * amount);
    }
}

void ScaleGreen(Image& image, float amount) {
    vector<Pixel>& pixels = image.pixelVect;

    for (unsigned int i = 0; i < pixels.size(); i++) {
        pixels[i].green = Clamp(pixels[i].green * amount);
    }
}

void ScaleBlue(Image& image, float amount) {
    vector<Pixel>& pixels = image.pixelVect;

    for (unsigned int i = 0; i < pixels.size(); i++) {
        pixels[i].blue = Clamp(pixels[i].blue * amount);
    }
}

void ModifyRedAndBlue(Image& image) {
    vector<Pixel>& pixels = image.pixelVect;

    for (unsigned int i = 0; i < pixels.size(); i++) {
        pixels[i].red = Clamp(pixels[i].red * 4);
        pixels[i].blue = Clamp(pixels[i].blue * 0);
    }
}

void RedChannel(Image& image) {
    for (unsigned int i = 0; i < image.pixelVect.size(); i++) {
        image.pixelVect[i].blue = image.pixelVect[i].red;
        image.pixelVect[i].green = image.pixelVect[i].red;
    }
}

void GreenChannel(Image& image) {
    for (unsigned int i = 0; i < image.pixelVect.size(); i++) {
        image.pixelVect[i].red = image.pixelVect[i].green;
        image.pixelVect[i].blue = image.pixelVect[i].green;
    }
}

void BlueChannel(Image& image) {
    for (unsigned int i = 0; i < image.pixelVect.size(); i++) {
        image.pixelVect[i].red = image.pixelVect[i].blue;
        image.pixelVect[i].green = image.pixelVect[i].blue;
    }
}

void Combine3Images(Image& redLayer, Image& greenLayer, Image& blueLayer) {
    redLayer.read("input/layer_red.tga");
    greenLayer.read("input/layer_green.tga");
    blueLayer.read("input/layer_blue.tga");

    for (unsigned int i = 0; i < redLayer.pixelVect.size(); i++) {
        redLayer.pixelVect[i].red = redLayer.pixelVect[i].red;
        redLayer.pixelVect[i].green = greenLayer.pixelVect[i].green;
        redLayer.pixelVect[i].blue = blueLayer.pixelVect[i].blue;
    }

    redLayer.write("output/part9.tga");
}

void FlipImageTwoArguments(Image& InitialImage, Image& FlippedImage) {
    FlippedImage.header = InitialImage.header;
    FlippedImage.pixelVect.resize(InitialImage.header.width * InitialImage.header.height);

    for (unsigned int i = 0; i < InitialImage.header.height; i++) {
        for (unsigned int j = 0; j < InitialImage.header.width; j++) {
            int origPixel = i * InitialImage.header.width + j;
            int newPixel = (InitialImage.header.width - j - 1) + (InitialImage.header.height - i - 1) * InitialImage.header.width;

            FlippedImage.pixelVect[newPixel] = InitialImage.pixelVect[origPixel];
        }
    }
}

void FlipImage(Image& InitialImage) {
    Image FlippedImage;
    FlippedImage.pixelVect.resize(InitialImage.header.width * InitialImage.header.height);

    for (unsigned int i = 0; i < InitialImage.header.height; i++) {
        for (unsigned int j = 0; j < InitialImage.header.width; j++) {
            int origPixel = i * InitialImage.header.width + j;
            int newPixel = (InitialImage.header.width - j - 1) + (InitialImage.header.height - i - 1) * InitialImage.header.width;

            FlippedImage.pixelVect[newPixel] = InitialImage.pixelVect[origPixel];
        }
    }

    InitialImage.pixelVect = FlippedImage.pixelVect;
}

bool CheckFileName(string fileName) {

    int sizeOfName = fileName.size();
    if (sizeOfName < 4) {
        return false;
    }
    else {
        string TGA = fileName.substr(sizeOfName - 4, 4);
        string str = ".tga";
        if (TGA.compare(str) == 0)
            return true;
        return false;
    }
}

bool CheckValid(string fileName) {
    if(!CheckFileName(fileName)) {
        return false;
    }
    ifstream testfile;
    testfile.open(fileName, ios_base::binary);
    if(testfile.is_open())
        return true;
    else {
        return false;
    }
}

bool CheckInteger(string integer) {
    for (char digit : integer) {
        if (!isdigit(digit))
            return false;
    }
    return true;
}

int main(int argc, char* argv[]) {

    Image imageTracker;

    // argv[1] = outputFileName;
    //argv[2] = inputFileName;

    if (argc < 2) {
        cout << "Project 2: Image Processing, Fall 2023" << endl;
        cout << "\nUsage:\n";
        cout << "\t./project2.out [output] [firstImage] [method] [...]\n";
        return 0;
    }
    if (strcmp(argv[1], "--help") == 0) {
        cout << "Project 2: Image Processing, Fall 2023" << endl;
        cout << "\nUsage:\n";
        cout << "\t./project2.out [output] [firstImage] [method] [...]\n";
        return 0;
    }

    if (argc < 3) {
        cout << "Invalid file name." << endl;
    }

    if (!CheckFileName(argv[1])) {
        cout << "Invalid file name." << endl;
        return 0;
    }

    if (!CheckFileName(argv[2])) {
        cout << "Invalid file name." << endl;
        return 0;
    }

    if (!CheckValid(argv[2])) {
        cout << "File does not exist." << endl;
        return 0;
    }

    imageTracker.read(argv[2]);

    int argIndex = 3;

    while (argIndex <= argc - 1) {
        if (strcmp(argv[argIndex], "multiply") == 0) {
            if (argIndex + 2 > argc) {
                cout << "Missing argument." << endl;
                return 0;
            }

            if (!CheckFileName(argv[argIndex + 1])) {
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }

            if (!CheckValid(argv[argIndex + 1])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            Image image;
            image.read(argv[argIndex + 1]);

            Multiply(imageTracker, image);

            cout << "Multiplying..... " << endl;

            argIndex+=2;
            continue;

        }

        else if (strcmp(argv[argIndex], "subtract") == 0) {
            if (argIndex + 2 > argc) {
                cout << "Missing argument." << endl;
                return 0;
            }

            if (!CheckFileName(argv[argIndex + 1])) {
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }

            if (!CheckValid(argv[argIndex + 1])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            Image image;
            image.read(argv[argIndex + 1]);

            Subtract(image, imageTracker);

            cout << "Subtracting... " << endl;

            argIndex+=2;
            continue;

        }

        else if (strcmp(argv[argIndex], "screen") == 0) {
            if (argIndex + 2 > argc) {
                cout << "Missing argument." << endl;
                return 0;
            }

            if (!CheckFileName(argv[argIndex + 1])) {
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }

            if (!CheckValid(argv[argIndex + 1])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            Image image;
            image.read(argv[argIndex + 1]);
            Screen(image, imageTracker);

            cout << "Screening... " << endl;

            argIndex+=2;
            continue;

        }

        else if (strcmp(argv[argIndex], "overlay") == 0) {
            if (argIndex + 2 > argc) {
                cout << "Missing argument." << endl;
                return 0;
            }

            if (!CheckFileName(argv[argIndex + 1])) {
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }

            if (!CheckValid(argv[argIndex + 1])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            Image image;
            image.read(argv[argIndex + 1]);

            Overlay(image, imageTracker);

            cout << "Overlay... " << endl;

            argIndex+=2;
            continue;
        }

        else if (strcmp(argv[argIndex], "addred") == 0) {
            if (argIndex + 2 > argc) {
                cout << "Missing argument." << endl;
                return 0;
            }

            if (!CheckInteger(argv[argIndex + 1])) {
                cout << "Invalid argument, expected number." << endl;
            }

            AddRed(imageTracker, stoi(argv[argIndex + 1]));

            argIndex+=2;
            continue;

        }

        else if (strcmp(argv[argIndex], "addgreen") == 0) {
            if (argIndex + 2 > argc) {
                cout << "Missing argument." << endl;
                return 0;
            }

            if (!CheckInteger(argv[argIndex + 1])) {
                cout << "Invalid argument, expected number." << endl;
            }

            AddGreen(imageTracker, stoi(argv[argIndex + 1]));

            cout << "Adding green... " << endl;

            argIndex+=2;
            continue;

        }

        else if (strcmp(argv[argIndex], "addblue") == 0) {
            if (argIndex + 2 > argc) {
                cout << "Missing argument." << endl;
                return 0;
            }

            if (!CheckInteger(argv[argIndex + 1])) {
                cout << "Invalid argument, expected number." << endl;
            }

            AddBlue(imageTracker, stoi(argv[argIndex + 1]));

            cout << "Adding blue... " << endl;

            argIndex+=2;
            continue;

        }

        else if (strcmp(argv[argIndex], "scalered") == 0) {

            if (argIndex + 2 > argc) {
                cout << "Missing argument." << endl;
                return 0;
            }

            if (!CheckInteger(argv[argIndex + 1])) {
                cout << "Invalid argument, expected number." << endl;
            }

            ScaleRed(imageTracker, stoi(argv[argIndex + 1]));

            cout << "Scaling red... " << endl;

            argIndex+=2;
            continue;

        }

        else if (strcmp(argv[argIndex], "scaleblue") == 0) {

            if (argIndex + 2 > argc) {
                cout << "Missing argument." << endl;
                return 0;
            }

            if (!CheckInteger(argv[argIndex + 1])) {
                cout << "Invalid argument, expected number." << endl;
            }

            ScaleBlue(imageTracker, stoi(argv[argIndex + 1]));

            cout << "Scaling blue... " << endl;

            argIndex+=2;
            continue;

        }

        else if (strcmp(argv[argIndex], "scalegreen") == 0) {

            if (argIndex + 2 > argc) {
                cout << "Missing argument." << endl;
                return 0;
            }

            if (!CheckInteger(argv[argIndex + 1])) {
                cout << "Invalid argument, expected number." << endl;
            }

            ScaleGreen(imageTracker, stoi(argv[argIndex + 1]));

            cout << "Scaling green... " << endl;

            argIndex+=2;
            continue;

        }

        else if (strcmp(argv[argIndex], "onlyred") == 0) {

            RedChannel(imageTracker);
            argIndex++;
            continue;

        }

        else if (strcmp(argv[argIndex], "onlyblue") == 0) {

            BlueChannel(imageTracker);
            argIndex++;
            continue;

        }

        else if (strcmp(argv[argIndex], "onlygreen") == 0) {

            GreenChannel(imageTracker);
            argIndex++;
            continue;

        }

        else if (strcmp(argv[argIndex], "flip") == 0) {

                FlipImage(imageTracker);

                cout << "Flipping... " << endl;
                argIndex++;
                continue;

        }

        else if (strcmp(argv[argIndex], "combine") == 0) {


            if (argIndex + 2 > argc) {
                cout << "Missing argument." << endl;
                return 0;
            }

            if (!CheckFileName(argv[argIndex + 1])) {
                cout << "Invalid argument, invalid file name." << endl;
                return 0;
            }

            if (!CheckValid(argv[argIndex + 1])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }

            if (!CheckValid(argv[argIndex + 2])) {
                cout << "Invalid argument, file does not exist." << endl;
                return 0;
            }


            Image greenChannel;
            greenChannel.read(argv[argIndex + 1]);

            Image blueChannel;
            blueChannel.read(argv[argIndex + 2]);

            Combine3Images(imageTracker, greenChannel, blueChannel);

            cout << "Combine... " << endl;

            argIndex += 3;
            continue;

        }

        else {
            cout << "Invalid method name." << endl;
            return 0;
        }

        argIndex++;
    }

    imageTracker.write(argv[1]);

    return 0;
}

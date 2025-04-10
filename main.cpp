#include <iostream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

uint16_t ConvertToRGB565(uint8_t r, uint8_t g, uint8_t b) {
    uint16_t rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    return rgb565;
}

int main() {
    const char* filename = "AF_12_31_2024.PNG"; // Replace with your image file
    int width, height, channels;
    uint8_t* img = stbi_load(filename, &width, &height, &channels, 3); // Load as 3 channels (RGB)

    int ausschnittStartW = 18; // set 0 for orig image
    int ausschnittStartH = 20; // set 0 for orig image
    int ausschnittWidth = 60;  // set = width for orig image
    int ausschnittHeight = 60; // set = height for orig image
    int scaleW = 6;  // set 1 for orig image
    int scaleH = 6;  // set 1 for orig image

    if (!img) {
        std::cerr << "Failed to load image!" << std::endl;
        return -1;
    }

    std::ofstream output("output_hex.txt");
    if (!output) {
        std::cerr << "Failed to open output file!" << std::endl;
        stbi_image_free(img);
        return -1;
    }

    output << "const uint16_t image_dump[" << ausschnittHeight << "][" << ausschnittWidth << "] = ";
    output << "{";
    
    for (int y = ausschnittStartH; y < ausschnittStartH + ausschnittHeight; ++y) {
        output << "{";
        for (int x = ausschnittStartW; x < ausschnittStartW + ausschnittWidth; ++x) {
            int idx = (scaleH*y * width + scaleW*x) * 3;
            uint8_t r = img[idx];
            uint8_t g = img[idx + 1];
            uint8_t b = img[idx + 2];
            uint16_t rgb565 = ConvertToRGB565(r, g, b);
            output << "0x" << std::hex << rgb565;
            if (x != ausschnittStartW + ausschnittWidth - 1) output << ",";
        }
        if (y != ausschnittStartH + ausschnittHeight - 1) {
            output << "}," << std::endl;
        }
        else {
            output << "}" << std::endl;
        }
    }
    output << "};";

    output.close();
    stbi_image_free(img);
    std::cout << "Hex dump saved to output_hex.txt" << std::endl;

    return 0;
}
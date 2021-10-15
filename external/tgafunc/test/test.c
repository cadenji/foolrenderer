// Copyright (c) 2021 Caden Ji
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tgafunc.h"

static void create_test(void) {
    uint8_t *data;
    tga_info *info;
    enum tga_error error_code;
    int size = 4;
    int oversize = TGA_MAX_IMAGE_DIMENSIONS + 1;

    // image size cannot be less than 1.
    error_code = tga_create(&data, &info, 0, size, TGA_PIXEL_RGB24);
    assert(error_code == TGA_ERROR_INVALID_IMAGE_DIMENSIONS);
    error_code = tga_create(&data, &info, size, 0, TGA_PIXEL_RGB24);
    assert(error_code == TGA_ERROR_INVALID_IMAGE_DIMENSIONS);
    error_code = tga_create(&data, &info, -1, size, TGA_PIXEL_RGB24);
    assert(error_code == TGA_ERROR_INVALID_IMAGE_DIMENSIONS);
    error_code = tga_create(&data, &info, size, -1, TGA_PIXEL_RGB24);
    assert(error_code == TGA_ERROR_INVALID_IMAGE_DIMENSIONS);
    // Image size cannot be greater than TGA_MAX_IMAGE_DIMENSISNS.
    error_code = tga_create(&data, &info, oversize, size, TGA_PIXEL_RGB24);
    assert(error_code == TGA_ERROR_INVALID_IMAGE_DIMENSIONS);
    error_code = tga_create(&data, &info, size, oversize, TGA_PIXEL_RGB24);
    assert(error_code == TGA_ERROR_INVALID_IMAGE_DIMENSIONS);
    // Wrong pixel format check.
    error_code =
        tga_create(&data, &info, size, size, (enum tga_pixel_format)100);
    assert(error_code == TGA_ERROR_UNSUPPORTED_PIXEL_FORMAT);

    // This time it should have succeeded.
    error_code = tga_create(&data, &info, size, size, TGA_PIXEL_RGB24);
    assert(error_code == TGA_NO_ERROR);
    assert(tga_get_image_width(info) == size);
    assert(tga_get_image_height(info) == size);
    assert(tga_get_pixel_format(info) == TGA_PIXEL_RGB24);
    assert(tga_get_bytes_per_pixel(info) == 3);
    tga_free_data(data);
    tga_free_info(info);

    // Check whether the created image data is all 0.
    int data_size = size * size;
    // The size of the temp should be greater than the total size of the
    // tga_info structure and image data. Ensure that the memory allocated by
    // image data is written in advance.
    int temp_size = data_size + 1024;
    uint8_t *temp = (uint8_t *)malloc(temp_size);
    // Write a non-zero value to the memory space that will be used as image
    // data.
    memset(temp, 0x80, temp_size);
    free(temp);
    error_code = tga_create(&data, &info, size, size, TGA_PIXEL_BW8);
    assert(error_code == TGA_NO_ERROR);
    for (int i = 0; i < data_size; i++) {
        assert(data[i] == 0);
    }
    tga_free_data(data);
    tga_free_info(info);
}

static void load_test(void) {
    const int image_size = 128;
    const char image_path[] = "images/";
    const char *image_name_list[] = {
        "CBW8.TGA", "CCM8.TGA", "CTC16.TGA", "CTC24.TGA", "CTC32.TGA",
        "UBW8.TGA", "UCM8.TGA", "UTC16.TGA", "UTC24.TGA", "UTC32.TGA"};
    enum tga_pixel_format pixel_format_list[] = {
        TGA_PIXEL_BW8,    TGA_PIXEL_RGB555, TGA_PIXEL_RGB555, TGA_PIXEL_RGB24,
        TGA_PIXEL_ARGB32, TGA_PIXEL_BW8,    TGA_PIXEL_RGB555, TGA_PIXEL_RGB555,
        TGA_PIXEL_RGB24,  TGA_PIXEL_ARGB32};

    char image_name[128];
    uint8_t *data_array[2];
    tga_info *info_array[2];

    // Test the loading correctness of RLE images.
    int image_count = sizeof(image_name_list) / sizeof(image_name_list[0]);
    int group_size = image_count / 2;
    for (int i = 0; i < group_size; i++) {
        for (int j = 0; j < 2; j++) {
            int list_index = j * group_size + i;
            // Create file name.
            memcpy(image_name, image_path, sizeof(image_path));
            strcat(image_name, image_name_list[list_index]);
            // Load the image and check the image information.
            enum tga_error error_code;
            error_code = tga_load(&data_array[j], &info_array[j], image_name);
            assert(error_code == TGA_NO_ERROR);
            if (tga_get_image_width(info_array[j]) != image_size ||
                tga_get_image_height(info_array[j]) != image_size ||
                tga_get_pixel_format(info_array[j]) !=
                    pixel_format_list[list_index]) {
                // The loaded image information is wrong.
                assert(0);
            }
        }
        // Compare whether the loaded two groups of image data are consistent;
        uint8_t pixel_size = tga_get_bytes_per_pixel(info_array[0]);
        size_t data_size = (size_t)image_size * image_size * pixel_size;
        int result = memcmp(data_array[0], data_array[1], data_size);
        assert(result == 0);
        for (int k = 0; k < 2; k++) {
            tga_free_data(data_array[k]);
            tga_free_info(info_array[k]);
        }
    }

    // Test the loading correctness of color mapped images.
    // By comparing whether the loaded data of UCM8.TGA and UTC16.TGA are the
    // same.
    for (int i = 0; i < 2; i++) {
        int list_index = 6 + i;
        memcpy(image_name, image_path, sizeof(image_path));
        strcat(image_name, image_name_list[list_index]);
        enum tga_error error_code;
        error_code = tga_load(&data_array[i], &info_array[i], image_name);
        assert(error_code == TGA_NO_ERROR);
    }
    uint8_t pixel_size = tga_get_bytes_per_pixel(info_array[0]);
    size_t data_size = (size_t)image_size * image_size * pixel_size;
    int result = memcmp(data_array[0], data_array[1], data_size);
    assert(result == 0);
    for (int k = 0; k < 2; k++) {
        tga_free_data(data_array[k]);
        tga_free_info(info_array[k]);
    }
}

int main(int argc, char *argv[]) {
    create_test();
    load_test();
    puts("Test cases passed.");
    return 0;
}

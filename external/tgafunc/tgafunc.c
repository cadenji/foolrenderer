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

#include "tgafunc.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tga_info {
    uint16_t width, height;
    enum tga_pixel_format pixel_format;
};

static inline bool check_dimensions(int width, int height);

static inline int pixel_format_to_pixel_size(enum tga_pixel_format format);

static enum tga_error load_image(uint8_t **data_out, tga_info **info_out,
                                 FILE *file);

static inline uint8_t *get_pixel(uint8_t *data, const tga_info *info, int x,
                                 int y);

static enum tga_error save_image(const uint8_t *data, const tga_info *info,
                                 FILE *file);

enum tga_error tga_create(uint8_t **data_out, tga_info **info_out, int width,
                          int height, enum tga_pixel_format format) {
    if (check_dimensions(width, height)) {
        return TGA_ERROR_INVALID_IMAGE_DIMENSIONS;
    }
    int pixel_size = pixel_format_to_pixel_size(format);
    if (pixel_size == -1) {
        return TGA_ERROR_UNSUPPORTED_PIXEL_FORMAT;
    }

    // Creates image data and info structure.
    uint8_t *data = (uint8_t *)calloc((size_t)width * height, pixel_size);
    if (data == NULL) {
        return TGA_ERROR_OUT_OF_MEMORY;
    }
    tga_info *info = (tga_info *)malloc(sizeof(tga_info));
    if (info == NULL) {
        free(data);
        return TGA_ERROR_OUT_OF_MEMORY;
    }
    info->width = width;
    info->height = height;
    info->pixel_format = format;

    *data_out = data;
    *info_out = info;
    return TGA_NO_ERROR;
}

enum tga_error tga_load(uint8_t **data_out, tga_info **info_out,
                        const char *file_name) {
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        return TGA_ERROR_FILE_CANNOT_READ;
    }
    enum tga_error error_code = load_image(data_out, info_out, file);
    fclose(file);
    return error_code;
}

enum tga_error tga_save(const uint8_t *data, int width, int height,
                        enum tga_pixel_format format, const char *file_name) {
    if (check_dimensions(width, height)) {
        return TGA_ERROR_INVALID_IMAGE_DIMENSIONS;
    }
    tga_info info = {width, height, format};
    return tga_save_from_info(data, &info, file_name);
}

enum tga_error tga_save_from_info(const uint8_t *data, const tga_info *info,
                                  const char *file_name) {
    if (data == NULL || info == NULL) {
        return TGA_ERROR_NO_DATA;
    }
    // Check if a file with the same name already exists.
    FILE *file = fopen(file_name, "r");
    if (file != NULL) {
        fclose(file);
        return TGA_ERROR_FILE_CANNOT_WRITE;
    }
    file = fopen(file_name, "wb");
    if (file == NULL) {
        return TGA_ERROR_FILE_CANNOT_WRITE;
    }

    enum tga_error error_code = save_image(data, info, file);
    fclose(file);
    if (error_code != TGA_NO_ERROR) {
        remove(file_name);
    }
    return error_code;
}

int tga_get_image_width(const tga_info *info) { return info->width; }

int tga_get_image_height(const tga_info *info) { return info->height; }

enum tga_pixel_format tga_get_pixel_format(const tga_info *info) {
    return info->pixel_format;
}

uint8_t tga_get_bytes_per_pixel(const tga_info *info) {
    return pixel_format_to_pixel_size(info->pixel_format);
}

uint8_t *tga_get_pixel(uint8_t *data, const tga_info *info, int x, int y) {
    return get_pixel(data, info, x, y);
}

void tga_free_data(void *data) { free(data); }

void tga_free_info(tga_info *info) { free(info); }

void tga_image_flip_h(uint8_t *data, const tga_info *info) {
    if (data == NULL || info == NULL) {
        return;
    }
    int pixel_size = pixel_format_to_pixel_size(info->pixel_format);
    uint8_t temp[pixel_size];
    int flip_num = info->width / 2;
    for (int i = 0; i < flip_num; ++i) {
        for (int j = 0; j < info->height; ++j) {
            uint8_t *p1 = get_pixel(data, info, i, j);
            uint8_t *p2 = get_pixel(data, info, info->width - 1 - i, j);
            // Swap two pixels.
            memcpy(temp, p1, pixel_size);
            memcpy(p1, p2, pixel_size);
            memcpy(p2, temp, pixel_size);
        }
    }
}

void tga_image_flip_v(uint8_t *data, const tga_info *info) {
    if (data == NULL || info == NULL) {
        return;
    }
    int pixel_size = pixel_format_to_pixel_size(info->pixel_format);
    uint8_t temp[pixel_size];
    int flip_num = info->height / 2;
    for (int i = 0; i < flip_num; ++i) {
        for (int j = 0; j < info->width; ++j) {
            uint8_t *p1 = get_pixel(data, info, j, i);
            uint8_t *p2 = get_pixel(data, info, j, info->height - 1 - i);
            // Swap two pixels.
            memcpy(temp, p1, pixel_size);
            memcpy(p1, p2, pixel_size);
            memcpy(p2, temp, pixel_size);
        }
    }
}

enum tga_image_type {
    TGA_TYPE_NO_DATA = 0,
    TGA_TYPE_COLOR_MAPPED = 1,
    TGA_TYPE_TRUE_COLOR = 2,
    TGA_TYPE_GRAYSCALE = 3,
    TGA_TYPE_RLE_COLOR_MAPPED = 9,
    TGA_TYPE_RLE_TRUE_COLOR = 10,
    TGA_TYPE_RLE_GRAYSCALE = 11
};

struct tga_header {
    uint8_t id_length;
    uint8_t map_type;
    uint8_t image_type;

    // Color map specification.
    uint16_t map_first_entry;
    uint16_t map_length;
    uint8_t map_entry_size;

    // Image specification.
    uint16_t image_x_origin;
    uint16_t image_y_origin;
    uint16_t image_width;
    uint16_t image_height;
    uint8_t pixel_depth;
    uint8_t image_descriptor;
};

struct color_map {
    uint16_t first_index;
    uint16_t entry_count;
    uint8_t bytes_per_entry;
    uint8_t *pixels;
};

#define HEADER_SIZE 18

#define IS_SUPPORTED_IMAGE_TYPE(header)                  \
    ((header).image_type == TGA_TYPE_COLOR_MAPPED ||     \
     (header).image_type == TGA_TYPE_TRUE_COLOR ||       \
     (header).image_type == TGA_TYPE_GRAYSCALE ||        \
     (header).image_type == TGA_TYPE_RLE_COLOR_MAPPED || \
     (header).image_type == TGA_TYPE_RLE_TRUE_COLOR ||   \
     (header).image_type == TGA_TYPE_RLE_GRAYSCALE)

#define IS_COLOR_MAPPED(header)                      \
    ((header).image_type == TGA_TYPE_COLOR_MAPPED || \
     (header).image_type == TGA_TYPE_RLE_COLOR_MAPPED)

#define IS_TRUE_COLOR(header)                      \
    ((header).image_type == TGA_TYPE_TRUE_COLOR || \
     (header).image_type == TGA_TYPE_RLE_TRUE_COLOR)

#define IS_GRAYSCALE(header)                      \
    ((header).image_type == TGA_TYPE_GRAYSCALE || \
     (header).image_type == TGA_TYPE_RLE_GRAYSCALE)

#define IS_RLE(header)                                   \
    ((header).image_type == TGA_TYPE_RLE_COLOR_MAPPED || \
     (header).image_type == TGA_TYPE_RLE_TRUE_COLOR ||   \
     (header).image_type == TGA_TYPE_RLE_GRAYSCALE)

// Convert bits to integer bytes. E.g. 8 bits to 1 byte, 9 bits to 2 bytes.
#define BITS_TO_BYTES(bit_count) (((bit_count)-1) / 8 + 1)

static bool has_read_file_error = false;

// Reads a 8-bit integer from the file stream.
static inline uint8_t read_uint8(FILE *file) {
    uint8_t value;
    if (fread(&value, 1, 1, file) != 1) {
        has_read_file_error = true;
        return 0;
    }
    return value;
}

// Gets a 16-bit little-endian integer from the file stream.
// This function should works on both big-endian and little-endian architecture
// systems.
static inline uint16_t read_uint16_le(FILE *file) {
    uint8_t buffer[2];
    if (fread(&buffer, 1, 2, file) != 2) {
        has_read_file_error = true;
        return 0;
    }
    return buffer[0] + (((uint16_t)buffer[1]) << 8);
}

// Checks if the picture size is correct.
// Returns true if invalid dimensisns, otherwise returns false.
static inline bool check_dimensions(int width, int height) {
    return width <= 0 || width > TGA_MAX_IMAGE_DIMENSIONS || height <= 0 ||
           height > TGA_MAX_IMAGE_DIMENSIONS;
}

// Gets the bytes per pixel by pixel format.
// Returns bytes per pxiel, otherwise returns -1 means the parameter `format` is
// invalid.
static inline int pixel_format_to_pixel_size(enum tga_pixel_format format) {
    switch (format) {
        case TGA_PIXEL_BW8:
            return 1;
        case TGA_PIXEL_BW16:
        case TGA_PIXEL_RGB555:
            return 2;
        case TGA_PIXEL_RGB24:
            return 3;
        case TGA_PIXEL_ARGB32:
            return 4;
        default:
            return -1;
    }
}

// Gets the pixel format according to the header.
// Returns false means the header is not illegal, otherwise returns true.
static bool get_pixel_format(enum tga_pixel_format *format,
                             const struct tga_header *header) {
    if (IS_COLOR_MAPPED(*header)) {
        // If the supported pixel_depth is changed, remember to also change
        // the pixel_to_map_index() function.
        if (header->pixel_depth == 8) {
            switch (header->map_entry_size) {
                case 15:
                case 16:
                    *format = TGA_PIXEL_RGB555;
                    return false;
                case 24:
                    *format = TGA_PIXEL_RGB24;
                    return false;
                case 32:
                    *format = TGA_PIXEL_ARGB32;
                    return false;
            }
        }
    } else if (IS_TRUE_COLOR(*header)) {
        switch (header->pixel_depth) {
            case 16:
                *format = TGA_PIXEL_RGB555;
                return false;
            case 24:
                *format = TGA_PIXEL_RGB24;
                return false;
            case 32:
                *format = TGA_PIXEL_ARGB32;
                return false;
        }
    } else if (IS_GRAYSCALE(*header)) {
        switch (header->pixel_depth) {
            case 8:
                *format = TGA_PIXEL_BW8;
                return false;
            case 16:
                *format = TGA_PIXEL_BW16;
                return false;
        }
    }
    return true;
}

// Loads TGA header from file stream and returns the pixel format.
static enum tga_error load_header(struct tga_header *header,
                                  enum tga_pixel_format *pixel_format,
                                  FILE *file) {
    has_read_file_error = false;

    header->id_length = read_uint8(file);
    header->map_type = read_uint8(file);
    header->image_type = read_uint8(file);
    header->map_first_entry = read_uint16_le(file);
    header->map_length = read_uint16_le(file);
    header->map_entry_size = read_uint8(file);
    header->image_x_origin = read_uint16_le(file);
    header->image_y_origin = read_uint16_le(file);
    header->image_width = read_uint16_le(file);
    header->image_height = read_uint16_le(file);
    header->pixel_depth = read_uint8(file);
    header->image_descriptor = read_uint8(file);

    if (has_read_file_error) {
        return TGA_ERROR_FILE_CANNOT_READ;
    }
    if (header->map_type > 1) {
        return TGA_ERROR_UNSUPPORTED_COLOR_MAP_TYPE;
    }
    if (header->image_type == TGA_TYPE_NO_DATA) {
        return TGA_ERROR_NO_DATA;
    }
    if (!IS_SUPPORTED_IMAGE_TYPE(*header)) {
        return TGA_ERROR_UNSUPPORTED_IMAGE_TYPE;
    }
    if (header->image_width <= 0 || header->image_height <= 0) {
        // No need to check if the image size exceeds TGA_MAX_IMAGE_DIMENSIONS.
        return TGA_ERROR_INVALID_IMAGE_DIMENSIONS;
    }
    if (get_pixel_format(pixel_format, header)) {
        return TGA_ERROR_UNSUPPORTED_PIXEL_FORMAT;
    }
    return TGA_NO_ERROR;
}

// Used for color mapped image decode.
static inline uint16_t pixel_to_map_index(uint8_t *pixel_ptr) {
    // Because only 8-bit index is supported now, so implemented in this way.
    return pixel_ptr[0];
}

// Gets the color of the specified index from the map.
// Returns false means no error, otherwise returns true.
static inline bool try_get_color_from_map(uint8_t *dest, uint16_t index,
                                          const struct color_map *map) {
    index -= map->first_index;
    if (index < 0 && index >= map->entry_count) {
        return true;
    }
    memcpy(dest, map->pixels + map->bytes_per_entry * index,
           map->bytes_per_entry);
    return false;
}

// Decode image data from file stream.
static enum tga_error decode_data(uint8_t *data, const tga_info *info,
                                  uint8_t pixel_size, bool is_color_mapped,
                                  const struct color_map *map, FILE *file) {
    enum tga_error error_code = TGA_NO_ERROR;
    size_t pixel_count = (size_t)info->width * info->height;

    if (is_color_mapped) {
        for (; pixel_count > 0; --pixel_count) {
            if (fread(data, 1, pixel_size, file) != pixel_size) {
                error_code = TGA_ERROR_FILE_CANNOT_READ;
                break;
            }
            // In color mapped image, the pixel as the index value of the color
            // map. The actual pixel value is found from the color map.
            uint16_t index = pixel_to_map_index(data);
            if (try_get_color_from_map(data, index, map)) {
                error_code = TGA_ERROR_COLOR_MAP_INDEX_FAILED;
                break;
            }
            data += map->bytes_per_entry;
        }
    } else {
        size_t data_size = pixel_count * pixel_size;
        if (fread(data, 1, data_size, file) != data_size) {
            error_code = TGA_ERROR_FILE_CANNOT_READ;
        }
    }
    return error_code;
}

// Decode image data with run-length encoding from file stream.
static enum tga_error decode_data_rle(uint8_t *data, const tga_info *info,
                                      uint8_t pixel_size, bool is_color_mapped,
                                      const struct color_map *map, FILE *file) {
    enum tga_error error_code = TGA_NO_ERROR;
    size_t pixel_count = (size_t)info->width * info->height;
    bool is_run_length_packet = false;
    uint8_t packet_count = 0;
    uint8_t pixel_buffer[is_color_mapped ? map->bytes_per_entry : pixel_size];
    // The actual pixel size of the image, In order not to be confused with the
    // name of the parameter pixel_size, named data element.
    uint8_t data_element_size = pixel_format_to_pixel_size(info->pixel_format);

    for (; pixel_count > 0; --pixel_count) {
        if (packet_count == 0) {
            uint8_t repetition_count_field;
            if (fread(&repetition_count_field, 1, 1, file) != 1) {
                error_code = TGA_ERROR_FILE_CANNOT_READ;
                break;
            }
            is_run_length_packet = repetition_count_field & 0x80;
            packet_count = (repetition_count_field & 0x7F) + 1;
            if (is_run_length_packet) {
                if (fread(pixel_buffer, 1, pixel_size, file) != pixel_size) {
                    error_code = TGA_ERROR_FILE_CANNOT_READ;
                    break;
                }
                if (is_color_mapped) {
                    // In color mapped image, the pixel as the index value of
                    // the color map. The actual pixel value is found from the
                    // color map.
                    uint16_t index = pixel_to_map_index(pixel_buffer);
                    if (try_get_color_from_map(pixel_buffer, index, map)) {
                        error_code = TGA_ERROR_COLOR_MAP_INDEX_FAILED;
                        break;
                    }
                }
            }
        }

        if (is_run_length_packet) {
            memcpy(data, pixel_buffer, data_element_size);
        } else {
            if (fread(data, 1, pixel_size, file) != pixel_size) {
                error_code = TGA_ERROR_FILE_CANNOT_READ;
                break;
            }
            if (is_color_mapped) {
                // Again, in color mapped image, the pixel as the index value of
                // the color map. The actual pixel value is found from the color
                // map.
                uint16_t index = pixel_to_map_index(data);
                if (try_get_color_from_map(data, index, map)) {
                    error_code = TGA_ERROR_COLOR_MAP_INDEX_FAILED;
                    break;
                }
            }
        }

        --packet_count;
        data += data_element_size;
    }
    return error_code;
}

static enum tga_error load_image(uint8_t **data_out, tga_info **info_out,
                                 FILE *file) {
    struct tga_header header;
    enum tga_pixel_format pixel_format;
    enum tga_error error_code;

    error_code = load_header(&header, &pixel_format, file);
    if (error_code != TGA_NO_ERROR) {
        return error_code;
    }
    // No need to handle the content of the ID field, so skip directly.
    if (fseek(file, header.id_length, SEEK_CUR)) {
        return TGA_ERROR_FILE_CANNOT_READ;
    }

    bool is_color_mapped = IS_COLOR_MAPPED(header);
    bool is_rle = IS_RLE(header);

    // Handle color map field.
    struct color_map color_map;
    color_map.pixels = NULL;
    size_t map_size = header.map_length * BITS_TO_BYTES(header.map_entry_size);
    if (is_color_mapped) {
        color_map.first_index = header.map_first_entry;
        color_map.entry_count = header.map_length;
        color_map.bytes_per_entry = BITS_TO_BYTES(header.map_entry_size);
        color_map.pixels = (uint8_t *)malloc(map_size);
        if (color_map.pixels == NULL) {
            return TGA_ERROR_OUT_OF_MEMORY;
        }
        if (fread(color_map.pixels, 1, map_size, file) != map_size) {
            free(color_map.pixels);
            return TGA_ERROR_FILE_CANNOT_READ;
        }
    } else if (header.map_type == 1) {
        // The image is not color mapped at this time, but contains a color map.
        // So skips the color map data block directly.
        if (fseek(file, map_size, SEEK_CUR)) {
            return TGA_ERROR_FILE_CANNOT_READ;
        }
    }

    uint8_t *data;
    tga_info *info;
    error_code = tga_create(&data, &info, header.image_width,
                            header.image_height, pixel_format);
    if (error_code != TGA_NO_ERROR) {
        free(color_map.pixels);
        return error_code;
    }

    // Load image data.
    uint8_t pixel_size = BITS_TO_BYTES(header.pixel_depth);
    if (is_rle) {
        error_code = decode_data_rle(data, info, pixel_size, is_color_mapped,
                                     &color_map, file);
    } else {
        error_code = decode_data(data, info, pixel_size, is_color_mapped,
                                 &color_map, file);
    }
    free(color_map.pixels);
    if (error_code != TGA_NO_ERROR) {
        tga_free_data(data);
        tga_free_info(info);
        return error_code;
    }

    // Flip the image if necessary, to keep the origin in upper left corner.
    bool flip_h = header.image_descriptor & 0x10;
    bool flip_v = !(header.image_descriptor & 0x20);
    if (flip_h) {
        tga_image_flip_h(data, info);
    }
    if (flip_v) {
        tga_image_flip_v(data, info);
    }

    *data_out = data;
    *info_out = info;
    return TGA_NO_ERROR;
}

// Returns the pixel at coordinates (x,y) for reading or writing.
// If the pixel coordinates are out of bounds (larger than width/height
// or small than 0), they will be clamped.
static inline uint8_t *get_pixel(uint8_t *data, const tga_info *info, int x,
                                 int y) {
    if (x < 0) {
        x = 0;
    } else if (x >= info->width) {
        x = info->width - 1;
    }
    if (y < 0) {
        y = 0;
    } else if (y >= info->height) {
        y = info->height - 1;
    }
    int pixel_size = pixel_format_to_pixel_size(info->pixel_format);
    return data + (y * info->width + x) * pixel_size;
}

static enum tga_error save_image(const uint8_t *data, const tga_info *info,
                                 FILE *file) {
    int pixel_size = pixel_format_to_pixel_size(info->pixel_format);
    uint8_t header[HEADER_SIZE];
    memset(header, 0, HEADER_SIZE);
    if (info->pixel_format == TGA_PIXEL_BW8 ||
        info->pixel_format == TGA_PIXEL_BW16) {
        header[2] = (uint8_t)TGA_TYPE_GRAYSCALE;
    } else {
        header[2] = (uint8_t)TGA_TYPE_TRUE_COLOR;
    }
    header[12] = info->width & 0xFF;
    header[13] = (info->width >> 8) & 0xFF;
    header[14] = info->height & 0xFF;
    header[15] = (info->height >> 8) & 0xFF;
    header[16] = pixel_size * 8;
    if (info->pixel_format == TGA_PIXEL_ARGB32) {
        header[17] = 0x28;
    } else {
        header[17] = 0x20;
    }

    if (fwrite(header, 1, HEADER_SIZE, file) != HEADER_SIZE) {
        return TGA_ERROR_FILE_CANNOT_WRITE;
    }

    size_t data_size = (size_t)info->width * info->height * pixel_size;
    if (fwrite(data, 1, data_size, file) != data_size) {
        return TGA_ERROR_FILE_CANNOT_WRITE;
    }
    return TGA_NO_ERROR;
}

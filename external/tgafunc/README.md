# TGAFunc

Lightweight library written in C for handling the Truevision TGA image format.

For more information about the TGA format, please refer to the
[specification](http://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf).

## Usage

Copy the `tgafunc.h` and `tgafunc.c` files to your porject and include the
`tgafunc.h` in your code. Please make sure that your compiler compliant with
the C99 standard or newer.

You can use the `tga_load()` function to parse the TGA file. Use the function
prefixed with `tga_get_*` to get image information. Don’t forget to release
`data` and `info` at the end:

```c
#include <stdint.h>
#include <stdio.h>

#include "tgafunc.h"

int main(int argc, char *argv[]) {
    const char *image_name = "images/sprite.tga";

    uint8_t *data;
    tga_info *info;
    enum tga_error error_code;
    error_code = tga_load(&data, &info, image_name);
    if (error_code == TGA_NO_ERROR) {
        printf("Width: %d, ", tga_get_image_width(info));
        printf("Height: %d\n", tga_get_image_height(info));
        printf("Pixel format: %d\n", tga_get_pixel_format(info));
        printf("Pixel size: %d\n", tga_get_bytes_per_pixel(info));

        tga_free_data(data);
        tga_free_info(info);
    }

    return 0;
}

```

You can use the `tga_create()` function to create an image. You can directly
manipulate image data, e.g. use standard functions to assign values to the
entire image data. Or use `tga_get_pixel()` function to read and write a pixel.
Finally use the `tga_save_from_info()` function to save as a TGA file (Don't
forget to release `data` and `info` as well):

```c
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "tgafunc.h"

int main(int argc, char *argv[]) {
    int width = 128, height = 128;

    uint8_t *data;
    tga_info *info;
    enum tga_error error_code;
    // Creates a 128 x 128 black image in RGB24 pixel format.
    error_code = tga_create(&data, &info, width, height, TGA_PIXEL_RGB24);
    if (error_code == TGA_NO_ERROR) {
        // Set the entire image to white.
        uint8_t pixel_size = tga_get_bytes_per_pixel(info);
        size_t data_size = (size_t)width * height * pixel_size;
        memset(data, 0xFF, data_size);
        // Sets the color of the left half of the image to magenta.
        uint8_t *pixel;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width / 2; j++) {
                pixel = tga_get_pixel(data, info, j, i);
                pixel[0] = 0xFF; // Blue channel.
                pixel[1] = 0x00; // Green channel.
                pixel[2] = 0xFF; // Red channel.
            }
        }
        // Saves the image as a TGA file.
        error_code = tga_save_from_info(data, info, "magenta_white.tga");
        if (error_code == TGA_NO_ERROR) {
            printf("Image saved successfully\n");
        } else {
            printf("Image save failed\n");
        }

        tga_free_data(data);
        tga_free_info(info);
    }

    return 0;
}

```

## License

Licensed under the [MIT](LICENSE) license.

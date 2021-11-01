#define TEST_NO_MAIN

#include "tinyobj_api_tests.h"
#include "tinyobj_loader_c.h"
#include "acutest.h"

void test_tinyobj_attrib_init(void)
{
    tinyobj_attrib_t attrib;
    tinyobj_attrib_init(&attrib);

    TEST_CHECK(attrib.vertices == NULL);
    TEST_CHECK(attrib.num_vertices == 0);
    TEST_CHECK(attrib.normals == NULL);
    TEST_CHECK(attrib.num_normals == 0);
    TEST_CHECK(attrib.texcoords == NULL);
    TEST_CHECK(attrib.num_texcoords == 0);
    TEST_CHECK(attrib.faces == NULL);
    TEST_CHECK(attrib.num_faces == 0);
    TEST_CHECK(attrib.face_num_verts == NULL);
    TEST_CHECK(attrib.num_face_num_verts == 0);
    TEST_CHECK(attrib.material_ids == NULL);
}

void loadFile(void *ctx, const char * filename, const int is_mtl, const char *obj_filename, char ** buffer, size_t * len)
{
    long string_size = 0, read_size = 0;
    FILE * handler = fopen(filename, "r");

    if (handler) {
        fseek(handler, 0, SEEK_END);
        string_size = ftell(handler);
        rewind(handler);
        *buffer = (char *) malloc(sizeof(char) * (string_size + 1));
        read_size = fread(*buffer, sizeof(char), (size_t) string_size, handler);
        (*buffer)[string_size] = '\0';
        if (string_size != read_size) {
            free(buffer);
            *buffer = NULL;
        }
        fclose(handler);
    }

    *len = read_size;
}

void test_tinyobj_parse_mtl_file(void)
{
    {
        const char * filename = "fixtures/cube.mtl";

        tinyobj_material_t * material;
        size_t num_materials;
        const char *search_path = NULL;

        TEST_CHECK(tinyobj_parse_mtl_file(&material, &num_materials, filename, /* obj filename */NULL, loadFile, NULL) == TINYOBJ_SUCCESS);

        TEST_CHECK(num_materials == 1);
        TEST_CHECK(strcmp(material->name, "CubeMaterial") == 0);

        TEST_CHECK(material->diffuse[0] == 1.0);
        TEST_CHECK(material->diffuse[1] == 0.0);
        TEST_CHECK(material->diffuse[2] == 0.0);

        TEST_CHECK(material->specular[0] == 0.5);
        TEST_CHECK(material->specular[1] == 0.25);
        TEST_CHECK(material->specular[2] == 0.125);

        TEST_CHECK(material->ambient[0] == 1.0);
        TEST_CHECK(material->ambient[1] == 1.0);
        TEST_CHECK(material->ambient[2] == 1.0);

        TEST_CHECK(material->emission[0] == 0.0);
        TEST_CHECK(material->emission[1] == 1.0);
        TEST_CHECK(material->emission[2] == 0.0);

        TEST_CHECK(material->illum == 2);
        TEST_CHECK(material->dissolve == 1.0);
    }
}

void test_tinyobj_parse_obj(void)
{
    const char * filename = "fixtures/cube.obj";
    const char * search_path = NULL;

    tinyobj_shape_t * shape = NULL;
    tinyobj_material_t * material = NULL;
    tinyobj_attrib_t attrib;

    unsigned long num_shapes;
    unsigned long num_materials;

    tinyobj_attrib_init(&attrib);

    int result = tinyobj_parse_obj(&attrib, &shape, &num_shapes, &material, &num_materials, filename, loadFile, NULL, TINYOBJ_FLAG_TRIANGULATE);

    TEST_CHECK(result == TINYOBJ_SUCCESS);

    // TODO: should these two values be swapped?
    TEST_CHECK(attrib.num_faces == 36);
    TEST_CHECK(attrib.num_face_num_verts == 12);

    TEST_CHECK(attrib.num_normals == 6);
    TEST_CHECK(attrib.num_vertices == 8);
    TEST_CHECK(attrib.num_texcoords == 0);

    tinyobj_attrib_free(&attrib);
    if (shape) {
        tinyobj_shapes_free(shape, num_shapes);
    }
    if (material) {
        tinyobj_materials_free(material, num_materials);
    }
}

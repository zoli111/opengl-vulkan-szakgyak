#include "texture.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>

unsigned int load_texture(const char* texturePath, int width, int height, int channels, int wrap, int filter) {
    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // hogyan kezelje a szeleket, pl: GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

    // nagyito es kicsinyito filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    stbi_set_flip_vertically_on_load_thread(true);
    unsigned char* data = stbi_load(texturePath, &width, &height, &channels, channels);

    if (data) {
        if (channels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,  data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else if (channels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,  data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "wrong texture channel count?: " << texturePath << std::endl;
        }
    } else {
        std::cout << "failed to load texture: " << texturePath << std::endl;
    }
    stbi_image_free(data);
    return id;
}

#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER
#include <glad/glad.h>

class Texture {
    public:
    Texture() = default;
    ~Texture();

    void load(const char* file);

    inline void bind() {
        glBindTexture(GL_TEXTURE_2D, ID);
    }
    
    private:
    GLuint ID;
};
#endif

#ifndef __GALAXY_FORT_SHADER_H
#define __GALAXY_FORT_SHADER_H

#include <vector>
#include <SDL_opengl.h>

class Sprite;
class SceneObject;

enum DrawMode {
    GF_BOX,
    GF_LINE,
    GF_FILL
};

class Shader {
    private:
        char* vertSource;
        char* fragSource;

        unsigned int numTextures;

        GLuint programID;
        GLuint vertPosBuffer;

        DrawMode currDrawMode;

        // A list might be faster
        std::vector<SceneObject*> sceneObjects;

    public:
        // This will compile a default shader
        Shader();
        Shader(const char* vertSource, const char* fragSource, unsigned int numTextures);
        ~Shader();

        void draw(Sprite& spr, int x, int y);
        void use(SceneObject& so);

        void setDrawMode(DrawMode mode);

        GLuint getProgramID();
};

#endif

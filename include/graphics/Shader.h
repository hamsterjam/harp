#ifndef __GALAXY_FORT_SHADER_H
#define __GALAXY_FORT_SHADER_H

#include <vector>
#include <SDL_opengl.h>

class Sprite;
class SceneObject;

enum DrawMode {
    RECT,
    RECT_FILL,
    TRIANGLE,
    TRIANGLE_FILL,
    LINE
};

class Shader {
    private:
        char* vertSource;
        char* fragSource;

        unsigned int numTextures;

        GLuint programID;
        GLuint vertPosBuffer;

        DrawMode currDrawMode;
        GLfloat  lineWidth;

        // A list might be faster
        std::vector<SceneObject*> sceneObjects;

        // Batched drawing
        // This is the ugliest way to fix this ever
        struct Pos {
            int x;
            int y;
        };
        std::vector<Sprite*> batchSprites;
        std::vector<Pos>  batchPositions;

        void drawUsingCurrentBuffers();
        void setBufferToRect(float x, float y, float w, float h);

    public:
        // This will compile a default shader
        Shader();
        Shader(const char* vertSource, const char* fragSource, unsigned int numTextures);
        ~Shader();

        void drawSprite(Sprite& spr, float x, float y);
        void drawRect(float x, float y, float w, float h);
        void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);

        void batchQueue(Sprite& spr, int x, int y);
        void batchDraw();

        void use(SceneObject& so);

        void setDrawMode(DrawMode mode);
        void setLineWidth(float width);

        GLuint getProgramID();
};

#endif

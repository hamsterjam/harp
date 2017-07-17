/*
 * HARP
 * graphics/Shader.h
 *
 * This is a wrapper on an OpenGL shader program. For the various draw
 * functions to work correctly a few uniforms and attributes are required in
 * your shader program sources. They are as follows:
 *
 * Vertex Shader:
 *     attribute vec2 aVertPos
 *     attribute vec2 aTexCoord
 *
 * Fragment Shader:
 *     uniform sampler2D uTexture
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

#ifndef HARP_SHADER_H
#define HARP_SHADER_H

#include <vector>
#include <SDL_opengl.h>

#include <harpMath.h>

class Sprite;
class SceneObject;

class Shader {
    private:
        enum DrawMode {
            RECT    = 0x01,
            TRI     = 0x02,
            LINE    = 0x03,
            POINT   = 0x04,

            // Options
            FILL    = 0x00,
            OUTLINE = 0x08
        };

        char* vertSource;
        char* fragSource;

        GLuint programID;
        GLuint vertPosBuffer;

        GLfloat  lineWidth;

        // A list might be faster
        std::vector<SceneObject*> sceneObjects;

        // Batched drawing
        std::vector<Sprite*> batchSprites;
        std::vector<Vec<2, float> >  batchPositions;

        void bindTextures(Sprite& spr);
        void draw(DrawMode mode, unsigned int numElements);

    public:
        // This will compile a default shader
        Shader(const char* vertSource, const char* fragSource);
        ~Shader();

        void drawSprite(Sprite& spr, float x, float y);
        void drawRectangle(float x, float y, float w, float h);
        void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
        void drawLine(float x1, float y1, float x2, float y2);
        void drawPoint(float x, float y);

        void batchQueue(Sprite& spr, float x, float y);
        void batchDraw();

        void use(SceneObject& so);

        // LineWidth = 0 means fill
        void setLineWidth(float width);

        GLuint getProgramID();
};

#endif

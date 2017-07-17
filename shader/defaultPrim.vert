attribute vec2 aVertPos;

void main(void) {
    gl_Position = vec4(aVertPos, 0.0, 1.0);
}

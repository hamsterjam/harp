#version 130
#extension GL_ARB_uniform_buffer_object : enable

#ifdef GL_ES
precision mediump float;
#endif

uniform allPrims {
    uint uShape;
    vec4 uColor;
};

uniform elipse {
    vec2 uCenter;
    float uRadiusX;
    float uRadiusY;
    float uTheta1;
    float uTheta2;
    float uLineWidthE;
};

uniform roundRect {
    vec2 uRoundPoint1;
    vec2 uRoundPoint2;
    float uRadius;
    float uLineWidthRR;
};

const uint OTHER = 0u;
const uint ELIPSE = 1u;
const uint ROUND_RECT = 2u;

void main(void) {
    switch (uShape) {
        case OTHER:
            gl_FragColor = uColor;
            break;
        case ELIPSE:
            vec2 trueRadial = gl_FragCoord.xy - uCenter;
            vec2 outerRadial = trueRadial / vec2(uRadiusX, uRadiusY);
            vec2 innerRadial = trueRadial / vec2(uRadiusX - uLineWidthE, uRadiusY - uLineWidthE);

            float angle = degrees(atan(trueRadial.y, trueRadial.x));
            angle = mod(angle, 360);

            bool rTest1 = dot(outerRadial, outerRadial) <= 1;
            bool rTest2 = dot(innerRadial, innerRadial) >= 1 || uLineWidthE == 0;
            bool aTest = angle >= uTheta1 && angle <= uTheta2;

            if (rTest1 && rTest2 && aTest) {
                gl_FragColor = uColor;
                break;
            }
            gl_FragColor = vec4(0.0);
            break;
        case ROUND_RECT:
            vec2 pos = gl_FragCoord.xy;

            vec2 radialNE = pos - uRoundPoint2;
            vec2 radialSE = pos - vec2(uRoundPoint2.x, uRoundPoint1.y);
            vec2 radialSW = pos - uRoundPoint1;
            vec2 radialNW = pos - vec2(uRoundPoint1.x, uRoundPoint2.y);

            float rNE = length(radialNE);
            float rSE = length(radialSE);
            float rSW = length(radialSW);
            float rNW = length(radialNW);

            bool isNE = dot(radialNE, vec2( 1.0,  1.0)) > rNE;
            bool isSE = dot(radialSE, vec2( 1.0, -1.0)) > rSE;
            bool isSW = dot(radialSW, vec2(-1.0, -1.0)) > rSW;
            bool isNW = dot(radialNW, vec2(-1.0,  1.0)) > rNW;

            if (isNE || isSE || isSW || isNW) {
                float r = min(min(min(rNE, rSE), rSW), rNW);
                bool rTest1 = r <= uRadius;
                bool rTest2 = r >= uRadius - uLineWidthRR || uLineWidthRR == 0;
                if (rTest1 && rTest2) {
                    gl_FragColor = uColor;
                    break;
                }
                gl_FragColor = vec4(0.0);
                break;
            }

            bool rTestN1 = pos.y >= uRoundPoint2.y + uRadius - uLineWidthRR;
            bool rTestS1 = pos.y <= uRoundPoint1.y - uRadius + uLineWidthRR;
            bool rTestE1 = pos.x >= uRoundPoint2.x + uRadius - uLineWidthRR;
            bool rTestW1 = pos.x <= uRoundPoint1.x - uRadius + uLineWidthRR;

            bool rTestN2 = pos.y <= uRoundPoint2.y + uRadius;
            bool rTestS2 = pos.y >= uRoundPoint1.y - uRadius;
            bool rTestE2 = pos.x <= uRoundPoint2.x + uRadius;
            bool rTestW2 = pos.x >= uRoundPoint1.x - uRadius;

            bool rTestInner = rTestN1 || rTestS1 || rTestE1 || rTestW1;
            bool rTestOuter = rTestN2 && rTestS2 && rTestE2 && rTestW2;

            if ((rTestOuter && rTestInner) || uLineWidthRR == 0) {
                gl_FragColor = uColor;
                break;
            }
            gl_FragColor = vec4(0.0);

            break;
        default:
            gl_FragColor = vec4(0.0);
            break;
    }
}

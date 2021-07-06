#include "sizeRecorder.h"

static SizeRecorder* sizeRecorder;


SizeRecorder* SizeRecorder::getInstance()
{   
    if (!sizeRecorder) {
        sizeRecorder = new SizeRecorder();
    }
    return sizeRecorder;
}

void SizeRecorder::record()
{
    return;
    glRotated(10,1,0,0);
    float mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    Mat4f mat4;
    mat4.getGLMatrix(mat);
    Mat4f unit = {  1,0,0,0,
                    1,0,0,0,
                    1,0,0,0,
                    1,0,0,0
        };

    mat4 = mat4 * unit;

    maxCoord[0] = max(maxCoord[0], mat4[0][0]);
    maxCoord[1] = max(maxCoord[1], mat4[1][0]);
    maxCoord[2] = max(maxCoord[2], mat4[2][0]);

    minCoord[0] = min(minCoord[0], mat4[0][0]);
    minCoord[1] = min(minCoord[1], mat4[1][0]);
    minCoord[2] = min(minCoord[2], mat4[2][0]);
    
}

SizeRecorder::SizeRecorder() {

}
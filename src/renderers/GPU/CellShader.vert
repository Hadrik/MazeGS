#version 330

layout (location = 0) in vec2 aVertPosition;
layout (location = 1) in vec2 aTexCoords;

layout (location = 2) in vec2 aVertOffset;
layout (location = 3) in vec2 aCellScale;

layout (location = 4) in vec3 aColor;
layout (location = 5) in float aSpriteRotation;
layout (location = 6) in int aSpriteOffset;

out vec4 bgColor;
flat out int spriteOffset;
out float spriteRotation;
out vec2 texCoords;

void main() {
    gl_Position = vec4(aVertPosition * aCellScale + aVertOffset, 0.0, 1.0);

    bgColor = vec4(aColor, 1.0);
    spriteOffset = aSpriteOffset;
    spriteRotation = aSpriteRotation;
    texCoords = aTexCoords;
}
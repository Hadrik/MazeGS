#version 330

in vec4 bgColor;
flat in int spriteOffset;
in float spriteRotation;
in vec2 texCoords;

out vec4 FragColor;

uniform sampler2DArray spriteSheet;

// https://gist.github.com/ayamflow/c06bc0c8a64f985dd431bd0ac5b557cd
vec2 rotateUV(vec2 uv, float rotation) {
    float cosAngle = cos(rotation);
    float sinAngle = sin(rotation);
    vec2 p = uv - vec2(0.5);
    return vec2(
    cosAngle * p.x + sinAngle * p.y + 0.5,
    cosAngle * p.y - sinAngle * p.x + 0.5
    );
}

void main() {
    if (spriteOffset >= 0) {
        vec4 spriteColor = texture(spriteSheet, vec3(rotateUV(texCoords, spriteRotation), spriteOffset));
        if (spriteColor.a > 0.0) {
            FragColor = mix(bgColor, spriteColor, spriteColor.a);
            return;
        }
    }
    FragColor = bgColor;
}
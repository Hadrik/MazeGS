#version 330

layout (location = 0) in vec2 aVertPosition;

layout (location = 1) in vec2 aStartPoint;
layout (location = 2) in vec2 aEndPoint;
layout (location = 3) in float aThickness;
layout (location = 4) in vec3 aColor;

out vec3 wallColor;

void main() {
    wallColor = aColor;

    vec2 wallDirection = aEndPoint - aStartPoint;
    float wallLength = length(wallDirection);
    vec2 dir = wallDirection / wallLength;

    vec2 normal = vec2(-dir.y, dir.x);

    vec2 scaledPos = dir * (aVertPosition.x * wallLength) + normal * (aVertPosition.y * aThickness);

    vec2 center = (aStartPoint + aEndPoint) / 2.0;
    gl_Position = vec4(center + scaledPos, 0.0, 1.0);
}
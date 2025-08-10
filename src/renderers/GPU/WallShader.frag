#version 330

in vec3 wallColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(wallColor, 1.0);
}
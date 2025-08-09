#version 330

layout (location = 0) in vec3 vertexPosition;     // Vertex input attribute: position
//in vec2 vertexTexCoord;     // Vertex input attribute: texture coordinate
//in vec4 vertexColor;        // Vertex input attribute: color
//out vec2 fragTexCoord;      // To-fragment attribute: texture coordinate
out vec4 fragColor;         // To-fragment attribute: color
//uniform mat4 mvp;           // Model-View-Projection matrix

void main() {
    gl_Position = vec4(vertexPosition, 1.0);

    fragColor = vec4(1.0, 0.0, 0.1, 1.0);
}
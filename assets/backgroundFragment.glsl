#version 330 core
in vec2 texCoord;

uniform sampler2D tex;

out vec4 pixelColor;

void main() {
    pixelColor = texture(tex, texCoord);
    //pixelColor = vec4(texCoord, 0.0f, 1.0f);
}

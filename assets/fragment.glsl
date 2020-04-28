#version 330 core
in vec2 texCoord;

uniform sampler2D tex;
uniform vec3 color;

out vec4 pixelColor;

void main() {
    vec4 pixel = texture(tex, texCoord);
    pixelColor = vec4((1.0f - pixel.w) * color, 1.0f);
}

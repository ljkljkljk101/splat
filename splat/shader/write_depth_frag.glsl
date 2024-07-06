#version 330 core
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec2 texcoord = vec2(gl_FragCoord.x / 1200, gl_FragCoord.y / 670);
    vec4 dstColor = texture(screenTexture, texcoord);
    if (dstColor.a <= 0.999) {
        discard;
    }
}
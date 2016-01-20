#version 330 core

in vec3 ourColor;
in vec2 fTexCoord;

uniform sampler2D ourTexture;

out vec4 color;

void main(){
	color = texture(ourTexture, fTexCoord);
}
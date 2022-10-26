#version 410 core

in vec4 color;              //color received from the pipeline (and vertex shader)
out vec4 fragmentColor;     //color sent to fragment on screen

void main() {

   fragmentColor = color;   //color given to current fragment (pixel)
}

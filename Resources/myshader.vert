/* 
  myshader.vert
  MyCppGame

  Created by guanghui on 4/8/14.

*/


attribute vec4 Position; // 1
attribute vec4 SourceColor; // 2
attribute vec2 TextureCoord;

varying vec4 DestinationColor; // 3
// Add right before the main
uniform mat4 Projection;
// Add right after the Projection uniform
uniform mat4 Modelview;

varying vec2 v_texCoord;


void main(void) { // 4
    DestinationColor = SourceColor; // 5
    v_texCoord = TextureCoord;
    gl_Position = CC_PMatrix * Modelview * Position; // 6
}
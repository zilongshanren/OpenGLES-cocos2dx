/* 
  myshader.vert
  MyCppGame

  Created by guanghui on 4/8/14.

*/


attribute vec4 a_position;
attribute vec4 a_color;

uniform mat4 modelView;
uniform mat4 projection;
varying vec4 fragmentColor;

void main()
{
//    gl_Position = CC_PMatrix * modelView * a_position;
    gl_Position = projection * modelView * a_position;
    fragmentColor = a_color;
}
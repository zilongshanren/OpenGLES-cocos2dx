/* 
  myshader.frag
  MyCppGame

  Created by guanghui on 4/8/14.

*/

varying lowp vec4 DestinationColor; // 1

uniform sampler2D CC_Texture0;
varying vec2 v_texCoord;

void main(void) { // 2
    gl_FragColor = DestinationColor * texture2D(CC_Texture0, v_texCoord); // 3
}
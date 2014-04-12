/* 
  myshader.frag
  MyCppGame

  Created by guanghui on 4/8/14.

*/

varying vec4 fragmentColor;

uniform sampler2D CC_Texture0;
varying vec2 v_texCoord;

void main()
{
    gl_FragColor = fragmentColor * texture2D(CC_Texture0, v_texCoord);
}
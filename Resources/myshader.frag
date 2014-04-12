/* 
  myshader.frag
  MyCppGame

  Created by guanghui on 4/8/14.

*/

varying vec4 fragmentColor;

uniform float width;
uniform float height;

uniform sampler2D CC_Texture0;
varying vec2 v_texCoord;

void main()
{
    float radius = 0.01;
    vec4 accum = vec4(0.0);
    vec4 normal = vec4(0.0);
    
    normal = texture2D(CC_Texture0, vec2(v_texCoord.x, v_texCoord.y));
    
    accum += texture2D(CC_Texture0, vec2(v_texCoord.x - radius, v_texCoord.y-radius));
    accum += texture2D(CC_Texture0, vec2(v_texCoord.x + radius, v_texCoord.y-radius));
    accum += texture2D(CC_Texture0, vec2(v_texCoord.x + radius, v_texCoord.y+radius));
    accum += texture2D(CC_Texture0, vec2(v_texCoord.x - radius, v_texCoord.y+radius));
    
    accum *= 1.75;
    
    accum.r = 1.0;
    accum.g = 0.2;
    accum.b = 0.5;
    
    normal = (accum * (1.0 - normal.a)) + (normal * normal.a);
    
    gl_FragColor = fragmentColor * normal;
}


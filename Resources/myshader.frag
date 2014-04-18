/* 
  myshader.frag
  MyCppGame

  Created by guanghui on 4/8/14.
http://www.idevgames.com/forums/thread-3010.html
*/


varying mediump vec3 EyespaceNormal;
varying lowp vec3 Diffuse;

uniform highp vec3 AmbientMaterial;  //is it a_color?
uniform highp vec3 SpecularMaterial;
uniform highp float Shininess;
uniform highp vec3 LightPosition;

uniform sampler2D Sampler;

varying mediump vec2 TextureCoordOut;

void main()
{
    highp vec3 N = normalize(EyespaceNormal);
    highp vec3 E = vec3(0,0,1);
    highp vec3 L = normalize(LightPosition);
    highp vec3 H = normalize(E + L);
    
    highp float df = max(0.0, dot(N,L));
    highp float sf = max(0.0, dot(N,H));
    sf = pow(sf, Shininess);
    
//    if (df < 0.1) df = 0.0;
//    else if (df < 0.3) df = 0.3;
//    else if (df < 0.6) df = 0.6;
//    else df = 1.0;
//    sf = step(0.5, sf);
    
    lowp vec3 color = AmbientMaterial + df * Diffuse + sf * SpecularMaterial;
    
    gl_FragColor = texture2D(Sampler, TextureCoordOut) * vec4(color,1.0) ;
}


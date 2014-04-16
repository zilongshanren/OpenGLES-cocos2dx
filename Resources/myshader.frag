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

void main()
{
    highp vec3 N = normalize(EyespaceNormal);
    highp vec3 E = vec3(0,0,1);
    highp vec3 L = normalize(LightPosition);
    highp vec3 H = normalize(E + L);
    
    highp float df = max(0.0, dot(N,L));
    highp float sf = max(0.0, dot(N,H));
    sf = pow(sf, Shininess);
    
    lowp vec3 color = AmbientMaterial + df * Diffuse + sf * SpecularMaterial;
    
    gl_FragColor = vec4(color, 1.0);
}


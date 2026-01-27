#version 330 core
layout (location = 0) in vec2 a_Pos;
        
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec4 u_UVrect;
        
out vec2 TexCoord;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 0.0, 1.0);
    TexCoord = mix(u_UVrect.xy, u_UVrect.zw, a_Pos);
}

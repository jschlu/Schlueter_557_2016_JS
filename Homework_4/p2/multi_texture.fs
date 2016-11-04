#version 410 core                                                 

uniform sampler2D texture_background; //this is the texture
uniform sampler2D texture_foreground; //this is the texture
uniform sampler2D texture_gradient;

in vec2 pass_TexCoord; //this is the texture coord
in vec4 pass_Color;
out vec4 color;

uniform int texture_blend;

void main(void)                                                   
{
    // This function finds the color component for each texture coordinate. 
    vec4 tex_color_1 =  texture(texture_background, pass_TexCoord);
    
    vec4 tex_color_2 =  texture(texture_foreground, pass_TexCoord);
	
	vec4 tex_color_gradient = texture(texture_gradient, pass_TexCoord);
    
    // This mixes the background color with the texture color.
    // The GLSL shader code replaces the former envrionment. It is now up to us
    // to figure out how we like to blend a texture with the background color.
    if(texture_blend == 0)
    {
        color = (tex_color_gradient + tex_color_1)/2.0 + tex_color_2;
    }
    else if(texture_blend == 1)
    {
        color = tex_color_gradient.r * ((tex_color_1 + tex_color_2)/2.0f);
    }
    else if(texture_blend == 2)
    {
        color = tex_color_2*tex_color_1*tex_color_gradient;
    }
    else
    {
        color = 0.1 * pass_Color + tex_color_1;
    }
    
}
#version 330 core

out vec4 out_color;

in vec2 texCoord;

// Thick lines 
float grid(vec2 fragCoord, float space, float gridWidth)
{
    vec2 p  = fragCoord - vec2(.5);
    vec2 size = vec2(gridWidth);
    
    vec2 a1 = mod(p - size, space);
    vec2 a2 = mod(p + size, space);
    vec2 a = a2 - a1;
       
    float g = min(a.x, a.y);
    return clamp(g, 0., 1.0);
}


void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = texCoord/iResolution.xy;

    // Pixel color
    vec3 col = vec3(.7,.7,.7);
    
    // Gradient across screen
    vec2 p = texCoord.xy;           // Point
	vec2 c = iResolution.xy / 2.0;   // Center
    col *= (1.0 - length(c - p)/iResolution.x*0.5);
	
    // 2-size grid
    col *= clamp(grid(texCoord, 10., 1.) *  grid(texCoord, 50., 1.5), grid_intensity, 1.0);
    
    // Output to screen
    out_color = vec4(col,1.0);
}
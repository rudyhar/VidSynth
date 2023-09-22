#version 330 core

precision highp float;

uniform sampler2D tex0;
uniform float sx;
uniform float nano1;

in vec2 texCoordVarying;
out vec4 FragColor;
 
vec3 rgb2hsb(in vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

//if we want to work with hsb space in shaders we have to
//convert the rgba color into an hsb, do some hsb stuffs
//and then convert back into rgb for the final draw to the screen
vec3 hsb2rgb(in vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
	//vec4 color = texture2D(tex0, texCoordVarying);
	
	float colorx=texCoordVarying.x;
	float colory=texCoordVarying.y;
	
	//within the vec4 the components here are
	//vec4(RED,GREEN,BLUE,ALPHA)
	//and we can ignore alpha but make sure it
	//is always at 1.0 or it will be confusing
	//vec4 color= vec4(colorx,sx,colory,1.0);
	vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);

	vec4 color = mix(red, blue, texCoordVarying.x);

	
	//remove the comment thingies to activate the
	//hsb section below
	
	//vec3 colorHsb=rgb2hsb(color.rgb);
	
	//within the vec3 the components are
	//vec3(HUE, SATURATION,BRIGHTNESS)
	
	//colorHsb= vec3(.5,.3,1.0);
	
	//color.rgb=hsb2rgb(colorHsb);

	FragColor = color;
}

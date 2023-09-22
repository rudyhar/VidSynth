#version 330 core

precision highp float;

uniform sampler2DRect video_feed;
uniform sampler2DRect framebufferdelay;

uniform vec2 u_resolution;
uniform float mixx;

uniform float lumavalue;

uniform float lumathreshold; // acts as a radius of tolerace around the value (midpoint) luma

uniform float keythreshold; // acts as a radius of tolerace around the value (midpoint) colour
uniform float keycolour;
uniform float camscaler;

// so inapp can make it red green or blue
// uniform vec3 keyChannel;

uniform float delayscale;

uniform float midi12;
uniform float midi13;
uniform float midi14;

uniform float time;

// RGB
uniform vec3 rgb_multiplier;

// HSB
uniform vec3 hsv_multiplier;

in vec2 texcoord1;

out vec4 FragColor;

vec2 coord;


vec3 col;

vec2 camScale = camscaler * gl_FragCoord.xy; // half the sample size of the pixel so each pixel gets sampled twice
// samples every half pixel of the texture effectibely doubling the resolution of the texture by sampling each pixel 4 times
// twice for x and twice for y

void kernel()
{
    const float offset = 1.0;  

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -2, -2, -2,
        -2,  16, -2,
        -2, -2, -2
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(video_feed, coord + offsets[i])); // sample the pixel of the texture at each offset and pass to sampleTex array
    }
    col = vec3(0.0);

    for(int i = 0; i < 9; i++){
        col += sampleTex[i] * kernel[i];
        // build uip a vec3 combining all sampled pixels multiplied by their kernel value
	}
}


vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


void main()
{
    vec2 norm_xy = vec2(gl_FragCoord.x / u_resolution.x, gl_FragCoord.y / u_resolution) ; 
    

	coord = gl_FragCoord.xy;

    
    // for upscalling
    coord = camScale;
    
	// vec4 invert = vec4(1 - color.x, 1 -color.y, 1 - color.z, 1);

	// vec3 fullscreen = mix(col, fbdelay.xyz, mixx);


    vec2 delay_coord = coord;
    delay_coord-= vec2(u_resolution.x / 2, u_resolution.y / 2);
    // delay_coord=delay_coord*(1.0);
    delay_coord+= vec2(u_resolution.x / 2, u_resolution.y / 2);
    
    //x and y offsets
    delay_coord.x+=midi13;
    delay_coord.y+=midi14;

    
	vec4 fbdelay = texture(framebufferdelay, delayscale*delay_coord);
	vec4 color = texture(video_feed, coord);
    
    
    // RGB MULTIPLIER
    fbdelay.xyz = vec3(fbdelay.x * rgb_multiplier.x, fbdelay.y * rgb_multiplier.y, fbdelay.z * rgb_multiplier.z);
    
    // HSB MODIFIERS
    vec3 fbdelayHSB = rgb2hsv(fbdelay.xyz);
    
    // apply sliders for hsb
    fbdelayHSB.xyz = vec3(fbdelayHSB.x * hsv_multiplier.x, fbdelayHSB.y * hsv_multiplier.y, fbdelayHSB.z * hsv_multiplier.z);
    
    
    // convert back to rbg
    fbdelay.xyz = hsv2rgb(fbdelayHSB.xyz);
    
    
    
    
    
    

	// kernel();
	// FragColo;


    // calculating the brightness value of the pixel based on the coefficinets of
    // brightness for rgb in human perception
    float bright=.33*color.r+.5*color.g+.16*color.b;
    
    vec3 keyChannel = vec3(1,0,0); // make into uniform

    // extract red from pixel ~
    vec3 channelValueVec =  color.rgb * keyChannel; // RED OF PIXEL
    float channelValue =  channelValueVec.r + channelValueVec.g + channelValueVec.b;

    // luma keyer
    if( (bright<lumavalue+lumathreshold)    &&  (bright>lumavalue-lumathreshold)    ){
        color=fbdelay;
//        color=vec4(0.0);
        
    }
    
    // rgb keyer
    if ( (channelValue<keycolour+keythreshold) && (channelValue>keycolour-keythreshold) ){
        // color=fbdelay;

        color=vec4(0.0);
    }
    
    FragColor = mix(fbdelay,color,mixx);


    // FragColor = vec4(fract(midi12 * norm_xy.x + time * 20),0.0,0.0,1.0);


    







}

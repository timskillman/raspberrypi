precision mediump float;       // Set the default precision to medium. We don't need as high of a

uniform mat4 u_ProjMatrix;     // view/projection matrix.
uniform mat4 u_ModelMatrix;    // model matrix.
uniform vec3 u_LightPos;       // The position of the light in eye space.

uniform vec2 u_animoffset;
uniform vec4 u_diffuseColour;
uniform vec4 u_emissiveColour;
uniform vec4 u_ambientColour;

uniform vec3 u_fogColour;
uniform float u_fogMaxDist;
uniform float u_fogRange;  	// effectively 1.0 / (fogMaxDist-fogMinDist)

attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_UV;
 
varying vec2 v_UV;
varying vec4 v_diffuseColour;
varying vec4 v_fogColour;
varying vec3 v_Normal;

void main()
{
    	// Transform position, lightPos and vertex normal into model space
    	vec3 Position = vec3(u_ModelMatrix * vec4(a_Position, 1.0));
    	vec3 LightPos = vec3(u_ModelMatrix * vec4(u_LightPos, 0.0));
    	v_Normal = normalize(vec3(u_ModelMatrix * vec4(a_Normal, 0.0)));

	// Calc UV with animation offset
	v_UV = vec2(a_UV.x, 1.0 - a_UV.y) + u_animoffset;

	// Calc fog
	float fogFactor = (u_fogMaxDist - Position.z) * u_fogRange;
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	if (u_fogRange==0.0) fogFactor = 1.0;
	v_fogColour = vec4(u_fogColour * (1.0 - fogFactor), 0);
	
	v_diffuseColour = vec4((u_diffuseColour * u_emissiveColour * fogFactor).rgb,u_diffuseColour.a); //preserve alpha
		
    	gl_Position = u_ProjMatrix * vec4(Position, 1.0);
}


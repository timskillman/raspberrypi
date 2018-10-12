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
uniform float u_fogMinDist;

attribute vec3 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_UV;
 
varying vec3 v_Position;       // This will be passed into the fragment shader.
varying vec3 v_LightPos;       // This will be passed into the fragment shader.
varying vec3 v_Normal;         // This will be passed into the fragment shader.
varying vec2 v_UV;
varying vec4 v_diffuseColour;  //
//varying float v_fogFactor;  //
varying vec4 v_fogColour;  //

// The entry point for our vertex shader.
void main()
{
    // Transform the vertex into Model space.
	//mat4 vmat = u_ModelMatrix; // * u_SceneMatrix; // * u_RotYMatrix * u_RotXMatrix;
    v_Position = vec3(u_ModelMatrix * vec4(a_Position, 1.0));
    v_LightPos = vec3(u_ModelMatrix * vec4(u_LightPos, 0.0));
	  
    // Transform the normal's orientation into eye space.
    v_Normal = normalize(vec3(u_ModelMatrix * vec4(a_Normal, 0.0)));

	v_UV = vec2(a_UV.x, 1.0 - a_UV.y) + u_animoffset;

	float fogFactor = (u_fogMaxDist - v_Position.z) / (u_fogMaxDist - u_fogMinDist); //calc fog
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	v_fogColour = vec4(u_fogColour * (1.0 - fogFactor), 1.0); 
	
	v_diffuseColour = u_diffuseColour * u_emissiveColour* fogFactor;
		
	// Multiply the vertex by the matrix to get the final position
    gl_Position = u_ProjMatrix * vec4(v_Position, 1.0);
}

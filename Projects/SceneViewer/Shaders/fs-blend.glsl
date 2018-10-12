precision mediump float;       	// Set the default precision to medium

uniform sampler2D u_Texture1;   // texture1 blends with
uniform sampler2D u_Texture2;   // texture2
uniform sampler2D u_BlendTex;   // blend texture texture
varying vec2 v_UV;				// Texture UV coordinate
varying vec4 v_diffuseColour;	// Diffuse colour
varying vec4 v_fogColour;		// Fog colour

void main()
{
		vec4 blend = texture2D(u_BlendTex, v_UV);
		gl_FragColor = (texture2D(u_Texture1, v_UV) * blend) + (texture2D(u_Texture2, v_UV) * (1.0 - blend)) * v_diffuseColour + v_fogColour;
}

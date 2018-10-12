precision mediump float;       	// Set the default precision to medium

uniform sampler2D u_Texture;   	// texture
varying vec2 v_UV;				// Texture UV coordinate
varying vec4 v_diffuseColour;	// Diffuse colour
varying vec4 v_fogColour;		// Fog colour

void main()
{
		gl_FragColor = texture2D(u_Texture, v_UV) * v_diffuseColour + v_fogColour;
}

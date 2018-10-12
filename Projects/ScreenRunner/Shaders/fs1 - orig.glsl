precision mediump float;       // Set the default precision to medium. We don't need as high of a
                               // precision in the fragment shader.
uniform sampler2D u_Texture;   // texture

uniform vec4 u_specColour;
uniform float u_specPower;
uniform int u_illum;

varying vec3 v_LightPos;       //
varying vec3 v_Position;       // Interpolated position for this fragment.
varying vec3 v_Normal;         // Interpolated normal for this fragment.
varying vec2 v_UV;			   // Texture coordinate
varying vec4 v_diffuseColour;  //
//varying float v_fogFactor;  //
varying vec4 v_fogColour;  //

// The entry point for our fragment shader.
void main()
{
	gl_FragColor = texture2D(u_Texture, v_UV) * v_diffuseColour + v_fogColour; //Read texture pixel
	
	//if (u_illum==2) {
	//	vec3 lightNormal = normalize(v_LightPos - v_Position);
	//	
	//	float rDotV = max(0.0, dot(lightNormal, v_Normal));
	//	vec4 specular = u_specColour * pow(rDotV, 120.0);
		
	//	gl_FragColor = vec4(gl_FragColor.rgb * max(dot(v_Normal, lightNormal), 0.1), gl_FragColor.a) + specular; // + u_ambientColour; //-diffuse; //Apply lambert shading
	//}
	
	//Apply fog ..
	//gl_FragColor = gl_FragColor * v_fogFactor + v_fogColour; //Apply fog
		
}

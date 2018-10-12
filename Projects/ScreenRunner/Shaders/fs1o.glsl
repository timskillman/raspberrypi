precision mediump float;       // Set the default precision to medium. We don't need as high of a
                               // precision in the fragment shader.
uniform vec3 u_LightPos;       // The position of the light in eye space.
uniform sampler2D u_Texture;   // texture
uniform float u_mixer;
uniform float u_illum;

uniform vec4 u_fogColour;
uniform float u_fogMaxDist;
uniform float u_fogMinDist;
uniform vec4 u_diffuseColour;
uniform vec4 u_specColour;
uniform vec2 u_animoffset;

varying vec3 v_Position;       // Interpolated position for this fragment.
varying vec3 v_Normal;         // Interpolated normal for this fragment.
varying vec2 v_UV;			   // Texture coordinate

void main()
{

	gl_FragColor = texture2D(u_Texture, v_UV + u_animoffset) * u_illum * u_diffuseColour; //Read texture pixel
	
	if (u_illum == 1.0) {
		float distance = length(u_LightPos - v_Position);
		vec3 lightDirNormal = normalize(u_LightPos);

		float rDotV = max(0.0, dot(lightDirNormal, v_Normal));
		vec4 specular = u_specColour * pow(rDotV, 80.0);
		
		gl_FragColor = vec4(gl_FragColor.rgb * max(dot(v_Normal, lightDirNormal), 0.1), gl_FragColor.a) + specular;
		
		//Apply fog ..
		float fogFactor = (u_fogMaxDist - distance) / (u_fogMaxDist - u_fogMinDist); //calc fog
		fogFactor = clamp(fogFactor, 0.0, 1.0);
		vec4 fogColour = fogFactor * u_fogColour;
		gl_FragColor = gl_FragColor * fogFactor + fogColour* (1.0 - fogFactor); //Apply fog
		
	}
		
}

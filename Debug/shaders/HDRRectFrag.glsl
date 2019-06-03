

uniform sampler2DRect s2Test;
uniform float fExposure;


void main (void)
{
	vec4 f4Color = texture2DRect(s2Test, gl_TexCoord[0].st);
	gl_FragColor = 1.0 - exp(f4Color * -fExposure);
}


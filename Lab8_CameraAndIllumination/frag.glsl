#version 330

// Take in our texture coordinate from our vertex shader
in vec2 texCoords;
// And our normal
in vec3 norm;
// And our fragment position for lighting
in vec3 fragPos;

// We always define a fragment color that we output.
out vec4 fragColor;

// Define our light(s)
struct PointLight {
    vec3 color;
    vec3 position;
    float ambientIntensity;
    float specularIntensity;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Maintain our uniforms.
uniform sampler2D tex;              // our primary texture
uniform mat4 view;                  // we need the view matrix for highlights
uniform PointLight pointLights[1];  // Our lights

void main() {
  // Set our output fragment color to whatever we pull from our input texture (Note, change 'tex' to whatever the sampler is named)
  vec3 red_light = vec3(1.0f, 0.0f, 0.0f);
  fragColor = texture(tex, texCoords);
  PointLight pL = pointLights[0];

  // Lighting logic pulled from Lecture 8
  vec3 ambient = pL.ambientIntensity * pL.color;
  vec3 normal = normalize(norm);

  vec3 lightDir = normalize(pL.position - fragPos);

  float diffImpact = max(dot(normal, lightDir), 0.0);
  vec3 diffuseLight = diffImpact * pL.color;

  vec3 Lighting = diffuseLight + ambient;

  fragColor = vec4(fragColor.rgb * Lighting * red_light, 1.0f);
  
}
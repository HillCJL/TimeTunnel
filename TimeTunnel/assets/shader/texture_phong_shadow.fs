#version 330 core
out vec4 FragColor;
in vec3 NVector;
in vec3 FragPos;
in vec4 FragPosLight;

uniform float ambient_strength;
uniform float diffuse_strength;
uniform float specular_strength;
uniform float n_shiny;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform vec3 light_color;
uniform vec3 object_color;

uniform sampler2D shadowMap;

uniform bool optimize;

float clamp(float i, float min, float max){
	return (i > max)?max:((i < min)?min:i);
}


void main(){
	vec3 ambient = ambient_strength * light_color;

	vec3 NV = normalize(NVector);
	vec3 light_direction = normalize(light_pos - FragPos);
	vec3 diffuse = diffuse_strength * max(0.0, dot(NV, light_direction)) * light_color;

    vec3 view_direction = normalize(view_pos - FragPos);
    vec3 reflect_direction = reflect(-light_direction, NV);  
    vec3 specular = specular_strength * pow(max(dot(view_direction, reflect_direction), 0.0), n_shiny) * light_color;  


	vec3 projCoords = FragPosLight.xyz / FragPosLight.w;
	projCoords = projCoords * 0.5 + 0.5;

	float shadow = 0.0f;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

	if(optimize){
	for(int x = -1; x <=1; x++){
		for(int y = -1; y <=1; y++){
			float closestDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			float currentDepth = projCoords.z;
			float bias = 0.0015 * tan(max(acos(dot(NV, light_direction)), 0));
			bias = clamp(bias, 0.001, 0.003);
			shadow += (currentDepth - bias > closestDepth)  ? 1.0 : 0.0;
		}
	}
		shadow /=9.0;
		if(projCoords.z > 1.0) shadow = 0.0;
	}else{
		float closestDepth = texture(shadowMap, projCoords.xy).r; 
		float currentDepth = projCoords.z;
		float bias = 0.005;
		shadow = (currentDepth - bias > closestDepth)  ? 1.0 : 0.0;
	}

	
	FragColor = vec4((ambient + (1 - shadow) * (diffuse + specular)) * object_color, 1.0);
};
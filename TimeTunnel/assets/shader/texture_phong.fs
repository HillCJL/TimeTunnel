#version 330 core
out vec4 FragColor;
in vec3 NVector;
in vec3 FragPos;

uniform float ambient_strength;
uniform float diffuse_strength;
uniform float specular_strength;
uniform float n_shiny;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform vec3 light_color;
uniform vec3 object_color;

void main(){
	vec3 ambient = ambient_strength * light_color;

	vec3 NV = normalize(NVector);
	vec3 light_direction = normalize(light_pos - FragPos);
	vec3 diffuse = diffuse_strength * max(0.0, dot(NV, light_direction)) * light_color;

    vec3 view_direction = normalize(view_pos - FragPos);
    vec3 reflect_direction = reflect(-light_direction, NV);  
    vec3 specular = specular_strength * pow(max(dot(view_direction, reflect_direction), 0.0), n_shiny) * light_color;  

	FragColor = vec4((ambient + diffuse + specular) * object_color, 1.0);
};
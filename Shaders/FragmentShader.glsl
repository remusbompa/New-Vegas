#version 330
 
uniform sampler2D textureStreet;
uniform sampler2D textureBuilding;

uniform int obj;

in vec2 texcoord;

layout(location = 0) out vec4 out_color;

in vec3 world_position;
in vec3 world_normal;
in vec3 my_color;

uniform vec3 light_direction;
uniform vec3 dirLight_position;
uniform vec3 spotLight_position[20];
uniform vec3 eye_position;
uniform int spot;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform float spot_angle;

uniform int nr_spots;

float rand (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

float drakerRandom(vec2 p){

	float random = rand(p);
	if(random <= 0.75){
		return 0;
	}else if(random <= 0.85){
		return 0.5;
	}else{
		return random;
	}
}

void main()
{
	// TODO : calculate the out_color using the texture2D() function
		vec2 txt = texcoord;
		if(obj == 0){
			out_color = texture2D(textureStreet, txt);
		}else if (obj == 1){
			vec2 position = floor(vec2(txt.x * 3, txt.y));
			float n = drakerRandom(position);
			out_color = vec4(vec3(n), 1.);
			
		}else if(obj == 3){
			txt = txt / 600.0f;
			txt = vec2(txt.x / 3.0f, txt.y / 15);
			out_color = texture2D(textureStreet, txt) ;
			return;
		}else{
			out_color = vec4(my_color, 1);
		}

	//light prelucration

	vec3 wp = world_position;
	// TODO: compute world space vectors
	vec3 N, V, L;
	N = normalize(world_normal);
	V =  normalize (vec3(eye_position - wp));

	float intensity1 = 0, intensity2 = 0;

	
	if(spot == 1){
		for(int i =0; i < nr_spots; i++){
			L = normalize (vec3(spotLight_position[i] - wp));

			float pi = 3.14159;
			float cut_off = pi * spot_angle / 180;
			float spot_light = dot(-L, light_direction);
			float spot_light_limit = cos(cut_off);
 
			float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
			float light_att_factor = pow(linear_att, 2);

			float diffuse_light = 1.0f * max (dot(N , normalize(L)), 0) * material_kd;


			float primeste_lumina = 0;
			float dt = dot(N, V);
			if (dt > 0)
				primeste_lumina = 1;
			vec3 H = normalize(normalize(L) + normalize(V));
			float specular_light = 0;
			if (diffuse_light > 0)
			{
				specular_light = primeste_lumina * pow(max(dot(V, reflect(-L, N)), 0), material_shininess) * material_ks;

			} else {
				diffuse_light = 0;
			}

			float d = distance(spotLight_position[i], wp);
			float factorAtenuare = min(1, 1.0f / (d * d));

			if (spot_light > cos(cut_off))
			{
				intensity1 +=  1800.0 * (diffuse_light + specular_light) * light_att_factor * factorAtenuare;
			}
		}
	}

	float material_ks2 = material_ks / 2.0f;
	float material_kd2 = material_kd / 2.0f;

	L = normalize (dirLight_position);

	float ambient_light = 0.25;

	float diffuse_light = 1.0f * max (dot(N , normalize(L)), 0) * material_kd;

	float primeste_lumina = 0;
	float dt = dot(N, V);
	if (dt > 0)
		primeste_lumina = 1;
	vec3 H = normalize(normalize(L) + normalize(V));
	float specular_light = 0;
	if (diffuse_light > 0)
	{
		specular_light = primeste_lumina * pow(max(dot(V, reflect(-L, N)), 0), material_shininess) * material_ks;

	} else {
		diffuse_light = 0;
	}

	float factorAtenuare = 1.0;
		
	intensity2 = ambient_light + (diffuse_light + specular_light) * factorAtenuare;

	vec3 color = out_color.xyz;
	float intensity = intensity1 + intensity2;
	out_color = vec4(intensity * color, 0);

}
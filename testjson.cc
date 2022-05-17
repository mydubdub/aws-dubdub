#include<stdio.h>
#include<json-c/json.h>


int main(int argc, char **argv) {
	FILE *fp;
	char buffer[1024];
	struct json_object *parsed_json;
	struct json_object *volt;
    struct json_object *volts;
	struct json_object *data;
	// struct json_object *friends;
	// struct json_object *friend;
	size_t n_data;

	size_t i;	

	fp = fopen("209-SVT.json","r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "209-SVT", &volt);
	// json_object_object_get_ex(parsed_json, "age", &age);
	// json_object_object_get_ex(parsed_json, "friends", &friends);

	// printf("Name: %s\n", json_object_get_string(name));
	// printf("209-SVT: %d\n", json_object_get_int(volt));

	n_data = json_object_array_length(volt);
	printf("Found %lu data\n",n_data);

	for(i=0;i<10;i++) {
		volts = json_object_array_get_idx(volt, i);
		printf("%lu. %i\n",i+1,json_object_get_int(volt));
	}	
}
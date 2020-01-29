#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

char *getuuid(){    
	static char str[36]; 
	uuid_t uuid;
	uuid_clear(uuid);
	uuid_generate_random(uuid);
	uuid_unparse(uuid,str); 
	return str; 
}

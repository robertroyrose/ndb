#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <cjson/cJSON.h>

extern struct ndb_message _Root_message;

#include "spwr.h"

#if 0
struct ndb_members {
	char *name;
	size_t off;
	size_t tag;
};

struct ndb_message {
	struct ndb_members *nm_members;
	size_t nm_members_cnt;
	size_t nm_message_size;
};
#endif

struct ndb_message _string_message = {
	0,0,0
};


void json_dump(struct ndb_message *p, int ident, void *m)
{
	int i;

	for (i = 0; i < p->nm_members_cnt; i++) {
		if (p->nm_members[i].type != &_string_message) {
			printf("\"%s\": {\n", p->nm_members[i].name);
			json_dump(p->nm_members[i].type, ident + 1, (char *)m + p->nm_members[i].off);
			printf("}\n");
		} else if (p->nm_members[i].type == &_string_message) {
			char *s = *(char **)((char *)m + p->nm_members[i].off);
			if (s)
				printf("\"%s\" : \"%s\"\n", p->nm_members[i].name, s);
			else
				printf("\"%s\" : null\n", p->nm_members[i].name);
		}
		if (i !=p->nm_members_cnt -1) printf(",\n"); 
	}
}

void printMem(struct ndb_message *p, int ident, void *m)
{
	int i;

	for (i = 0; i < p->nm_members_cnt; i++) {
		printf("%d %s\n", ident, p->nm_members[i].name);
		if (p->nm_members[i].type != &_string_message)
			printMem(p->nm_members[i].type, ident + 1, (char *)m + p->nm_members[i].off);
		else if (p->nm_members[i].type == &_string_message) {
			char *s = *(char **)((char *)m + p->nm_members[i].off);
			printf("%p %s\n", s, s ?s: "null");
		}
	}
}

Root root;

void printPath(struct ndb_message *p, void *m, char *argv[])
{
	int i;

	if (argv[0] == NULL)
		return;
	//printf("%s\n", p->nm_members[i].name);
	for (i = 0; i < p->nm_members_cnt; i++) {
		if (strcmp(argv[0], p->nm_members[i].name) == 0) {
			printf("/%d-%s", i, p->nm_members[i].name);
			printPath(p->nm_members[i].type, (char *)m + p->nm_members[i].off, argv + 1);
		}

	}
}

cJSON *loadfile(const char *filename)
{
    struct stat stat;
    int message_length;
    int f;
    int status;
    void *buffer = NULL;

    if ((f = open(filename, 0)) < 0) {
        perror(filename);
        return NULL;
    }

    if (fstat(f, &stat) != 0) {
        close(f);
        return NULL;
    }
    if ((buffer = malloc(stat.st_size)) == NULL) {
        close(f);
        return NULL;
    }
    message_length = read(f, buffer, stat.st_size);
    close(f);
    if (message_length != stat.st_size) {
        free(buffer);
        return NULL;
    }

    cJSON *json = cJSON_ParseWithLength(buffer, message_length);

    printf("\n%s\n", json->child->valuestring);
    //printf("%s\n", cJSON_Print(json));
    //printf("%s\n", cJSON_Print(json->child));

    return json;
}

int main(int argc, char *argv[])
{
	cJSON *p;
	char *ar[] = { "config", "system", "serialNum", 0};
	memset(&root, 0, sizeof(root));
	root.config.system.lmac = "crap";
	printMem(&_Root_message, 1, (void *)&root);
	json_dump(&_Root_message, 1, (void *)&root);
	printPath(&_Root_message, (void *)&root, ar);

	p = loadfile("./mqtt_certs.txt");
	return 0;
}

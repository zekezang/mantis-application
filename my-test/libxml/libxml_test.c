#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "list.h"
#include <string.h>

struct media_type {
	char tag[16];
	char type[512];
	int value;
	int support;
	struct list_head list;
};

static struct media_type mytype_list;

int main(int argc, char **argv) {
	xmlDocPtr pdoc = NULL;
	xmlNodePtr proot = NULL, curNode = NULL;
	char *psfilename;

	if (argc < 1) {
		exit(1);
	}
	psfilename = argv[1];
	pdoc = xmlReadFile("mt.xml", "UTF-8", XML_PARSE_RECOVER);
	if (pdoc == NULL) {
		exit(1);
	}

	proot = xmlDocGetRootElement(pdoc);
	if (proot == NULL) {
		exit(1);
	}

	curNode = proot->xmlChildrenNode;
	xmlChar *key;
	char n = 0;

	INIT_LIST_HEAD(&mytype_list.list);
	struct media_type* s_node;
	char* tmp_type;

	while (curNode != NULL) {
		if ((!xmlStrcmp(curNode->name, (const xmlChar *) "item"))) {

			s_node = (struct media_type*) calloc(1,sizeof(struct media_type));

			key = xmlGetProp(curNode, (const xmlChar *) "tag");
			memcpy(s_node->tag,key,strlen(key));
			xmlFree(key);

			key = xmlGetProp(curNode, (const xmlChar *) "type");
			memcpy(s_node->type,key,strlen(key));
			xmlFree(key);

			key = xmlGetProp(curNode, (const xmlChar *) "value");
			s_node->value = atoi(key);
			xmlFree(key);

			key = xmlGetProp(curNode, (const xmlChar *) "support");
			s_node->support = atoi(key);
			xmlFree(key);

			list_add(&(s_node->list), &(mytype_list.list));
		}
		curNode = curNode->next;
	}
	xmlFreeDoc(pdoc);
	xmlCleanupParser();

	list_for_each_entry(s_node, &mytype_list.list, list) {
		printf("tag:%s  type:%s  value:%d  support:%d\n",s_node->tag, s_node->type, s_node->value, s_node->support);
	}


	return 0;
}

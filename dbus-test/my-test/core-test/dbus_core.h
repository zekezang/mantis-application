#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>

#define METHOD_LEN 2


struct thread_param {
	char *thread_name;
	char *path;
	char *type;
	char *interface;
	char *method;
	char *sigvalue;
};

void receive_singal(DBusConnection* conn, char *interface, char *method, void (deal_method)(char *));

DBusConnection *dbus_get_connection();

void set_connection_name(DBusConnection* conn, char *name);

int add_match(DBusConnection* conn, char *type, char *interface);

DBusMessage *create_new_msg_singal(char *path, char *iterface, char *method);

void add_args_tomsg(DBusMessage *msg, void *p);

void send_msg(DBusConnection* conn, DBusMessage *msg);

void receive_exit(char *sigvalue);

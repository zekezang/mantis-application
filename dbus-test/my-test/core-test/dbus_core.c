
#include "dbus_core.h"

int run = 1;

DBusConnection *conn;
DBusMessage *msg;

DBusError err;
int ret = 0;

char* sigvalue;
dbus_uint32_t serial = 0;

void receive_exit(char *sigvalue) {
	if (strcmp(sigvalue, "exit") == 0) {
		run = 0;
	}
}

void receive_singal(DBusConnection* conn, char *interface, char *method, void (*deal_method)(char *sigvalue)) {
	DBusMessage *msg;
	DBusMessageIter args;
	while (run) {
		// non blocking read of the next available message
		dbus_connection_read_write(conn, 0);
		msg = dbus_connection_pop_message(conn);

		// loop again if we haven't read a message
		if (NULL == msg) {
			sleep(1);
			continue;
		}
		// check if the message is a signal from the correct interface and with the correct name
		if (dbus_message_is_signal(msg, interface, method)) {
			// read the parameters
			if (!dbus_message_iter_init(msg, &args))
				fprintf(stderr, "no param: \n");
			else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
				fprintf(stderr, "args is not string: \n");
			else
				dbus_message_iter_get_basic(&args, &sigvalue);

			receive_exit(sigvalue);

			deal_method(sigvalue);
		}
		// free the message
		dbus_message_unref(msg);
	}
}

DBusConnection *dbus_get_connection() {
	dbus_error_init(&err);
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "connect: (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (NULL == conn) {
		exit(1);
	}
	return conn;
}

void set_connection_name(DBusConnection* conn, char *name) {
	// request our name on the bus and check for errors
	ret = dbus_bus_request_name(conn, name, DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "name: (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
		exit(1);
	}
}

int add_match(DBusConnection* conn, char *type, char *interface) {
	char disc[128];
	sprintf(disc, "type='%s',interface='%s'", type, interface);
	dbus_bus_add_match(conn, disc, &err); // see signals from the given interface
	dbus_connection_flush(conn);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "match: (%s)\n", err.message);
		return 0;
	}
	return 1;
}

DBusMessage *create_new_msg_singal(char *path, char *iterface, char *method) {
	msg = dbus_message_new_signal(path, iterface, method);
	if (NULL == msg) {
		fprintf(stderr, "crate error\n");
	}
	return msg;
}

void add_args_tomsg(DBusMessage *msg, void *p) {
	DBusMessageIter args;
	dbus_message_iter_init_append(msg, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &p)) {
		fprintf(stderr, "no memory!\n");
	}
}

void send_msg(DBusConnection* conn, DBusMessage *msg) {
	if (!dbus_connection_send(conn, msg, &serial)) {
		fprintf(stderr, "no memory!\n");
	}
	dbus_connection_flush(conn);
	dbus_message_unref(msg);
}

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

int run = 1;

DBusConnection *rece_conn;
DBusConnection *send_conn;

DBusMessage *s_msg;

DBusError err;
int ret = 0;

char* sigvalue;
dbus_uint32_t serial = 0;

void method_exit(char *sigvalue) {
	if (strcmp(sigvalue, "exit") == 0) {
		run = 0;
	}
}

void receive_method_process(char *s) {
	printf("---%s--11-\n", s);
}

void receive_singal(DBusConnection* conn, char *interface, char *method, void (deal_method)(char *)) {
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
				fprintf(stderr, "Message Has No Parameters\n");
			else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
				fprintf(stderr, "Argument is not string!\n");
			else
				dbus_message_iter_get_basic(&args, &sigvalue);

			method_exit(sigvalue);

			deal_method(sigvalue);
		}
		// free the message
		dbus_message_unref(msg);
	}
}

DBusConnection *dbus_get_connection() {
	DBusConnection *conn;
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
	DBusMessage *msg = dbus_message_new_signal(path, iterface, method);
	if (NULL == msg) {
		fprintf(stderr, "Message Null\n");
	}
	return msg;
}

void add_args_tomsg(DBusMessage *msg, void *p) {
	DBusMessageIter args;
	dbus_message_iter_init_append(msg, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &p)) {
		fprintf(stderr, "Out Of Memory!\n");
	}
}

void send_msg(DBusConnection* conn, DBusMessage *msg) {
	if (!dbus_connection_send(conn, msg, &serial)) {
		fprintf(stderr, "Out Of Memory!\n");
	}
	dbus_connection_flush(conn);
	dbus_message_unref(msg);
}


void receive_thread(struct thread_param *p) {
	rece_conn = dbus_get_connection();
	set_connection_name(rece_conn, p->thread_name);
	if (add_match(rece_conn, p->type, p->interface)) {
		receive_singal(rece_conn, p->interface, p->method, receive_method_process);
	}
}


void send_thread(struct thread_param *p) {

	send_conn = dbus_get_connection();
	set_connection_name(send_conn, p->thread_name);
	s_msg = create_new_msg_singal(p->path, p->interface, p->method);
	add_args_tomsg(s_msg,p->sigvalue);
	send_msg(send_conn,s_msg);
}

int main(int argc, char **argv) {
	dbus_error_init(&err);

	pthread_t receive_1;
	struct thread_param rec_p_1 = {
			.thread_name = "test.singal.receive",
			.path = NULL,
			.type = "signal",
			.interface = "test.signal.Type",
			.method = "testmethod",
			.sigvalue = NULL, };
	if (pthread_create(&receive_1, NULL, (void*) receive_thread, &rec_p_1) < 0) {
		_exit(1);
	}

	sleep(1);

	pthread_t send_1;
	struct thread_param send_p_1 = {
			.thread_name = "test.singal.send",
			.path = "/test/signal/xxx",
			.type = "signal",
			.interface = "test.signal.Type",
			.method = "testmethod",
			.sigvalue = "exit", };
	if (pthread_create(&send_1, NULL, (void*) send_thread, &send_p_1) < 0) {
		_exit(1);
	}


	pthread_join(send_1, NULL);
	pthread_join(receive_1, NULL);
	return 0;
}

#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>

struct receive_thread_param {
	char *receive_thread_name;
	char *type;
	char *interface;
	char *method;
};

struct send_thread_param {
	char *send_thread_name;
	char *interface;
	char *method;
	char* sigvalue;
};

void sendsignal_thread(struct send_thread_param *send_p) {
	DBusMessage* msg;
	DBusMessageIter args;
	DBusConnection* conn;
	DBusError err;
	int ret;
	dbus_uint32_t serial = 0;

	printf("Sending signal with value %s\n", send_p->sigvalue);

	// initialise the error value
	dbus_error_init(&err);

	// connect to the DBUS system bus, and check for errors
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "Connection Error (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (NULL == conn) {
		exit(1);
	}

	printf("-------send--%s-\n",send_p->send_thread_name);
	// register our name on the bus, and check for errors
	ret = dbus_bus_request_name(conn, send_p->send_thread_name, DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "Name Error (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
		exit(1);
	}

	// create a signal & check for errors
	printf("---1111----send--%s--%s--\n",send_p->interface,send_p->method);
	msg = dbus_message_new_signal("/test/signal/Object", // object name of the signal
			send_p->interface, // interface name of the signal
			send_p->method); // name of the signal
	if (NULL == msg) {
		fprintf(stderr, "Message Null\n");
		exit(1);
	}

	// append arguments onto signal
	dbus_message_iter_init_append(msg, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &send_p->sigvalue)) {
		fprintf(stderr, "Out Of Memory!\n");
		exit(1);
	}

	// send the message and flush the connection
	if (!dbus_connection_send(conn, msg, &serial)) {
		fprintf(stderr, "Out Of Memory!\n");
		exit(1);
	}
	dbus_connection_flush(conn);

	printf("Signal Sent\n");

	// free the message and close the connection
	dbus_message_unref(msg);
//	dbus_connection_close(conn);
}

void receive_thread(struct receive_thread_param *param) {
	DBusMessage* msg;
	DBusMessageIter args;
	DBusConnection* conn;
	DBusError err;
	int ret;
	char* sigvalue;

	printf("Listening for signals\n");

	// initialise the errors
	dbus_error_init(&err);

	// connect to the bus and check for errors
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "Connection Error (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (NULL == conn) {
		exit(1);
	}

	// request our name on the bus and check for errors
	ret = dbus_bus_request_name(conn, param->receive_thread_name, DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "Name Error (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
		exit(1);
	}

	// add a rule for which messages we want to see
	char disc[128];
	sprintf(disc, "type='%s',interface='%s'", param->type, param->interface);
	printf("---%s-\n", disc);
	dbus_bus_add_match(conn, disc, &err); // see signals from the given interface
	dbus_connection_flush(conn);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "Match Error (%s)\n", err.message);
		exit(1);
	}
	printf("Match rule sent\n");

	// loop listening for signals being emmitted
	while (true) {
		// non blocking read of the next available message
		dbus_connection_read_write(conn, 0);
		msg = dbus_connection_pop_message(conn);

		// loop again if we haven't read a message
		if (NULL == msg) {
			sleep(1);
			continue;
		}

		// check if the message is a signal from the correct interface and with the correct name
		if (dbus_message_is_signal(msg, param->interface, param->method)) {
			// read the parameters
			if (!dbus_message_iter_init(msg, &args))
				fprintf(stderr, "Message Has No Parameters\n");
			else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
				fprintf(stderr, "Argument is not string!\n");
			else
				dbus_message_iter_get_basic(&args, &sigvalue);

			printf("Got Signal with value %s\n", sigvalue);
		}
		// free the message
		dbus_message_unref(msg);
	}
	// close the connection
//	dbus_connection_close(conn);
}

struct receive_thread_param rec_p_1 = {
			.receive_thread_name = "test.singal.receive",
			.type = "signal",
			.interface = "test.signal.Type",
			.method = "testmethod", };

struct send_thread_param send_p_1 = {
			.send_thread_name = "test.singal.send1",
			.interface = "test.signal.Type",
			.method = "testmethod",
			.sigvalue = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", };


struct send_thread_param send_p_2 = {
			.send_thread_name = "test.singal.send2",
			.interface = "test.signal.Type",
			.method = "testmethod",
			.sigvalue = "2222222222222222222222222222", };


struct send_thread_param send_p_3 = {
			.send_thread_name = "test.singal.send3",
			.interface = "test.signal.Type",
			.method = "testmethod",
			.sigvalue = "33333333333333333333333333333333333333333", };

int main(int argc, char **argv) {

	pthread_t receive_1;
	if (pthread_create(&receive_1, NULL, (void*) receive_thread, &rec_p_1) < 0) {
		_exit(1);
	}

	sleep(2);


	pthread_t send_1;
	if (pthread_create(&send_1, NULL, (void*) sendsignal_thread, &send_p_1) < 0) {
		_exit(1);
	}

	pthread_t send_2;
	if (pthread_create(&send_2, NULL, (void*) sendsignal_thread, &send_p_2) < 0) {
		_exit(1);
	}


	pthread_t send_3;
	if (pthread_create(&send_3, NULL, (void*) sendsignal_thread, &send_p_3) < 0) {
		_exit(1);
	}



	pthread_join(send_1, NULL);
	pthread_join(send_2, NULL);
	pthread_join(send_3, NULL);
	pthread_join(receive_1, NULL);

	return 0;

}

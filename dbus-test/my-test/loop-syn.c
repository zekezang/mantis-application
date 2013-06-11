#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>


void broadcast_and_wait_response(char* server_name) {
	DBusMessage* msg;
	DBusMessageIter args;
	DBusConnection* conn;
	DBusError err;
	DBusPendingCall* pending;
	int ret;
	char *stat;
	dbus_uint32_t level;

//	printf("broadcast_and_wait_response-----Calling remote method\n");
	// initialiset the errors

	dbus_error_init(&err);

	// connect to the system bus and check for errors
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "Connection Error (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (NULL == conn) {
		exit(1);
	}

	// request our name on the bus
	ret = dbus_bus_request_name(conn, "test.method.caller", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "Name Error (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
		exit(1);
	}

	char name_[50];
	sprintf(name_,"test.method.%s",server_name);
	// create a new method call and check for errors
	msg = dbus_message_new_method_call(name_, // target for the method call
			"/test/method/Object", // object to call on
			"test.method.Type", // interface to call on
			"Method"); // method name
	if (NULL == msg) {
		fprintf(stderr, "Message Null\n");
		exit(1);
	}

	char *param = "hello hanmeimei..";
	// append arguments
	dbus_message_iter_init_append(msg, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &param)) {
		fprintf(stderr, "Out Of Memory!\n");
		exit(1);
	}

	// send message and get a handle for a reply
	if (!dbus_connection_send_with_reply(conn, msg, &pending, -1)) { // -1 is default timeout
		fprintf(stderr, "Out Of Memory!\n");
		exit(1);
	}
	if (NULL == pending) {
		fprintf(stderr, "Pending Call Null\n");
		exit(1);
	}
	dbus_connection_flush(conn);

	// free message
	dbus_message_unref(msg);

//	printf("broadcast_and_wait_response-----...............wait........\n");

	// block until we recieve a reply
	dbus_pending_call_block(pending);

//	printf("broadcast_and_wait_response-----now reply........come in..\n");

	// get the reply message
	msg = dbus_pending_call_steal_reply(pending);
	if (NULL == msg) {
		fprintf(stderr, "Reply Null\n");
		exit(1);
	}
	// free the pending message handle
	dbus_pending_call_unref(pending);

	stat = (char*)malloc(128);
	// read the parameters
	if (!dbus_message_iter_init(msg, &args))
		fprintf(stderr, "Message has no arguments!\n");
	else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
		fprintf(stderr, "Argument is not string!\n");
	else
		dbus_message_iter_get_basic(&args, &stat);

	if (!dbus_message_iter_next(&args))
		fprintf(stderr, "Message has too few arguments!\n");
	else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args))
		fprintf(stderr, "Argument is not int!\n");
	else
		dbus_message_iter_get_basic(&args, &level);

//	printf("broadcast_and_wait_response----Got Reply: %s, %d\n", stat, level);
	printf("%s, %d\n", stat, level);

	// free reply and close connection
	dbus_message_unref(msg);
//	dbus_connection_close(conn);
}


void reply_to_method_call(DBusMessage* msg, DBusConnection* conn) {
	DBusMessage* reply;
	DBusMessageIter args;
	dbus_uint32_t level = 88;
	dbus_uint32_t serial = 0;
	char* param = "";

	// read the arguments
	if (!dbus_message_iter_init(msg, &args))
		fprintf(stderr, "Message has no arguments!\n");
	else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
		fprintf(stderr, "Argument is not string!\n");
	else
		dbus_message_iter_get_basic(&args, &param);

//	printf("reply_to_method_call----Method called with %s add prepare to reply\n", param);
	printf("%s \n", param);

	// create a reply from the message
	reply = dbus_message_new_method_return(msg);

	const char *stat_x = "Hello lilei----who your's";
	// add the arguments to the reply
	dbus_message_iter_init_append(reply, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &stat_x)) {
		fprintf(stderr, "Out Of Memory!\n");
		exit(1);
	}
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &level)) {
		fprintf(stderr, "Out Of Memory!\n");
		exit(1);
	}
	// send the reply && flush the connection
	if (!dbus_connection_send(conn, reply, NULL)) {
		fprintf(stderr, "Out Of Memory!\n");
		exit(1);
	}
	dbus_connection_flush(conn);
	// free the reply
	dbus_message_unref(reply);
}

void listen_and_forward(char *my_name) {
	DBusMessage* msg;
	DBusMessage* reply;
	DBusMessageIter args;
	DBusConnection* conn;
	DBusError err;
	int ret;
	char* param;

//	printf("listen_and_forward-----coming into server init\n");
	// initialise the error
	dbus_error_init(&err);

	// connect to the bus and check for errors
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "Connection Error (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (NULL == conn) {
		fprintf(stderr, "Connection Null\n");
		exit(1);
	}

	// request our name on the bus and check for errors
	char name_[50];
	sprintf(name_,"test.method.%s",my_name);
	ret = dbus_bus_request_name(conn, name_, DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "Name Error (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
		fprintf(stderr, "Not Primary Owner (%d)\n", ret);
		exit(1);
	}

	// loop, testing for new messages
	while (true) {
		// non blocking read of the next available message
		dbus_connection_read_write(conn, 0);
		msg = dbus_connection_pop_message(conn);

		// loop again if we haven't got a message
		if (NULL == msg) {
			sleep(1);
			continue;
		}

		// check this is a method call for the right interface & method
		if (dbus_message_is_method_call(msg, "test.method.Type", "Method")){
			reply_to_method_call(msg, conn);
			sleep(1);
			break;
		}
		// free the message
		dbus_message_unref(msg);
	}

	// close the connection
//	dbus_connection_close(conn);
}



void echo_a(){
	listen_and_forward("server");
}

void echo_b(){
	broadcast_and_wait_response("server");
}




int main(int argc, char **argv) {


	pid_t pid = fork();

	if (pid > 0) {
		echo_a();
	} else if (pid == 0) {
		echo_b();
	}


//	pthread_t echo_A, echo_B;
//
//	if (pthread_create(&echo_A, NULL, (void*) echo_a, NULL) < 0) {
//		perror("\necho_A:");
//		_exit(1);
//	}
//
//	sleep(1);
//
//	if (pthread_create(&echo_B, NULL, (void*) echo_b, NULL) < 0) {
//		perror("\necho_B:");
//		_exit(1);
//	}
//
//
//
//	pthread_join(echo_A, NULL);
//	pthread_join(echo_B, NULL);

	return 0;

}


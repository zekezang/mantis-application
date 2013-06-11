
#include "dbus_core.h"

DBusConnection *rece_conn;
DBusConnection *send_conn;
DBusMessage *s_msg;
DBusError err;


void receive_method_process(char *s) {
	printf("---%s--11-\n", s);
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

/*
 * dbus-ding-listen.c
 *
 *  Created on: 2012-9-14
 *      Author: zekezang
 */

#include <glib.h>                               // 包含glib库
#include <dbus/dbus-glib.h>                     // 包含glib库中D-Bus管理库
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <stdlib.h>

static DBusHandlerResult signal_filter(DBusConnection *connection, DBusMessage *message, void *user_data); // 定义接收消息函数的原型
static char *s = NULL;

int main() {
	s = (char*)malloc(100);
	GMainLoop *loop; // 定义一个事件循环对象的指针
	DBusConnection *bus; // 定义总线连接对象的指针
	DBusError error; // 定义D-Bus错误消息对象
	loop = g_main_loop_new(NULL, FALSE); // 创建新事件循环对象
	dbus_error_init(&error); // 将错误消息对象连接到D-Bus 错误消息对象
	bus = dbus_bus_get(DBUS_BUS_SESSION, &error); // 连接到总线
	if (!bus) { // 判断是否连接错误
		g_warning("connection error: %s", error.message); // 使用GLib输出错误警告信息
		dbus_error_free(&error); // 清除错误消息
		return 1;
	}
	dbus_connection_setup_with_g_main(bus, NULL); // 将总线设为接收GLib事件循环
	dbus_bus_add_match(bus, "type='signal',interface='com.zekezang.dbus.Signal'", &error); // 定义匹配器
	dbus_connection_add_filter(bus, signal_filter, loop, NULL); // 调用函数接收消息
	g_main_loop_run(loop); // 启动事件循环
	free(s);
	return 0;
}


static DBusHandlerResult signal_filter(DBusConnection *connection, DBusMessage *message, void *user_data) {  // 定义接收消息函数的细节
	GMainLoop *loop = user_data; // 定义事件循环对象的指针，并与主函数中的同步
//	if (dbus_message_is_signal(message, DBUS_INTERFACE_ORG_FREEDESKTOP_LOCAL, "Disconnected")) {  // 接收连接成功消息，判断是否连接失败
//		g_main_loop_quit(loop); // 退出主循环
//		return DBUS_HANDLER_RESULT_HANDLED;
//	}
	DBusError error; // 定义错误对象
	dbus_error_init(&error); // 将错误消息对象连接到D-Bus错误
	if (dbus_message_is_signal(message, "com.zekezang.dbus.Signal", "helloworld")) {  // 指定消息对象路径，判断是否成功
		if (dbus_message_get_args(message, &error, DBUS_TYPE_STRING, &s, DBUS_TYPE_INVALID)) {  // 接收消息，并判断是否有错误
			g_print("receive message: %s\n", s); // 输出接收到的消息
//			dbus_free(s); // 清除该消息----zeke
		} else { // 有错误时执行下列语句
			g_print("receive message, but error: %s\n", error.message);
			dbus_error_free(&error);
		}
		return DBUS_HANDLER_RESULT_HANDLED;
	}

	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

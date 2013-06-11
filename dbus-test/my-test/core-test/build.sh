

arm-linux-gcc dbus_core.c test.c \
-I/sourcecode/arm-workspace/dbus-test/out/include \
-I/sourcecode/arm-workspace/dbus-test/my-test/core-test \
-L/sourcecode/arm-workspace/dbus-test/out/lib -ldbus-1 -lglib-2.0 -lpthread -O2 -o xxx


build:
	arm-linux-gcc dbus-demo.c -I/sourcecode/arm-workspace/dbus-test/out/include -L/sourcecode/arm-workspace/dbus-test/out/lib -ldbus-1 -lglib-2.0 -o a
	
arm-linux-gcc loop-syn.c -I/sourcecode/arm-workspace/dbus-test/out/include \
-L/sourcecode/arm-workspace/dbus-test/out/lib -ldbus-1 -lglib-2.0 -lpthread -O2 -o loop



arm-linux-gcc loop-asyn.c -I/sourcecode/arm-workspace/dbus-test/out/include \
-L/sourcecode/arm-workspace/dbus-test/out/lib -ldbus-1 -lglib-2.0 -lpthread -O2 -o asyn

arm-linux-gcc msn-loop.c -I/sourcecode/arm-workspace/dbus-test/out/include \
-L/sourcecode/arm-workspace/dbus-test/out/lib -ldbus-1 -lglib-2.0 -lpthread -O2 -o msn
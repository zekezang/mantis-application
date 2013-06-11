

build:
	gcc dbus-ding-listen.c -L/lib -ldbus-1 -lglib-2.0 -ldbus-glib-1 -o listen
	
	gcc dbus-ding-send.c -L/lib -ldbus-1 -lglib-2.0 -ldbus-glib-1 -o send
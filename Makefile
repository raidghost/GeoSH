OBJECTS_SERVER = main_server.o graphics.o game.o server.o
OBJECTS_CLIENT = main_client.o client.o callbacks.o
CFLAGS_SERVER = -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
CFLAGS_CLIENT = -pthread -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gtk-3.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/mirclient -I/usr/include/mircore -I/usr/include/mircookie -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng16 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
LIBS_SERVER = -lSDL -lGL -lGLU -L/usr/lib/x86_64-linux-gnu -lm
LIBS_CLIENT = -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0
OPTIONS = -std=gnu11

default : all

all : $(OBJECTS_SERVER) $(OBJECTS_CLIENT)
	gcc $(OPTIONS) $(CFLAGS_SERVER) $(OBJECTS_SERVER) $(LIBS_SERVER) -o server
	gcc $(OPTIONS) $(CFLAGS_CLIENT) $(OBJECTS_CLIENT) $(LIBS_CLIENT) -o client
main_server.o : main_server.c
	gcc $(OPTIONS) $(CFLAGS_SERVER) -c main_server.c
main_client.o : main_client.c
	gcc $(OPTIONS) $(CFLAGS_CLIENT) -c main_client.c
graphics.o : graphics.c
	gcc $(OPTIONS) $(CFLAGS_SERVER) -c graphics.c
game.o : game.c
	gcc $(OPTIONS) $(CFLAGS_SERVER) -c game.c
server.o : server.c
	gcc $(OPTIONS) $(CFLAGS_SERVER) -c server.c
callbacks.o : callbacks.c
	gcc $(OPTIONS) $(CFLAGS_CLIENT) -c callbacks.c
client.o : client.c
	gcc $(OPTIONS) $(CFLAGS_CLIENT) -c client.c


clean :
	rm -f $(OBJECTS_SERVER) $(OBJECTS_CLIENT)

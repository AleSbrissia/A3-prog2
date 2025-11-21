TARGET = A3 
SOURCES = main.c player.c joystick.c fase.c game.c 
HEADERS = joystick.h player.h fase.h game.h
ALLEGRO = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)


all: $(SOURCES) $(HEADERS)
	gcc $(SOURCES) -o $(TARGET) $(ALLEGRO)

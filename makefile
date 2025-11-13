
TARGET = A3 
SOURCES = main.c Square.c Joystick.c   
ALLEGRO = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)


all: $(SOURCES) Joystick.h Square.h
	gcc $(SOURCES) -o $(TARGET) $(ALLEGRO)

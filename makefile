TARGET = Forest_Run 
CFLAGS = -Wall -g
SOURCES = main.c player.c joystick.c game.c obstacle.c platform.c
HEADERS = joystick.h player.h game.h obstacle.h platform.h

ALLEGRO = $(shell pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 --libs --cflags) -lm

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	gcc $(CFLAGS) $(SOURCES) -o $(TARGET) $(ALLEGRO)

clean:
	rm -f $(TARGET)
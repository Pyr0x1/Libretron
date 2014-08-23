all: *.c *.h
	gcc -O *.c `pkg-config --cflags gtk+-3.0` -o libretron `pkg-config --libs gtk+-3.0`

install: 
	mv libretron /usr/local/bin
	cp ../data/libretron.png /usr/share/pixmaps
	chmod +x ../data/libretron.desktop
	cp ../data/libretron.desktop /usr/share/applications

uninstall:
	rm /usr/local/bin/libretron
	rm /usr/share/pixmaps/libretron.png
	rm /usr/share/applications/libretron.desktop

#
# ICP 2014/2015 Makefie
#

# Hack for Fedora (no qmake, but qmake-qt5)
ifneq ("$(wildcard /usr/bin/qmake-qt5)","")
QMAKE = "qmake-qt5"
else
QMAKE = "qmake"
endif
ifneq ("$(wildcard /usr/local/share/Qt-5.2.1/5.2.1/gcc_64/bin/qmake)","")
QMAKE = "/usr/local/share/Qt-5.2.1/5.2.1/gcc_64/bin/qmake"
endif


# all actions
all: gui cli

#build gui
gui:
	@cd src && $(QMAKE) -o Makefile && make
	mv src/labyrinth ./labyrinth2015


# buildtests
cli:
	g++ src/test.cpp src/map.cpp src/player.cpp src/block.cpp -o labyrinth2015-cli


# run - build and run
run: all
	./labyrinth2015-cli
	./labyrinth2015

doxygen:
	-mkdir doc
	doxygen src/doxygen.conf

pack: clean
	zip xnechu01-xsmutk00.zip Makefile src doc save.dat LICENSE README.txt


# clean
clean:
	rm src/*.o  -rf
	rm src/Makefile  -rf
	rm labyrinth2015  -rf
	rm labyrinth2015-cli  -rf
	rm src/labyrinth -rf
	rm doc/* -rf
	rm -rf xnechu01-xsmutk00.zip

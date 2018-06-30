
all:
	cd libs && make -j4
	cd particles_test && make -j4


clean:
	cd libs && make clean
	cd particles_test && make clean

all:
#	make -f Makefile.tests all
	make -f Makefile.lpcxpresso V=1 all

clean:
	make -f Makefile.tests clean
	make -f Makefile.lpcxpresso clean

tests:
	make -f Makefile.tests all

tests_clean:
	make -f Makefile.tests clean

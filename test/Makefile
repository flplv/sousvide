.phony: all

output = sousvide-tests

cpp_main = main.cpp

cpp_sources = \
    test_example.cpp \
    test_owts.cpp \
    mocks.cpp \

c_sources = \
#    test_from_c_code.c \

cpp_flags = -I ./ -I ../src -Wall -std=c++14 -g -O0
c_flags = -I ./ -I ../src -Wall -std=gnu11 -g  -O0
ld_flags = -L ../src
libs = -lsousvide

main_objs = $(addprefix objs/,$(patsubst %.cpp,%.o,$(cpp_main)))
cpp_objs = $(addprefix objs/,$(patsubst %.cpp,%.o,$(cpp_sources)))
c_objs = $(addprefix objs/,$(patsubst %.c,%.o,$(c_sources)))

all: src_lib pre $(output) run

src_lib:
	make -C ../src -f Makefile.lib

pre:
	mkdir -p objs
	rm -rf $(cpp_objs)

run: $(output)
	./$(output) -a

clean:
	rm -rf objs
	rm -f $(output)

objs/%.o: %.c
	gcc -c $(c_flags) -o $@ $^

objs/%.o: %.cpp
	g++ -c $(cpp_flags) -o $@ $^

$(output): $(c_objs) $(cpp_objs) $(main_objs)
	g++ $(ld_flags) -o $(output) $^ $(libs)

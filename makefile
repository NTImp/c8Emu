src := $(wildcard src/*.c)
objects := $(addprefix bin/,$(notdir $(addsuffix .o ,$(basename $(src)))))


flags = -std=c11 -lSDL2 -lSDL2main

out = c8EMU
out_dir = bin

binary_route := $(out_dir)/$(out)

build: $(out_dir) $(objects)
	gcc -o $(binary_route) $(objects) $(flags)

$(out_dir):
	mkdir $(out_dir)

bin/%.o: src/%.c
	gcc -c $< $(flags) -o $@
src := $(wildcard src/*.c)

flags = -std=c11

out = c8EMU
out_dir = bin

binary_route = $(out_dir)/$(out)


build: $(out_dir) $(src)
	gcc -o $(binary_route) $(src) $(flags)

$(out_dir):
	mkdir $(out_dir)
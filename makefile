src := $(wildcard src/*.c)
objects := $(notdir $(addsuffix .o ,$(basename $(src))))


flags = -std=c11 -lSDL2 -lSDL2main

target = c8EMU

$(target): $(objects)
	gcc -o $(target) $(objects) $(flags)

%.o: src/%.c
	gcc -c $< $(flags) -o $@

clean:
	rm $(objects) $(target)

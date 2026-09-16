CSRCS := $(shell find ./ -name "*.c")

debug:
	gcc -g $(shell pkg-config --libs libgvc) -ldag_viewer -lraylib -lyaml $(CSRCS) -o bstide
	./bstide ../cop/

clean:
	rm bstide

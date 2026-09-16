CSRCS := $(shell find ./ -name "*.c")

debug:
	gcc -g -ldag_viewer -lraylib -lyaml $(CSRCS) -o bstide
	./bstide ../cop/

clean:
	rm bstide

CSRCS := $(shell find ./ -name "*.c")
BSTDIR ?=

all: run

install: build
	sudo cp bstide /usr/bin/.

run: debug
	./bstide $(BSTDIR)

build: $(CSRCS)
	gcc -O3 -ldag_viewer -lraylib -lyaml $(CSRCS) -o bstide

debug: $(CSRCS)
	gcc -g -ldag_viewer -lraylib -lyaml $(CSRCS) -o bstide

clean:
	rm bstide

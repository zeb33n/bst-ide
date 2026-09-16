CSRCS := $(shell find ./ -name "*.c")
BSTDIR ?=

all: install

install: build
	sudo cp bstide /usr/bin/.

run: debug
	./bstide $(BSTDIR)

build: $(CSRCS)
	gcc -O3 $(shell pkg-config --libs libgvc) -ldag_viewer -lraylib -lyaml $(CSRCS) -o bstide

debug: $(CSRCS)
	gcc -g $(shell pkg-config --libs libgvc) -ldag_viewer -lraylib -lyaml $(CSRCS) -o bstide

clean:
	rm bstide

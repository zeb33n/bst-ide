CSRCS := $(shell find ./ -name "*.c")
BSTDIR ?=

all: run

install: build
	sudo cp bstide /usr/bin/.

run: debug
	./bstide $(BSTDIR)

build: $(CSRCS)
	gcc -O3 -ldag_viewer -lraylib -lyaml $(CSRCS) -o bstide

build-static: $(CSRCS)
	gcc -O3 -Iinclude -lgvc -lcdt -lcgraph -lyaml -lm -lX11 $(CSRCS) deps/libdag_viewer.a deps/libraylib.a -o bstide

debug: $(CSRCS)
	gcc -g -ldag_viewer -lraylib -lyaml $(CSRCS) -o bstide

clean:
	rm bstide

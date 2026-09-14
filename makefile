CSRCS := $(shell find ./ -name "*.c")

debug:
	gcc -g -ldag_viewer -lraylib $(CSRCS) -o bstide
	./bstide ../sbomber/site/dag_viewer_assets/processed_sbom.dot

clean:
	rm bstide

all:
	gcc -ldag_viewer -lraylib main.c dv_defs.c utils.c -o bstide
	./bstide ../dag-viewer/site/dag_viewer_assets/processed_pilk.dot

clean:
	rm bstide

build: image_editor

image_editor: main.o apply_functions.o basic_functions.o load_functions.o crop_functions.o histogram.o rotate.o
	gcc -Wall -Wextra -g main.o apply_functions.o basic_functions.o load_functions.o crop_functions.o histogram.o rotate.o -lm -o image_editor
clean:
	rm -f main.o apply_functions.o basic_functions.o load_functions.o crop_functions.o histogram.o rotate.o

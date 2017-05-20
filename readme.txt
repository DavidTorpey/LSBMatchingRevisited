STEP 1:
	gcc `pkg-config --cflags opencv` LSB\ Matching\ Revisited.c `pkg-config --libs opencv` -o LSB

STEP 2:
	./LSB

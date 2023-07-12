objects = hytab.o n1.o n2.o n3.o n4.o n5.o n6.o n7.o n8.o n9.o n10.o n11.o ni.o nii.o ntab.o 
CFLAGS = -DNROFF -w -fsanitize=address -ggdb -O0
LDFLAGS = -fsanitize=address -ggdb
all: $(objects) nroff

nroff: $(objects)
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(objects): %.o: %.c

clean:
	rm -f *.o all
	rm nroff

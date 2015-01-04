main: main.o parse_ini_file.o
	gcc $^ -o $@

clean: 
	-rm main *.o

sources=main.c parse_ini_file.c
include $(sources:.c=.d)

%.d: %.c 
	set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
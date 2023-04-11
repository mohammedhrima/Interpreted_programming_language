
exe = ./exe
srcs = file.c tools.c
hedears = header.h

all: $(exe)

$(exe) : $(srcs)
	@rm -rf $(exe)
	@gcc -fsanitize=address -fsanitize=null -g3 $(srcs) -o $(exe)
	@$(exe)

clean:
	rm -rf $(exe)

re: clean all
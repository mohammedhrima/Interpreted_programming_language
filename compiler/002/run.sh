rm -rf a.out
gcc -fsanitize=address -fsanitize=null -fsanitize=undefined -g3 $@ && ./a.out
rm -rf a.out
NAME = philosophersv2
all : $(NAME)

$(NAME) : philosophersv2.o
	gcc -o $(NAME)  philosophersv2.o


philosophersv2.o : philosophersv2.c philosophersv2.h
	gcc -o philosophersv2.o -c philosophersv2.c

clean : 
	del /Q *.o
fclean: clean
	del /Q philosophersv2.exe

re: fclean all

.PHONY: all clean fclean re

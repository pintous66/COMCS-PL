CC      = gcc
CFLAGS  = -Wall -Wextra -g

# Primeiro argumento: o ficheiro .c
SRC = $(word 2,$(MAKECMDGOALS))
OBJ = $(SRC:.c=.o)
EXEC = $(SRC:.c=.bin)

# Argumentos extra depois do ficheiro
ARGS = $(wordlist 3,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

# Compilar .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Criar o executável
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para correr o programa com argumentos
run: $(EXEC)
	./$(EXEC) $(ARGS)

# Limpar
clean:
	rm -f *.o *.bin

# Impede o make de tentar criar ficheiros com nomes iguais aos argumentos
%:
	@:

CC      = gcc
CFLAGS  = -Wall -Wextra -g

# O primeiro argumento depois de "make run"
SRC = $(word 2,$(MAKECMDGOALS))
OBJ = $(SRC:.c=.o)
EXEC = $(SRC:.c=.bin)

# Compilar .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Criar o executável
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para correr o programa
run: $(EXEC)
	./$(EXEC)

# Limpar
clean:
	rm -f *.o *.bin

# Isto impede que o make tente criar ficheiros com nomes iguais aos "argumentos"
%:
	@:

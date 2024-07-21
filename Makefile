# Compilador e flags do compilador
CC = gcc
CFLAGS = -g -Wall -I./bibliotecas

# Diretórios
SRC_DIRS = auxiliares auxiliares_arvB funcionalidades funcionalidades_arvB
INCLUDE_DIR = bibliotecas

# Arquivos fonte
SRCS = $(wildcard $(foreach dir, $(SRC_DIRS), $(dir)/*.c)) main.c

# Executável alvo
TARGET = programaTrab

# Regra padrão
all: $(TARGET)

# Linkar os arquivos objeto para criar o executável
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Executar o programa
run: all
	./$(TARGET)

# Limpeza
clean:
	rm -f $(TARGET) valgrind.log

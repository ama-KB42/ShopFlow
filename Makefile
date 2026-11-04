# Makefile - ShopFlow
# Utilisation : make        -> compile le programme
#               make clean  -> supprime les fichiers compiles
#               make valgrind -> lance le programme sous valgrind

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
OBJ = main.o utils.o produit.o client.o commande.o statistique.o initialisation.o

programme: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o programme

main.o: main.c types.h utils.h produit.h client.h commande.h statistique.h initialisation.h
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c utils.h types.h
	$(CC) $(CFLAGS) -c utils.c

produit.o: produit.c produit.h types.h utils.h
	$(CC) $(CFLAGS) -c produit.c

client.o: client.c client.h types.h utils.h
	$(CC) $(CFLAGS) -c client.c

commande.o: commande.c commande.h types.h client.h produit.h utils.h
	$(CC) $(CFLAGS) -c commande.c

statistique.o: statistique.c statistique.h types.h utils.h
	$(CC) $(CFLAGS) -c statistique.c

initialisation.o: initialisation.c initialisation.h types.h client.h produit.h commande.h
	$(CC) $(CFLAGS) -c initialisation.c

valgrind: programme
	valgrind --leak-check=full ./programme

clean:
	rm -f $(OBJ) programme

cmake_minimum_required(VERSION 4.3)
project(code_clion C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(initialisation)
include_directories(types) 

dd_executable(code_clion 
        .idea/.gitignore
        .idea/code_v2.2.iml
        .idea/modules.xml
        initialisation.c
        initialisation.h
        commande.c
        commande.h
        client.c
        client.h
        statistique.c
        statistique.h
        produit.c
        produit.h
        utils.c
        utils.h
        types.h
        main.c) 


/* programme: client.c commande.c initialisation.c main.c produit.c statistique.c utils.c
	gcc client.c commande.c initialisation.c main.c produit.c statistique.c utils.c -o programme

programme: client.c commande.c initialisation.c main.c produit.c statistique.c utils.c
    gcc -Wall -Wextra client.c commande.c initialisation.c main.c produit.c statistique.c utils.c -o programme */ 

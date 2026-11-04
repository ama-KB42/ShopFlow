cmake_minimum_required(VERSION 3.10)
project(code_clion C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

include_directories(.)

add_executable(code_clion
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

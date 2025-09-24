typedef struct NodoBlockchain {
    int id;
    char *mensaje;
    struct NodoBlockchain *anterior;
} NodoBlockchain;

#ifndef SCANNER_H
#define SCANNER_H
#include <iostream>
#include <fstream>

class Scanner
{
    public:
        Scanner();
        virtual ~Scanner();

        bool cargarFichero(char *nombre);
        std::string extraerToken();
        std::string siguienteToken(); //Extrae un token sin consumirlo
        std::ifstream fichero_fuente;


    protected:
    private:

};

#endif // SCANNER_H

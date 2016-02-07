#include "../include/Scanner.h"
#include <sstream>      // std::stringstream

Scanner::Scanner()
{

}

Scanner::~Scanner()
{
    fichero_fuente.close();
}

bool Scanner::cargarFichero(char *nombre)
{
    fichero_fuente.open(nombre);

    //Comprobamos si hubo éxito en la carga
    if(fichero_fuente.is_open())
        return true;
    else
        return false;
}

std::string Scanner::extraerToken()
{
    std::string token;

    //Extraemos un token y lo consumimos
    if(!fichero_fuente.eof())
    {
        fichero_fuente >> token;

        return token;
    }
    else
    {
        fichero_fuente.close();
        return "EOF";
    }
}

std::string Scanner::siguienteToken()
{
    std::string token;

    //Extraemos un token y restauramos posicion
    if(!fichero_fuente.eof())
    {
        int posicion_actual = fichero_fuente.tellg();

        fichero_fuente >> token;

        //Restauramos posicion
        fichero_fuente.seekg(posicion_actual,fichero_fuente.beg);
        return token;
    }
    else
    {
        return "EOF";
    }
}



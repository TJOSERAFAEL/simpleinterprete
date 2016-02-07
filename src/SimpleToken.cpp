#include "../include/SimpleToken.h"

SimpleToken::SimpleToken()
{
    //Construimos la tabla de símbolos
    Tabla_Token["while"]    = palabra_reservada;
    Tabla_Token["wend"]    = palabra_reservada;
    Tabla_Token["if"]       = palabra_reservada;
    Tabla_Token["endif"]    = palabra_reservada;
    Tabla_Token["="]        = asignacion;
    Tabla_Token["+"]        = aritmetico;
    Tabla_Token["%"]        = aritmetico;
    Tabla_Token["-"]        = aritmetico;
    Tabla_Token["*"]        = aritmetico;
    Tabla_Token["/"]        = aritmetico;
    Tabla_Token[">"]        = simbolo_especial;
    Tabla_Token["<"]        = simbolo_especial;
    Tabla_Token["("]        = simbolo_especial;
    Tabla_Token[")"]        = simbolo_especial;
    Tabla_Token["print"]    = palabra_reservada;
    Tabla_Token["println"]    = palabra_reservada;
    Tabla_Token["read"]     = palabra_reservada;
    Tabla_Token["\""]       = simbolo_especial;
    Tabla_Token["#"]        = simbolo_especial;
}

SimpleToken::~SimpleToken()
{
    Tabla_Token.clear();
}


bool SimpleToken::esSimbolo(std::string token)
{
    std::map<std::string,tipo_token>::iterator it;

    //Buscamos el token en la tabla
    it = Tabla_Token.find(token);

    //Si no lo encontramos significa que no es un símbolo
    //reservado
    if (it != Tabla_Token.end())
        return true;
    else
        return false;
}

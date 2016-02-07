#ifndef SIMPLETOKEN_H
#define SIMPLETOKEN_H
#include <map>
#include <string>

class SimpleToken
{
    public:
        SimpleToken();
        virtual ~SimpleToken();
        bool esSimbolo(std::string token);

    private:

        enum tipo_token
        {
            palabra_reservada,
            simbolo_especial,
            aritmetico,
            asignacion
        };

        std::map<std::string,tipo_token> Tabla_Token;
};

#endif // SIMPLETOKEN_H

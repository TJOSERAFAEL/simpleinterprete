#ifndef SIMPLEINTERPRETE_H
#define SIMPLEINTERPRETE_H
#include <map>
#include <stack>
#include <string>
#include "Scanner.h"
#include "SimpleToken.h"
#include "tree.h"
#include <stack>
#include <queue>
#include <cstdlib>
#include <stdlib.h>     /* atoi */
#include <vector>
#include <utility>      // std::pair, std::get
#include <algorithm> // std::pair, std::get

class SimpleInterprete
{
    public:
        SimpleInterprete();
        virtual ~SimpleInterprete();

        void ejecutar(char *fichero);

    private:
        int num;

        enum tipo_token
        {
            palabra_reservada,
            simbolo_especial,
            asignacion,
            aritmetico,
            numero_entero,
            cadena,
            variable
        };


        void escanear();
        void construirArbol(char *fichero);
        void rellenarTablaVariables();

        void interpretar();
        void interpretar_asignacion(tree<std::pair<std::string,tipo_token>>::node nodo);
        int interpretar_suma(tree<std::pair<std::string,tipo_token>>::node nodo);
        int interpretar_resta(tree<std::pair<std::string,tipo_token>>::node nodo);
        int interpretar_division(tree<std::pair<std::string,tipo_token>>::node nodo);
        int interpretar_multiplicacion(tree<std::pair<std::string,tipo_token>>::node nodo);
        int interpretar_modulo(tree<std::pair<std::string,tipo_token>>::node nodo);

        void interpretar_print(tree<std::pair<std::string,tipo_token>>::node nodo);
        void interpretar_println(tree<std::pair<std::string,tipo_token>>::node nodo);
        void interpretar_read(tree<std::pair<std::string,tipo_token>>::node nodo);
        void interpretar_while(tree<std::pair<std::string,tipo_token>>::node nodo);
        void interpretar_if(tree<std::pair<std::string,tipo_token>>::node nodo);
        void interpretar_nodo(tree<std::pair<std::string,tipo_token>>::node nodo);
        int interpretar_nodo_aritmetico(tree<std::pair<std::string,tipo_token>>::node nodo);

        tree<std::pair<std::string,tipo_token>> construirSubArbol();
        tree<std::pair<std::string,tipo_token>> construirSubArbol_Print();
        tree<std::pair<std::string,tipo_token>> construirSubArbol_While();

        std::map<std::string,int> Tabla_Variables;
        Scanner scanner;
        SimpleToken s_token;



        //Árbol generado interpretación
        tree<std::pair<std::string,tipo_token>> arbol;

};

#endif // SIMPLEINTERPRETE_H

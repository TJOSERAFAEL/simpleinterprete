#include "../include/SimpleInterprete.h"

SimpleInterprete::SimpleInterprete()
{
    //ctor
    num = 0;
}

SimpleInterprete::~SimpleInterprete()
{
    //dtor
}

inline bool isInteger(const std::string & s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

    char * p ;
    strtol(s.c_str(), &p, 10) ;

    return (*p == 0) ;
}




tree<std::pair<std::string,SimpleInterprete::tipo_token>> SimpleInterprete::construirSubArbol()
{

    std::string token;

    token = scanner.extraerToken();
    std::string op_left = token;


    if(!s_token.esSimbolo(op_left)) //No es una palabra reservada, puede ser variable o numero entero
    {
        std::string sig_token;

        sig_token = scanner.siguienteToken();

        if(sig_token == "=" || sig_token == "+" || sig_token == "-" || sig_token == "*" || sig_token == "/" || sig_token == "%")
        {
            scanner.extraerToken(); //Consumimos el token

            tree<std::pair<std::string,tipo_token>> arbol_aux;

            if( sig_token == "=") //Tipo asignacion
            {
                arbol_aux.setroot(std::make_pair(sig_token,asignacion));
            }
            else //Tipo aritmetico
            {
                arbol_aux.setroot(std::make_pair(sig_token,aritmetico));
            }

            tree<std::pair<std::string,tipo_token>> arbol_aux2;

            arbol_aux2 = construirSubArbol(); //LLamada recursiva

            arbol_aux.insert_left(arbol_aux.root(),arbol_aux2);

            if(isInteger(op_left))
            {
                arbol_aux.insert_left(arbol_aux.root(),std::make_pair(op_left,numero_entero));
            }
            else //NO es numero entero , no es simbolo por tanto es una variable
            {
                arbol_aux.insert_left(arbol_aux.root(),std::make_pair(op_left,variable));
            }

            return arbol_aux;
        }
        else
        {

            tree<std::pair<std::string,tipo_token>> arbol_aux;

            //Comprobamos si es un numero
            if(isInteger(op_left))
            {
                arbol_aux.setroot(std::make_pair(op_left,numero_entero));
            }
            else
            {
                arbol_aux.setroot(std::make_pair(op_left,variable));
            }

            return arbol_aux;
        }
    }
    else  //Simbolo reservado o numero entero
    {

        tree<std::pair<std::string,tipo_token>> arbol_aux;

        if(isInteger(op_left))
        {
            arbol_aux.setroot(std::make_pair(op_left,numero_entero));
        }
        else
        {
            if(op_left == "print" || op_left == "println")
            {
                arbol_aux.setroot(std::make_pair(op_left,palabra_reservada));

                std::string token;

                std::stack<std::string> hijos_nodo_print;

                // token = scanner.extraerToken();
                // hijos_nodo_print.push(token);

                while( token != ";")
                {
                    token = scanner.extraerToken();
                    std::size_t found;

                    found=token.find("\"");

                    //Si el token tiene comillas " es una cadena y por
                    //tanto debemos concatenar los sucesivos tokens hasta encontrar
                    // final de cadena "
                    if(found!=std::string::npos)
                    {

                        std::string token_cadena = scanner.extraerToken();
                        found = token_cadena.find("\"");

                        //Mientras no encontremos " segumis extrayendo token
                        while(found==std::string::npos)
                        {
                            std::string aux;
                            aux = scanner.extraerToken();
                            token_cadena += ' ' + aux;
                            found = aux.find("\"");
                        }
                        //Cadena resultante

                        token += ' ' + token_cadena;
                        //Borramos la ultima comilla
                        token.pop_back();
                    }

                    if(token != ";")
                        hijos_nodo_print.push(token);

                }

                while(!hijos_nodo_print.empty())
                {
                    std::size_t found;
                    found = hijos_nodo_print.top().find("\"");

                    //Si el token es una cadena debemos poner
                    //tipo cadena
                    if(found!=std::string::npos)
                    {
                        arbol_aux.insert_left(arbol_aux.root(),std::make_pair(hijos_nodo_print.top(),cadena));
                    }
                    else //Es una variable
                    {
                        arbol_aux.insert_left(arbol_aux.root(),std::make_pair(hijos_nodo_print.top(),variable));
                    }

                    hijos_nodo_print.pop();
                }
            }
            else if(op_left == "while")
            {
                arbol_aux.setroot(std::make_pair(op_left,palabra_reservada));


                std::stack< tree<std::pair<std::string,tipo_token>>> pila_sub_arboles;

                while(true)
                {
                    tree<std::pair<std::string,tipo_token>> sub_arbol;
                    sub_arbol = construirSubArbol();

                    if((*sub_arbol.root()).first == "wend")
                        break;

                    pila_sub_arboles.push(sub_arbol);
                }

                //Insertar en orden correcto los sub arboles
                while(!pila_sub_arboles.empty())
                {
                    arbol_aux.insert_left(arbol_aux.root(),pila_sub_arboles.top());
                    pila_sub_arboles.pop();
                }

            }
            else if(op_left == "wend")
            {
                arbol_aux.setroot(std::make_pair(op_left,palabra_reservada));
            }
            else if(op_left == "if")
            {
                arbol_aux.setroot(std::make_pair(op_left,palabra_reservada));


                std::stack< tree<std::pair<std::string,tipo_token>>> pila_sub_arboles;

                while(true)
                {
                    tree<std::pair<std::string,tipo_token>> sub_arbol;
                    sub_arbol = construirSubArbol();

                    if((*sub_arbol.root()).first == "endif")
                        break;

                    pila_sub_arboles.push(sub_arbol);
                }

                //Insertar en orden correcto los sub arboles
                while(!pila_sub_arboles.empty())
                {
                    arbol_aux.insert_left(arbol_aux.root(),pila_sub_arboles.top());
                    pila_sub_arboles.pop();
                }

            }
            else if(op_left == "endif")
            {
                arbol_aux.setroot(std::make_pair(op_left,palabra_reservada));
            }
            else if(op_left == "read") //Solo leemos variables
            {
                arbol_aux.setroot(std::make_pair(op_left,palabra_reservada));

                std::string token;

                token = scanner.extraerToken();

                arbol_aux.insert_left(arbol_aux.root(),std::make_pair(token,variable));
            }
            else
            {
                arbol_aux.setroot(std::make_pair(op_left,palabra_reservada));
            }
        }

        return arbol_aux;
    }
}


tree<std::pair<std::string,SimpleInterprete::tipo_token>> SimpleInterprete::construirSubArbol_Print()
{
    std::string token;

    token = scanner.extraerToken();

    std::string op_left = token;
    tree<std::pair<std::string,tipo_token>> arbol_aux(std::make_pair(op_left,variable));

    while(token != ";")
    {
        token = scanner.extraerToken();
    }

    return arbol_aux;
}

void SimpleInterprete::construirArbol(char *fichero)
{

    if(!scanner.cargarFichero(fichero))
    {
        std::cout << "Fallo al cargar: " << fichero << std::endl;
        return;
    }

    arbol.setroot(std::make_pair("start",palabra_reservada));

    //Para insertar los arboles de forma correcta
    std::stack< tree<std::pair<std::string,tipo_token>>> pila_sub_arboles;

    while( !scanner.fichero_fuente.eof())
    {
        tree<std::pair<std::string,tipo_token>> arbol_aux;

        arbol_aux = construirSubArbol();
        pila_sub_arboles.push(arbol_aux);
    }

    //Recorremos la pila de sub arboles y los insertamos (Ahora se insertan
    //en orden correcto

    while(!pila_sub_arboles.empty())
    {
        arbol.insert_left(arbol.root(),pila_sub_arboles.top());
        pila_sub_arboles.pop();
    }

    interpretar();
}

void SimpleInterprete::ejecutar(char *fichero)
{
    construirArbol(fichero);
}

void SimpleInterprete::interpretar()
{
    //Recorremos arbol por niveles
    std::queue<tree<std::pair<std::string,tipo_token> >::node> cola;

    //Insertamos root
    cola.push(arbol.root());

    tree<std::pair<std::string,tipo_token> >::node nodo = cola.front();
    cola.pop();

    nodo = nodo.left();
    cola.push(nodo);

    //Metemos el primer nivel en la cola
    while(!nodo.next_sibling().null())
    {
        nodo = nodo.next_sibling();
        cola.push(nodo);
    }

    while(!cola.empty())
    {
        nodo = cola.front();
        interpretar_nodo(nodo); //Interpretamos el nodo
        cola.pop();
    }
}


void SimpleInterprete::interpretar_asignacion(tree<std::pair<std::string,tipo_token>>::node nodo)
{

    tree<std::pair<std::string,tipo_token>>::node aux = nodo.left();

    // Si el hermano es un numero asignamos diractamente
    if((*aux.next_sibling()).second == numero_entero)
    {
        std::string valor = (*aux.next_sibling()).first;
        Tabla_Variables[(*aux).first] = atoi(valor.c_str());
    }
    else if((*aux.next_sibling()).second == variable)
    {
        //Left = Right
        //Asignacion de una variable al valor de otra variable
        //Ejemplo a = b
        Tabla_Variables[(*aux).first] = Tabla_Variables[(*aux.next_sibling()).first];
    }
    else if((*aux.next_sibling()).second == aritmetico )
    {
        //Le pasamos el nodo hermano como argumento ( + - * / % ...)
        Tabla_Variables[(*aux).first] = interpretar_nodo_aritmetico(aux.next_sibling());
    }
}

int SimpleInterprete::interpretar_suma(tree<std::pair<std::string,tipo_token>>::node nodo)
{
    int valor_left;
    int valor_right;

    //Comprobamos si el nodo left es un numero o una variable
    if((*nodo.left()).second == numero_entero)
    {
        valor_left = atoi(((*nodo.left()).first).c_str());
    }
    else if ((*nodo.left()).second == variable)
    {
        valor_left = Tabla_Variables[(*nodo.left()).first];
    }

    //Ahora el nodo es el nodo left para poder acceder a su hermano
    nodo = nodo.left();

    //Si el hermano es un numero
    if((*nodo.next_sibling()).second == numero_entero)
    {
        //Convertimos el string a entero
        valor_right = atoi(((*nodo.next_sibling()).first).c_str());
    }
    else if ((*nodo.next_sibling()).second == variable ) //Si el hermano es una variable
    {
        valor_right = Tabla_Variables[(*nodo.next_sibling()).first];
    }
    else if((*nodo.next_sibling()).second == aritmetico ) //Si el hermano es operador aritmetico
    {
        //Le pasamos el nodo hermano como argumento ( + - * / % ...)
        valor_right = interpretar_nodo_aritmetico(nodo.next_sibling());//Llamada recursiva
    }


    return valor_left+valor_right;
}

int SimpleInterprete::interpretar_resta(tree<std::pair<std::string,tipo_token>>::node nodo)
{
    int valor_left;
    int valor_right;

    //Comprobamos si el nodo left es un numero o una variable
    if((*nodo.left()).second == numero_entero)
    {
        valor_left = atoi(((*nodo.left()).first).c_str());
    }
    else if ((*nodo.left()).second == variable)
    {
        valor_left = Tabla_Variables[(*nodo.left()).first];
    }

    //Ahora el nodo es el nodo left para poder acceder a su hermano
    nodo = nodo.left();

    //Si el hermano es un numero
    if((*nodo.next_sibling()).second == numero_entero)
    {
        //Convertimos el string a entero
        valor_right = atoi(((*nodo.next_sibling()).first).c_str());
    }
    else if ((*nodo.next_sibling()).second == variable ) //Si el hermano es una variable
    {
        valor_right = Tabla_Variables[(*nodo.next_sibling()).first];
    }
    else if( (*nodo.next_sibling()).second == aritmetico ) //Si el hermano es operador aritmetico
    {
        //Le pasamos el nodo hermano como argumento ( + - * / % ...)
        valor_right = interpretar_nodo_aritmetico(nodo.next_sibling());//Llamada recursiva
    }


    return valor_left-valor_right;
}

int SimpleInterprete::interpretar_division(tree<std::pair<std::string,tipo_token>>::node nodo)
{
    int valor_left;
    int valor_right;

    if((*nodo.left()).second == numero_entero)
    {
        valor_left = atoi(((*nodo.left()).first).c_str());
    }
    else if ((*nodo.left()).second == variable)
    {
        valor_left = Tabla_Variables[(*nodo.left()).first];
    }

    //Ahora el nodo es el nodo left para poder acceder a su hermano
    nodo = nodo.left();

    //Si el hermano es un numero
    if((*nodo.next_sibling()).second == numero_entero)
    {
        //Convertimos el string a entero
        valor_right = atoi(((*nodo.next_sibling()).first).c_str());
    }
    else if ((*nodo.next_sibling()).second == variable ) //Si el hermano es una variable
    {
        valor_right = Tabla_Variables[(*nodo.next_sibling()).first];
    }
    else if( (*nodo.next_sibling()).second == aritmetico ) //Si el hermano es operador aritmetico
    {
        //Le pasamos el nodo hermano como argumento ( + - * / % ...)
        valor_right = interpretar_nodo_aritmetico(nodo.next_sibling());//Llamada recursiva
    }

    return valor_left/valor_right;
}

int SimpleInterprete::interpretar_multiplicacion(tree<std::pair<std::string,tipo_token>>::node nodo)
{

    int valor_left;
    int valor_right;

    if((*nodo.left()).second == numero_entero)
    {
        valor_left = atoi(((*nodo.left()).first).c_str());
    }
    else if ((*nodo.left()).second == variable)
    {
        valor_left = Tabla_Variables[(*nodo.left()).first];
    }

    //Ahora el nodo es el nodo left para poder acceder a su hermano
    nodo = nodo.left();

    //Si el hermano es un numero
    if((*nodo.next_sibling()).second == numero_entero)
    {
        //Convertimos el string a entero
        valor_right = atoi(((*nodo.next_sibling()).first).c_str());
    }
    else if ((*nodo.next_sibling()).second == variable ) //Si el hermano es una variable
    {
        valor_right = Tabla_Variables[(*nodo.next_sibling()).first];
    }
    else if( (*nodo.next_sibling()).second == aritmetico ) //Si el hermano es operador aritmetico
    {
       //Le pasamos el nodo hermano como argumento ( + - * / % ...)
        valor_right = interpretar_nodo_aritmetico(nodo.next_sibling());//Llamada recursiva
    }

    return valor_left*valor_right;
}

int SimpleInterprete::interpretar_modulo(tree<std::pair<std::string,tipo_token>>::node nodo)
{

    int valor_left;
    int valor_right;

    if((*nodo.left()).second == numero_entero)
    {
        valor_left = atoi(((*nodo.left()).first).c_str());
    }
    else if ((*nodo.left()).second == variable)
    {
        valor_left = Tabla_Variables[(*nodo.left()).first];
    }

    //Ahora el nodo es el nodo left para poder acceder a su hermano
    nodo = nodo.left();

    //Si el hermano es un numero
    if((*nodo.next_sibling()).second == numero_entero)
    {
        //Convertimos el string a entero
        valor_right = atoi(((*nodo.next_sibling()).first).c_str());
    }
    else if ((*nodo.next_sibling()).second == variable ) //Si el hermano es una variable
    {
        valor_right = Tabla_Variables[(*nodo.next_sibling()).first];
    }
    else if( (*nodo.next_sibling()).second == aritmetico ) //Si el hermano es operador aritmetico
    {
         //Le pasamos el nodo hermano como argumento ( + - * / % ...)
        valor_right = interpretar_nodo_aritmetico(nodo.next_sibling());//Llamada recursiva
    }

    return valor_left%valor_right;
}

void SimpleInterprete::interpretar_print(tree<std::pair<std::string,tipo_token>>::node nodo)
{
    //tree<std::string>::node aux = nodo.left();
    nodo = nodo.left();

    std::queue<tree<std::pair<std::string,tipo_token>>::node> cola;

    //Insertamos root
    cola.push(nodo);

    //Metemos los hijos del nodo print
    while(!nodo.next_sibling().null())
    {
        nodo = nodo.next_sibling();
        cola.push(nodo);
    }

    //Imprimimos los hijos
    while(!cola.empty())
    {
        //Comprobamos si el token es un numero entero
        if((*cola.front()).second == numero_entero)
        {
            std::cout << (*cola.front()).first;
        }
        else if ((*cola.front()).second == cadena)
        {
            //FIX temporal para las comillas delanteras
            std::cout << (*cola.front()).first.substr(1,(*cola.front()).first.length());
        }
        else if ((*cola.front()).second == variable)
        {
            std::cout << Tabla_Variables[(*cola.front()).first];
        }

        cola.pop();
    }
}

void SimpleInterprete::interpretar_println(tree<std::pair<std::string,tipo_token>>::node nodo)
{
    //tree<std::string>::node aux = nodo.left();
    nodo = nodo.left();

    std::queue<tree<std::pair<std::string,tipo_token>>::node> cola;

    //Insertamos root
    cola.push(nodo);

    //Metemos los hijos del nodo print
    while(!nodo.next_sibling().null())
    {
        nodo = nodo.next_sibling();
        cola.push(nodo);
    }

    //Imprimimos los hijos
    while(!cola.empty())
    {
        //Comprobamos si el token es un numero entero
        if((*cola.front()).second == numero_entero)
        {
            std::cout << (*cola.front()).first;
        }
        else if ((*cola.front()).second == cadena)
        {
            //FIX temporal para las comillas delanteras

            std::cout << (*cola.front()).first.substr(1,(*cola.front()).first.length());
        }
        else if ((*cola.front()).second == variable)
        {
            std::cout << Tabla_Variables[(*cola.front()).first];
        }

        cola.pop();
    }

    //Salto de linea
    std::cout << std::endl;
}



void SimpleInterprete::interpretar_read(tree<std::pair<std::string,tipo_token>>::node nodo)
{
    nodo = nodo.left();

    int var_left;

    std::cin >> var_left; //Leemos la variable

    //Asiganamos el resultado de la lectura a la variable
    Tabla_Variables[(*nodo).first] = var_left;
}


void SimpleInterprete::interpretar_while(tree<std::pair<std::string,tipo_token>>::node nodo)
{
    nodo = nodo.left();

    std::vector<tree<std::pair<std::string,tipo_token>>::node> vector_hijos;

    vector_hijos.push_back(nodo);

    //Metemos los hijos del while al vector hijos
    while(!nodo.next_sibling().null())
    {
        nodo = nodo.next_sibling();

        vector_hijos.push_back(nodo);
    }

    int valor_right = 0;
    //Comprobamos si el hijo 3 es un numero o una variable
    if((*vector_hijos[2]).second == numero_entero)
    {
        valor_right = atoi(((*vector_hijos[2]).first).c_str());
    }
    else //Es una variable
    {
        valor_right = Tabla_Variables[(*vector_hijos[2]).first];
    }

    //Comprobamos que tipo de condicion es
    if((*vector_hijos[1]).first == "<") //Condicion <
    {
        while(Tabla_Variables[(*vector_hijos[0]).first] < valor_right)
        {
            for(int i = 3; i < vector_hijos.size(); i++)
            {
                nodo = vector_hijos[i];

                interpretar_nodo(nodo);
            }
        }
    }

    else if((*vector_hijos[1]).first == ">") //Condicion <
    {
        while(Tabla_Variables[(*vector_hijos[0]).first] > valor_right)
        {
            for(int i = 3; i < vector_hijos.size(); i++)
            {
                nodo = vector_hijos[i];

                interpretar_nodo(nodo);
            }
        }
    }

    else if((*vector_hijos[1]).first == "==") //Condicion <
    {
        while(Tabla_Variables[(*vector_hijos[0]).first] == valor_right)
        {
            for(int i = 3; i < vector_hijos.size(); i++)
            {
                nodo = vector_hijos[i];

                interpretar_nodo(nodo);
            }
        }
    }

    else if((*vector_hijos[1]).first == "!=") //Condicion <
    {
        while(Tabla_Variables[(*vector_hijos[0]).first] != valor_right)
        {
            for(int i = 3; i < vector_hijos.size(); i++)
            {
                nodo = vector_hijos[i];

                interpretar_nodo(nodo);
            }
        }
    }

}

void SimpleInterprete::interpretar_if(tree<std::pair<std::string,tipo_token>>::node nodo)
{
    nodo = nodo.left();

    std::vector<tree<std::pair<std::string,tipo_token>>::node> vector_hijos;

    vector_hijos.push_back(nodo);

    //Metemos los hijos del if al vector hijos
    while(!nodo.next_sibling().null())
    {
        nodo = nodo.next_sibling();

        vector_hijos.push_back(nodo);
    }

    int valor_right = 0;
    //Comprobamos si el hijo 3 es un numero o una variable
    if((*vector_hijos[2]).second == numero_entero)
    {
        valor_right = atoi(((*vector_hijos[2]).first).c_str());
    }
    else //Es una variable
    {
        valor_right = Tabla_Variables[(*vector_hijos[2]).first];
    }

    //Comprobamos que tipo de condicion es
    if((*vector_hijos[1]).first == "<") //Condicion <
    {
        if(Tabla_Variables[(*vector_hijos[0]).first] < valor_right)
        {
            for(int i = 3; i < vector_hijos.size(); i++)
            {
                nodo = vector_hijos[i];

                interpretar_nodo(nodo);
            }
        }
    }

    else if((*vector_hijos[1]).first == ">") //Condicion <
    {
        if(Tabla_Variables[(*vector_hijos[0]).first] > valor_right)
        {
            for(int i = 3; i < vector_hijos.size(); i++)
            {
                nodo = vector_hijos[i];

                interpretar_nodo(nodo);
            }
        }
    }

    else if((*vector_hijos[1]).first == "==") //Condicion <
    {
        if(Tabla_Variables[(*vector_hijos[0]).first] == valor_right)
        {
            for(int i = 3; i < vector_hijos.size(); i++)
            {
                nodo = vector_hijos[i];

                interpretar_nodo(nodo);
            }
        }
    }

    else if((*vector_hijos[1]).first == "!=") //Condicion <
    {
        if(Tabla_Variables[(*vector_hijos[0]).first] != valor_right)
        {
            for(int i = 3; i < vector_hijos.size(); i++)
            {
                nodo = vector_hijos[i];

                interpretar_nodo(nodo);
            }
        }
    }

}

void  SimpleInterprete::interpretar_nodo(tree<std::pair<std::string,tipo_token>>::node nodo)
{
    if((*nodo).first == "=") //Interpretar asignacion
    {
        interpretar_asignacion(nodo);
    }

    if((*nodo).first == "print") //Interpretar print
    {
        interpretar_print(nodo);
    }

    if((*nodo).first == "println") //Interpretar println
    {
        interpretar_println(nodo);
    }

    if((*nodo).first == "read") //Interpretar read
    {
        interpretar_read(nodo);
    }

    if((*nodo).first == "while") //Interpretar while
    {
        interpretar_while(nodo);
    }

    if((*nodo).first == "if") //Interpretar if
    {
        interpretar_if(nodo);
    }
}

int SimpleInterprete::interpretar_nodo_aritmetico(tree<std::pair<std::string,tipo_token>>::node nodo)
{

    if((*nodo).first == "+")
    {
        return interpretar_suma(nodo);//Llamada recursiva
    }
    else if ((*nodo).first == "-" )
    {
        return interpretar_resta(nodo);//Llamada recursiva
    }
    else if ((*nodo).first == "*" )
    {
        return interpretar_multiplicacion(nodo);//Llamada recursiva
    }
    else if ((*nodo).first == "/" )
    {
        return interpretar_division(nodo);//Llamada recursiva
    }
    else if ((*nodo).first == "%")
    {
        return interpretar_modulo(nodo);//Llamada recursiva
    }


}

void SimpleInterprete::escanear()
{
    rellenarTablaVariables();
}

void SimpleInterprete::rellenarTablaVariables()
{
    std::string token;

    //Rellenamos la tabla de variables mediante
    //extracción de tokens
    while(token != "EOF")
    {
        token = scanner.extraerToken();
        //Si el token no es un simbolo reservado
        //lo añadimos a la tabla de variables
        if(!s_token.esSimbolo(token) && token != "EOF")
        {
            //std::cout << token << std::endl;
            Tabla_Variables[token] = 0; //Variable inicializada a 0
        }
    }
}





#include "include/CDiccionario.h"

void ejecutaOpcMP(int opc,CDiccionario *dic);
void ejecutaOpcEntidad(int opc,CDiccionario *dic);
int menuPrincipal();
int menuEntidad();
int menuAtributo();
int menuBloque();
void ejecutaOpcAtributo(int opc, CDiccionario *dic);
void ejecutaOpcBloque(int opc, CDiccionario *dic);

int main(){
    CDiccionario dic;
    int opc=0;

    while(opc!=6){
        opc=menuPrincipal();
        ejecutaOpcMP(opc,&dic);
    }
    dic.cierraArchivo();
    return 0;
}

void ejecutaOpcMP(int opc,CDiccionario *dic){
    int comando=0, comando2=0, comando3=0;

    switch(opc){
        case 1: dic->nuevo();
                break;

        case 2: dic->abrir();
                break;

        case 3:
            if(dic->verificaArchivo()!=false)
            {
            while(comando!=6)
                {
                    comando=menuEntidad();
                    ejecutaOpcEntidad(comando,dic);
                }
            }
            else
            {
                cout<<"\n****NO NINGUN ARCHIVO ABIERTO****\n";
            }
            break;
        case 4:
            if(dic->verificaArchivo()!=false)
            {
            while(comando2!=4)
                {
                    comando2=menuAtributo();
                    ejecutaOpcAtributo(comando2,dic);
                }
            }
            else
            {
                cout<<"\n****NO NINGUN ARCHIVO ABIERTO****\n";
            }
            break;
        case 5:
            if(dic->verificaArchivo()!=false)
            {
              while(comando3!=5)
                {
                    comando3=menuBloque();
                    ejecutaOpcBloque(comando3,dic);
                }
            }
            else
            {
                cout<<"\n****NO NINGUN ARCHIVO ABIERTO****\n";
            }
            break;
        case 6:
            cout << endl;
            cout << "\n****Saliendo del programa****\n";
            break;

        case 7:
            dic->Maximo();

        break;

        default:
		cout << endl;
		cout << "\n****Opcion no reconocida****\n";
		break;
    }
}

void ejecutaOpcEntidad(int opc, CDiccionario *dic){

    switch(opc){
        case 1:
		cout << endl;
		dic->consultaDiccionario();
                break;
        case 2:
		cout << endl;
		dic->ConsultaEntidad();
                break;
        case 3:
		cout << endl;
		dic->altaEntidad();
                break;
        case 4:
		cout << endl;
		dic->bajaEntidad();

                break;
        case 5:
		cout << endl;
		dic->modificaEntidad();

                break;
        case 6:
		cout << endl;
		cout << "\n****Regresando el menu principal****\n";
		break;

        default:
		cout << endl;
		cout<<"\n****Opcion no reconocida****\n";
		break;
    }
}

int menuPrincipal(){
    int opc;

    cout<<"\n1. Nuevo archivo\n"
        <<"2. Abrir archivo\n"
        <<"3. Estructura de tablas\n"
        <<"4. Columnas\n"
        <<"5. Datos\n"
        <<"6. Salir\n"
        <<"7. Funcion especial\n"
        <<">>";
    cin>>opc;
    return opc;
}

int menuEntidad(){
    int opc;
    cout<<"\n>___Menu Entidad___<\n"
        <<"1. Consulta Diccionario\n"
        <<"2. Consulta Entidad\n"
        <<"3. Alta Entidad\n"
        <<"4. Elimina Entidad\n"
        <<"5. Modifica Entidad\n"
        <<"6. Regresar\n"
        <<">>";
    cin>>opc;
    return opc;
}

int menuAtributo(){
	int opc;
    cout<<"\n>___Menu Atributos___<\n"
		<<"1. Alta atributo\n"
		<<"2. Elimina Atributo\n"
		<<"3. Modifica Atributo\n"
		<<"4. Regresar\n"
        <<">>";
	cin >> opc;
	return opc;
}

int menuBloque(){
	int opc;
    cout<<"\n>___Menu Bloques___<\n"
        <<"1. Consulta Bloque\n"
		<<"2. Alta Bloque\n"
		<<"3. Elimina Bloque\n"
		<<"4. Modifica Bloque\n"
		<<"5. Regresar\n"
        <<">>";
	cin >> opc;
	return opc;
}

void ejecutaOpcAtributo(int opc, CDiccionario *dic){
    switch(opc){
	case 1:
		cout << endl;
		dic->altaAtributo();
		break;
	case 2:
		cout << endl;
		dic->eliminaAtr();
		break;
    case 3:
		cout << endl;
		dic->modificaAtributo();
		break;
	case 4:
		cout << endl;
		cout << "\n****Regresando a el menu principal****\n";
		break;


    }
}

void ejecutaOpcBloque(int opc, CDiccionario *dic){

    switch(opc){
        case 1:
            cout << endl;
            dic->consultaBloque();
            break;
        case 2:
            cout << endl;
            dic->AltaBloque();
            break;
        case 3:
            cout << endl;
            dic->BajaBLoque();
            break;
        case 4:
            cout << endl;
            dic->modifcaBloque();
            break;
        case 5:
            cout << endl;
            cout << "\n****Regresando el menu principal****\n";
            break;
        default:
		cout << endl;
		cout<<"\n****Opcion no reconocida****\n";
            break;
    }
}
//menu principal nuevo, abrir, tablas, columnas, datos, salida, funcion especial (entidad, bloque, atributo)

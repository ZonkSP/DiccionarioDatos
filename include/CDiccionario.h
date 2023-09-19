#ifndef CDICCIONARIO_H
#define CDICCIONARIO_H

#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;// evita std::cout

typedef struct{
	char nombre[30];//el nombre es con el que nosotros sabemos reconocer una entidad de otro
	long sig;//donde nosotros guardamos la posici�n de la entidad siguiente a determinada entidad
	long atr;//donde guardamos la posicion donde se encuentra el primer atributo de nuestra entidad
	long bloque;//donde guardamos la posicion donde se encuentra el primer bloque de nuestra entidad
}Entidad;//Estructura de nuestras entidades

enum TIPOATR{CHAR = 1, INT, FLOAT, LONG, DOUBLE};//el enum nos sirve para crear una variable TIPOATR que nos ayuda a definir todos los valores fijos que puede contener

typedef struct{
	char nombre[30];//el nombre de nuestro atributo y poder diferenciarlo de los demas atributos
	long sig;//donde guardamos el siguiente atributo al que esta ligado
	TIPOATR tipo;//varibale que ya tenemos en enum y donde definimos los valores que puede contener
	int tamano;//tama�o del atributo
	char esclave;//donde nostros definimos si nuestro atributo es clave o no
}Atributo;

class CDiccionario{
	public:
		CDiccionario();
		virtual ~CDiccionario();
		//Entidades
		Entidad leeEntidad(long pos);//regresara los datos de un auxiliar y lee la entidad dependiendo de la posicion que reciba
		bool    altaEntidad();//regresa un valor boleano de verdadero o falso si se inserto o no la entidad
		long 	buscaEntidad(char* nombre);//Se le da el nombre de la entidad que queremos encontrar y regresara si encontro la entidad
		long    escribeEntidad(Entidad ent);//regresamos la posicion donde se escriben los datos de la entidad que capturamos
		bool    insertaEntidad(long pos,Entidad ent);//regresa un valor booleano de si se inserto correctamente la entidad y de parametros usamos la posicion en donde se inserto la entidad en el archivo y la entidad ya que en eesta se encuentran los datos a insertar
		void    consultaDiccionario();//no regresa nada ya que solo muestra las entidades existentes
		void    rescribeEntidad(long pos, Entidad cambio);//esta funcion nos permite reescribir una entidad dentro del archivo se ingresa la posicion de la entidad que qeuremos modificar al igual que los datos que vamos a cambiar
		void    cierraArchivo();//no regresa nada ya que solo cierra un archivo ya abierto
		void    escribeCabecera(long cab);//no regresa nada ya que escribe la cabecera de la primera entidad dentro del archivo
		long    leeCabecera();//regresa la posicion de inicio de la lista dentro del archivo
		void    abrir();//no regresa nada ya que abre un archivo ya creado
		void    nuevo();//No regresa nada ya que esta funcion crea un nuevo archivo
		Entidad capturaEntidad();//Se captura la entidad junto con sus datos y  regresa un auxiliar
		long 	eliminaEntidad(char name[30]);//regresa la posicion de la entidad eliminada, pasamos por parametro el nombre de la entidad que eliminaremos
		bool 	bajaEntidad();//regresara la si la entidad se elimino de manera correcta o incorrecta
		bool 	modificaEntidad();//regresa un booleano para verificar si se modifico de manera correcta la entidad
		//Atributos
		Atributo    leeAtributo(long posEnt);//esta funcion lee los atributo del archivo dependiendo de la posici�n que pasamos
		void    consultaAtributos(Entidad ent);//esta funcion nos permite ver los atributos de una entidad
		Atributo    capturaAtributo();//en esta funcion capturaremos los datos del atributo y regresaremos estos datos
		long 	escribeAtributo(Atributo atr);//en esta funcion escribimos escribimos los datos del atributo dentro del archivo y regresamos la posicion de donde los insertamos
		long    buscaAtributo(char atrnombre[30], char entnombre[30]);// en esta funcion buscamos un atributo a partir del nombre de este y del nombre de la entidad donde se encuentra y es de tipo long ya que regresa donde encontramos el atributo
		void 	rescribeAtributo(long pos, Atributo atr);//en esta funcion volvemos a reescribir un  atributo diferente dentro del archivo
		bool 	insertaAtributo(long posatr, long posEnt, Atributo atr);//regresamos un valor boleono de si se inserto correctamente el atributo, necesita la posicion del atributo, el entero y los datos del atributo
		bool 	altaAtributo();//en esta funcion es donde nosotros llamaremos a las funciones de inserta y regresa si se inserto de manera correcta o no el atributo
		void    eliminaAtr();//en esta funcion ingresamos los datos del atributo que vamos a usar para llamar a la funcion eliminar
		long    EliminaAtributo(Entidad entAux,long pos,char *nombre);//esta funcion desenlaza el atributo  que queremos eliminar y necesita del auxiliar entidad, posicion de la entidad donde se encuentra el atributo y su nombre
		bool    modificaAtributo();//esta funcion nos regresa un valor booleano ya que regresa si se modifico de manera correcta el atributo o no
		//Bloques
		void *capturaBloque(Atributo *arrAtr, long tamBloque);//en esta funcion capturamos los datos de un bloque y los regresamos en un void * y usamos el arreglo de atributos y el tama�o del bloque
		void *leeBloque(long posBlo, long tamBloque);//en esta funcion leemos el bloque a partir del archivo y regresamos sus datos, es de tipo void apuntador ya que no sabemos que tipo es y ahcemos uso de la posicion del bloque y su tama�o
		void consultaBloque();//en esta funcion imprimimos los bloques de la entidad
		long buscaBloque(Entidad ent,Atributo *arrAtr, void* bloque, long tamBloque);//esta funcion nos ayuda a buscar un bloque y regresa su posicion en la que lo encontro, se pasan los parametros, la entidad, atributo, el bloque y su tama�o
		long escribeBloque(void *bloque, long tamBloque);//esta funcion escribe el bloque dentro del archivo y regresa la posiciondonde donde lo escribio, se da como parametro el bloque y su tama�o
		void reescribeBloque(long pos, void *bloque, long tamBloque);//en esta funcion volvemos a reescribir un  atributo diferente dentro del archivo, ingresamos la posicion donde lo queremos reescribir y los datos del atributo
        bool AltaBloque();//en esta funcion nosotros damos de alta un bloque esta funcion regresara si se logro insertar de manera correcta o no
        bool InsertaBloque(Entidad ent, Atributo *atr, long posbloq, long posent, long tambloq, void* bloque);//esta funcion se enlazara el nuevo bloque de parametros utilizamos la entidad, el arreglo de atributos, la posicion de la entidad, el tama�o del bloque junto con sus datos
        void *capturaClave(Atributo atr,long tambloque, Entidad ent);//en esta funcion capturamos los datos de un bloque y los regresamos en un void * y usamos el arreglo de atributos y el tama�o del bloque
        long EliminaBloque(Entidad ent, Atributo *atr, long posbloq, long posent, long tambloq, void* clave);//la funcion elimina bloque desenlaza al bloquede los demas y regreesa la posicion, de parametros utilizamos la entidad, el arreglo de atributos la posicion de la entidad, la posicion del bloque, el tama�o del bloque y la clave del bloque que queremos eliminar
        bool BajaBLoque();//Esta funcion nos regresa un valor booleano ya que nos dice si se elimino de manera correcta el bloque
        bool modifcaBloque();//este metodo nos permite modificar un bloque ya exitente y regresa un valor booleano si se modifico de manera correcta o incorrecta
        //Funcion get atributo y compara bloques
        Atributo* getAtributos(Entidad ent, long *tamBloque);//en esta funcion nosotros obtenemos los tipos de datos de los atributos y de parametros utiliza la entidad y el tama�o del bloque que al mismo tiempo este se modificara
        long comparaBloques(Atributo esclave,void *bloque1, void *bloque2);//en esta funcion comparamos las claves de los bloques ya que estas no pueden repetirse o en caso de querer buscar un bloque de parametros ocupamos el auxiliar de atributo para verificar el tipo de dato de la clave y los dos bloques que vmaos a comparar
        //verificaciones
        bool verificaclave(long posent, Atributo atr);//Verifica si dos atributos son ESCLAVE ingresamos la posicion de la entidad, y los atributos
        bool verificaentidad(char nombre[30]);//Verifica si la entidad contiene al menos 1 atributo, ingresamos el nombre de la entidad
        bool verificaAtributo(char nombre[30]);//Verifica si la entidad tiene al menos 1 bloque ingresamos el nombre de la entidad
        bool verificaArchivo();//Verifica si hay un archivo abierto
        bool verificasiclave(Entidad ent);//Verifica si hay al menos 1 atributo ESCLAVE, ingresamos la entodada
        void ConsultaEntidad();//esta funcion nos imprime las entidades y los atributos dentro del archivo
      void Maximo();

	protected:
	private:
        	FILE* arch;
};

#endif // CDICCIONARIO_H

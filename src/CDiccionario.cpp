#include "..\include\CDiccionario.h"

CDiccionario::CDiccionario()
{
    arch=NULL;
}

CDiccionario::~CDiccionario()
{
    //dtor
}

bool CDiccionario::altaEntidad() //regresa un valor boleano de verdadero o falso si se inserto o no la entidad
{
    Entidad ent; //es un apuntador auxiliar
    long posent; //posicion de la entidad
    bool resp=false; //es una bandera que se regresa al insertar la entidad

    ent=capturaEntidad(); //se llama a capturaentidad para obtener el nombre de
    if(buscaEntidad(ent.nombre)==-1)//Busca la entidad para asegurarse de que no se repita y si no la encuentra la inserta
    {
        posent=escribeEntidad(ent); //nos regresa la posiciones de donde se escribio la entidad
        insertaEntidad(posent,ent); //aqui insertamos los datos de la entidad, al igual que la posiscion de esta
        resp=true; //la bandera se pone en true para dar a entender que se inserto correctamente la entidad
    }
    else
        cout<<"\nError la entidad ya existe\n"; //en caso de que en busca entidad ya lo encontro
    return resp; //regresamos la respuesta si se nnserto o no
}

long CDiccionario::buscaEntidad(char *nombre)//Se le da el nombre de la entidad y regresara si encontro la entidad y de parametros usa el nombre de la entidad que estamos buscando
{
    long cabent; //posicion de la cabecera donde empieza a buscar la entidad
    Entidad entAux; //auxiliar de entero con el cual compararemos el nombre de todas las entidades con la que buscamos

    cabent=leeCabecera();//la cabecera se pone en la posicion incial
    while(cabent!=-1)//ciclo que se va a repetir hasta que ya no haya entidades,la lista se encuentre vacia
    {
        entAux=leeEntidad(cabent);//dependiendo de la posicion donde se encuntre la cabecera regresara la entidad que le corresponde
        if(strcmpi(entAux.nombre,nombre)==0) //si el auxiliar es igual a la entidad que nosostros buscamos significa que la encontro dentro de la lista
            return cabent;//regresa la direccion donde encontro la cabecera
        cabent=entAux.sig; //si todavia no lo encuentra entonces la cabecera avanza para comparar con la siguiente entidad
    }
    return -1;//no la encontro
}

Entidad CDiccionario::leeEntidad(long pos)//regresara los datos de un auxiliar y lee la entidad dependiendo de la posicion que reciba
{
    Entidad ent;//auxiliar de la entidad que se regresara
    fseek(arch,pos,SEEK_SET); //el puntero se desplaza por el archivo a la posicion donde se encuentra la entidad de esta
    fread(&ent,sizeof(Entidad),1,arch);//lee los datos del achivo que encontramos con anterioridad
    return ent; //regresa los datos en el auxiliar
}

long CDiccionario::escribeEntidad(Entidad ent)//regresamos la posicion donde se escriben los datos de la entidad
{
    long pos; //posicion donde se escribe la entidad
    fseek(arch,0,SEEK_END); //el puntero se desplaza por el archivo a la posicion donde se encuentra la entidad de esta
    pos=ftell(arch); //Retorna la posición del puntero del archivo
    fwrite(&ent,sizeof(Entidad),1,arch);//escribe dentro del archivo los datos de la entidad
    return pos;//regresa la posicion donde se escribieron los datos de la entidad
}

bool CDiccionario::insertaEntidad(long pos,Entidad ent)//regresa un valor booleano de si se inserto correctamente la entidad
{
    bool res;//el valor que debemos regresar que nos dira si se inserto correctamente la entidad
    long cabent, posentAnt; //posiciones de la cabecera que sera la que inicia y recorre la posicion y la posicion de la entidad anterior que sera con la que se ligara
    Entidad entAux,entAnt; //al igual estos auxiliares corresponden a la entidad a la que se esta insertando y la entidad anterior con la que se ligara

    cabent=leeCabecera();//la cabecera tiene que empezar al inicio del diccionario para irse recorriendo y colocarse en el orden que le corresponda
    //4 casos
    //Caso 1: Lista vacia
    if(cabent==-1)//cuando la lista esta vacia
    {
        escribeCabecera(pos);//se escribe la entidad ya que no hay con quien comparar y es la primera entidad en insertarse
        res=true;//la variable se vuelve true ya que si fue posible ingresar la entidad
    }
    else //en caso de que el diccionario no este vacio
    {
        entAux=leeEntidad(cabent); //el auxiliar que nosotros ya habiamos declarado toma los datos de la ultima entidad que se encuentra en el diccionario
        //Caso 2: Al inicio con datos
        if(strcmpi(ent.nombre,entAux.nombre)<0)//aqui hace una comparacion si la entidad que vamos a insertar va antes de la entidad anterior
        {
            ent.sig=cabent;//ya que la entidad que vamos a ingresar es menor a la otra entidad esta se ligara despues
            rescribeEntidad(pos,ent);//reescribimos la entidad
            escribeCabecera(pos);//la cabecera procede a ser la nueva entidad ya que es la primera
            res=true;//la entidad fue ingresada de manera correcta
        }
        else//en caso de que no deba de ir en la cabecera
        {
            entAnt=entAux;//la entidad anterior se vuelve la cabecera
            posentAnt=cabent;//y la posiscion anterior es igual a la posicion de la cabecera
            while(cabent!=-1 && strcmpi(ent.nombre,entAux.nombre) > 0)//esto es para ir comparando entidades y ver en que parte colocar la entidad que queremos ingresar
            {
                entAux=leeEntidad(cabent);//la entidad auxiliar lee los datos de las enitdades que queremos comaparar
                entAnt=entAux;//la entidad anterior obtiene los datos de la entidad auxiliar
                posentAnt=cabent;//la posicion anterior tiene el valor de la cabecera
                cabent=entAux.sig;//la cabecera obtiene el valor de la entidad sig
                if(cabent!=-1)//si la cabecera no es nula
                    entAux=leeEntidad(cabent);//la entidad auxiliar obtiene los datos de la cabecera actual para segur avanzando
            }
            //caso 3: Al final
            if(cabent==-1)//esto significa que la cabecera es nula y la entidad va al final
            {
                entAnt.sig=pos;//se une la entidad anterior con la actual
                rescribeEntidad(posentAnt,entAnt);//reescribimos la entidad
                res=true;//la entidad fue ingresada de manera correcta
            }
            //Caso 4: En medio
            else
            {
                ent.sig=entAnt.sig;//la entidad sig se vuelve la entidad siguiente del auxiliar que usamos
                entAnt.sig=pos;//la entidad siguiente de la entidad anterior se vuelve la posicion
                rescribeEntidad(pos,ent);//reescribimos la entidad que ingresamos
                rescribeEntidad(posentAnt,entAnt);//reescribimos la entidad anterior ya que tambien cambio
                res=true;//la entidad fue ingresada de manera correcta
            }
        }
    }
    return res;//regresamos si se ingreo la entidad de manera correcta
}

void CDiccionario::consultaDiccionario()//no regresa nada ya que solo muestra las entidades
{
    long cab;//posicion de la cabecera
    Entidad ent;//auxiliar de la entidad donde se encuentran los datos
    cout << "=====Entidades en el archivo=====" << endl;

    cab=leeCabecera();//la cabecera se pone en la posicion de incio
    while(cab!=-1)//esto se va haciendo hasta que ya no haya mas entidades
    {
        ent=leeEntidad(cab);//regresa los datos de la posicion donde se encuentra la cabecera
        cout<<">>"<<ent.nombre <<"\n";//imprime el dato de la entidad
        cab=ent.sig;//la cabecera se recorre para seguir imprimiendo las siguientes entidades
    }
}

Entidad CDiccionario::capturaEntidad()//Se captura la entidad junto con sus datos y  regresa un auxiliar
{
    Entidad nuevo; //un auxiliar de entidad

    cout<<"Coloca el nombre de la entidad: ";
    cin>>nuevo.nombre; //se guarda el nombre de la entidad
    nuevo.bloque=-1; //Guarda un lugar para la posicion del bloque de la entidad
    nuevo.atr=-1; //Guarda un lugar para la posicion del atributo de la entidad
    nuevo.sig=-1; //Guarda un lugar para la posicion de la siguiente entidad

    return nuevo; //regresa el auxiliar donde se guardo la entidad
}

void CDiccionario::rescribeEntidad(long pos, Entidad cambio)//esta funcion nos permite reescribir una entidad dentro del archivo se ingresa la posicion de la entidad que qeuremos modificar al igual que los datos que vamos a cambiar
{
    fseek(arch,pos,SEEK_SET);//el puntero se desplaza por el archivo a la posicion de la entidad
    fwrite(&cambio,sizeof(Entidad),1,arch);//escribe en la posicion la nueva entidad
}

void CDiccionario::nuevo()//No regresa nada ya que esta funcion crea un nuevo archivo
{
    char namearch[50];//nombre del archivo
    long cab=-1;//la cabecera osea el archivo se encuentra vacio ya que aun no se han insertado datos

    cierraArchivo();//si existe un archivo previamente abierto esta duncion lo cierra
    cout<<"Ingrese el nombre del archivo: ";
    cin>>namearch;//ingresa el nombre del nuevo archivo
    if(access(namearch,F_OK)!=1)//verifica si el archivo existe
    {
        arch=fopen(namearch,"wb+");//abre un archivero y regresa un FILE
        fwrite(&cab,sizeof(long),1,arch);//guarda un espacio en el que no hay nada dentro del archivo
    }
    else
        cout<<"\nERROR: El archivo ya existe\n";//manda el mensaje en caso de que ya exista el archivo
}

void CDiccionario::abrir()//no regresa nada ya que abre un archivo ya creado
{
    char namearch[50]; //nombre del archivo que deaseamos abrir

    cierraArchivo(); //cierra el archivo en caso de que uno se encuentre ya abierto
    cout<<"Ingrese el nombre del archivo: ";
    cin>>namearch;//ingresa el nombre del archivo que deseamos abrir
    arch=fopen(namearch,"rb+");//abre un archivero y regresa un FILE
    if(arch==NULL)//en caso de que el archivo no exista imprime un mensaje
        cout<<"\n****ERROR: El archivo no existe****\n";
}

void CDiccionario::cierraArchivo()//no regresa nada ya que solo cierra un archivo ya abierto
{
    if(arch!=NULL)//se cierra el archivo solo en caso de que haya uno
        fclose(arch);//cierra el archivo
    arch=NULL;//ahora ya no hay archivo
}

long CDiccionario::leeCabecera()//regresa la posicion de inicio de la lista dentro del archivo
{
    long cabecera; //posicion de la cabecera

    fseek(arch,0,SEEK_SET);//el puntero se desplaza por el archivo a la posicion donde se encuentra la entidad de esta
    fread(&cabecera,sizeof(long),1,arch);//lee los datos del achivo que encontramos con anterioridad
    return cabecera; //regresa la posicion del incio de la lista
}

void CDiccionario::escribeCabecera(long cab)//no regresa nada ya que escribe la cabecera de la primera entidad dentro del archivo
{
    fseek(arch,0, SEEK_SET);//se pone en el incio del archivo para escibir los datos
    fwrite(&cab, sizeof(long), 1, arch);//  Se escriben los datos dentro del archivo
}

//METODOS ELMINAENTIDAD, BAJAENTIDAD, MODIFICAENTIDAD
long CDiccionario::eliminaEntidad(char name[30])//regresa la posicion de la entidad eliminada, ingresamos el nombre de la entidad que eliminaremos
{
    long cab, posentAnt;//es un long ya que aqui guardaremos la posicion de la cabecera y de la posicion de la entidad anterior que esta ligada a esta entidad
    Entidad ent, entAnt;//los auxiliares de la entidad

    cab = leeCabecera();//la cabecera toma la posicion de inicio
    ent = leeEntidad(cab);//leemos la entidad de la cabecera
    if(strcmpi(ent.nombre,name) == 0)//aqui es donde comparamos si la primera entidad es igual a la entidad que queremos eliminar
        escribeCabecera(ent.sig);//entonces la cabecera pasara a ser la siguiente entidad
    else//si no es la primera entidad entonces
    {
        while(cab != -1 && strcmpi(ent.nombre,name) != 0)//mientras la cabecera no sea nula y encontremos la entidad que estamos buscando
        {
            posentAnt = cab;//la variable poseant obtiene el valor de la cabecera, esto es para guardar la posicion
            entAnt = ent;//el auxiliar entAnt obtiene el valor de la entidad, ya que tenemos que guardar los datos
            cab = ent.sig;//y la cabecera procede a ser la entidad siguien
            if(cab != -1)//si la cabecera no es nula
                ent = leeEntidad(cab);//la entidad procede a tener la posicion de la entidad siguiente osea la cebecera
        }
        if(strcmpi(ent.nombre,name) == 0)//si el nombre de la entidad es igual al nombre de la entidad que nosotros buscamos
        {
            entAnt.sig = ent.sig;//entonces la entidad siguiente del auxiliar que estuvimos usando obtiene los valores de la entidad sig
            rescribeEntidad(posentAnt,entAnt);//reescribimos la entidad con la posicion anterior ya que se recorre
        }
    }
    return cab;//regresamos la posicion de donde se elimino la entidad
}

bool CDiccionario::bajaEntidad()//regresara la si la entidad se elimino de manera correcta o incorrecta
{
    bool res=false;
    char nombre[30];//nombre de la entidad que deseamos eliminar
    long pos;//posicion donde se encontrara esta
    cout<<"\nIngrese la entidad que desea eliminar: ";
    cin>>nombre;//Se ingresa el nombre de la entidad
    pos  = buscaEntidad(nombre);//primero busca la entidad para verificar que esta existe
    if(pos == -1) //en caso de no existir se imprimira un mensaje que la entidad no existe
        cout<<"\n****La entidad "<<nombre <<" no existe.****\n";
    else //en caso de encontrarla se llamara a la funcion elimina entidad
    {
        pos=eliminaEntidad(nombre);
        res=true;
    }
    return res;//regresa si se elimino de manera correcta o incorrecta
}

bool CDiccionario::modificaEntidad()//regresa un booleano para verificar si se modifico de manera correcta la entidad
{
    bool res=false;// este es el dato que vamos a regresar al inicia es falso ya que todavia no se ha modificado la entidad
    char vieja[30];//en estas variables guardaremos los nombres de la entidad que queremos modificar y a que lo vamos a modificar
    long pos;//en este guardaremos la posicionde donde se encuentra la entidad que queremos modificar
    bool verifica;//esta varible es para comprobar si la entidad tiene o no atributos
    Entidad nuevaent;//auxiliar que utilizaremos para la nueva entidad

    cout<<"\nEntidad a modificar: ";
    cin>>vieja;//nombre de la entidad que el usuario quiere modificar

    if(buscaEntidad(vieja) != -1)//si la entidad existe entonces
    {

        nuevaent = capturaEntidad();//el auxiliar tomara los datos de la nueva entidad
        verifica=verificaentidad(vieja);//aqui comprobamos si la entida tiene atributos
        if(verifica!=false) //En caso de tener no se puede modificar
        {
           if(buscaEntidad(nuevaent.nombre) == -1)//si la nueva entidad no existe entonces
            {
                pos = eliminaEntidad(vieja);//la posicion se vuelve al lugar donde estaba la entidad vieja
                rescribeEntidad(pos,nuevaent);//se reescribe la nueva entidad donde estaba la entidad vieja
                insertaEntidad(pos,nuevaent);//se inserta la nueva entidad
                res=true;//la entidad fue modificada exitosamente
            }
            else//se repite la nueva entidad
                cout<<"\n****Esa entidad ya existe, no se puede registrar****\n";
        }else//la entidad ya tiene atributos
            cout<<"\n****No se puede modificar esta entidad****\n";
    }else//la entidad vieja no existe
        cout<<"\n****La entidad no existe****\n";
    return res;//se regresa si la entidad se logro modificar

}

//ATRIBUTOS
//METODOS CAPTURAATRIBUTOS, LEEATRIBUTOS, CONSULTAATRIBUTOS
Atributo CDiccionario::capturaAtributo()//en esta funcion capturaremos los datos del atributo y regresaremos estos datos
{
	int tam;//el valor del tamaño del atributo
	Atributo nuevo;//auxiliar de atributo donde guardaremos los datos nombre, si es clave, tipo y su tamaño

	cout<<"\nColoca el nombre del atributo: ";
	cin>>nuevo.nombre;//pedimos el nombre del atributo
	nuevo.sig=-1;
	cout<<"\nColoca si el atributo es clave Y/N: ";
	cin>>nuevo.esclave;//si el atributo es clave
       do
        {
            cout<<"Coloca el tipo de dato: "
                    <<"\n1. Char"
                    <<"\n2. Int"
                    <<"\n3. Float"
                    <<"\n4. Long"
                    <<"\n5. Double\n";

            scanf("\n%d",&(nuevo.tipo));//el tipo del atributo

        }while(nuevo.tipo!=5 && nuevo.tipo!=1  && nuevo.tipo!=2 && nuevo.tipo!=3 && nuevo.tipo!=4);

	switch(nuevo.tipo){//aqui verificamos de que tipo es nuestro atributo para de esta manera guardar su tamaño
		case 1://si el tipo es char
			cout<<"Coloca el tamano de la cadena: ";
			cin>>tam;//pedimos el tamaño de la cadena del atributo
			nuevo.tamano = tam*sizeof(char);//se guarda el tamaño de un char
			break;
		case 2:
			nuevo.tamano = sizeof(int);//si el tipo es un entero
			break;
		case 3:
			nuevo.tamano = sizeof(float);//si el tipo es un float
			break;
		case 4:
			nuevo.tamano = sizeof(long);//si el tipo es double
			break;
		case 5:
			nuevo.tamano = sizeof(double);//si el tipo es long
			break;
	}

	return nuevo;//retorna los datos del nuevo atributo
}

Atributo CDiccionario::leeAtributo(long posAtr)//esta funcion lee los atributo del archivo dependiendo de la posición que pasamos
{
	Atributo atr;//utilizamos un auxiliar atr

	fseek(arch,posAtr,SEEK_SET);//Sitúa el puntero del archivo en la posición indicada
	fread(&atr,sizeof(Atributo),1,arch);//Esta función permite leer datos de un archivo donde lo apuntamos
    return atr;//regresa los datos encontrados
}

void CDiccionario::consultaAtributos(Entidad ent)//esta funcion nos permite ver los atributos de una entidad
{
    long cab;//las variables donde guardaremos
    Atributo atri;//el auxiliar del atributo donde guardaremos los datos
    cab=ent.atr;//empezamos desde la cabecera de los atributos
        while(cab!=-1)//esto se repite hasta que la entidad no tenga atributos
        {
            atri=leeAtributo(cab);//leemos el atributo de donde se encuentra la cabecera y guardamos los datos
            cout<<"\nNombre: "<<atri.nombre <<"/    ";//imprimos el nombre del atributo
            cout<<"Es clave: "<<atri.esclave <<"/   ";
            cout<<"Tamaño: "<<atri.tamano <<"/  ";
            switch(atri.tipo)
            {
            case 1:
                cout<<"Tipo: Char\n";
                break;
            case 2:
                cout<<"Tipo: Int\n";
                break;
            case 3:
                cout<<"Tipo: Float\n";
                break;
            case 4:
                cout<<"Tipo: Long\n";
                break;
            case 5:
                cout<<"Tipo: Double\n";
                break;
            }

            cab=atri.sig;//y la cabecera la desplazamos al sig atributo para seguir imprimiendo los siguientes
        }

}

//METODOS ESCRIBEATRIBUTO, BUSCAATRIBUTO
long CDiccionario::escribeAtributo(Atributo atr)//en esta funcion escribimos escribimos los datos del atributo dentro del archivo y regresamos la posicion de donde los insertamos
{
	long pos;//en esta variable guardaremos la posicion donde se guardo el atributo
	fseek(arch, 0, SEEK_END);//El fseek es para situar el puntero en el archivo
	pos = ftell(arch);//retorna la posición de donde tenemos el puntero
	fwrite(&atr, sizeof(Atributo), 1, arch);//escribimos los datos dentro de la posicón ya indicada
	return pos;//regresamos la posición donde guardamos los datos
}

long CDiccionario::buscaAtributo(char atrnombre[30], char entnombre[30])// en esta funcion buscamos un atributo a partir del nombre de este y del nombre de la entidad donde se encuentra y es de tipo long ya que regresa donde encontramos el atributo
{
	Atributo atrAux;//tenemos un auxiliar de atributo donde guardaremos los datos
	Entidad entAux;//tenemos un auxiliar de entidad donde guardaremos los datos
	long res, cabatr;//aqui guardaremos las posiciones

	res = buscaEntidad(entnombre);//buscamos la entidad para verificar si esta existe
	if(res != -1)//si la entidad existe
    {
        entAux = leeEntidad(res);//el auxiliar de entidad guarda los datos de la entidad donde se guarda el atributo
		cabatr = entAux.atr;//la cabecera guarda la posicion del primer atributo que tiene esa entidad
		while(cabatr != -1)//esta se va a hacer hasta que ya no existan atributos
		{
			atrAux = leeAtributo(cabatr);//el auxiliar de atributos lee los datos del atributo donde se encuentra la cabecera
			if(strcmpi(atrAux.nombre, atrnombre) == 0)//comparamos si el nombre del atributo de la cabecera es el mismo al atributo que buscamos
			{
				return cabatr;//si es el mismo nombre de atributo entonces regresa la posicion donde lo encontro (la cabecera actual)
			}
			cabatr = atrAux.sig;//si no lo encuentra entonces la cabecera se recorre al siguiente atributo y asi seguir comparando
		}
	}

	return -1;//si es que no la encuentra significa que el atributo no existe
}

//METODOS ALTAATRIBUTO, INSERTAATRIBUTO
bool CDiccionario::altaAtributo()//en esta funcion es donde nosotros llamaremos a las funciones de inserta y regresa si se inserto de manera correcta o no el atributo
{
    Atributo atr;//tenemos un auxiliar atributo donde guardaremos los datos
    Entidad entReal;//tenemos un auxiliar entidad donde guardaremos los datos
    long posatr, posent;//en estas vatiables guardaremos las posicion del atributo y de la entidad
    bool resp = false, vercla=false, verbloq=true;//estas varibales son verificaciones por si se ingreso de manera correcta o si una entidad ya tiene un atributo clave
    char nombreEnt[30];//guardamos el nombre de la entidad donde guardaremos los atributos

    atr = capturaAtributo();//aqui capturamos los datos del atributo que desamos ingresar y los pasamos al auxiliar del atributo
   //ent = capturaEntidad();
    cout << "Ingrese el nombre de la entidad" << endl;//pedimos la entidad donde queremos guardar los atributos
    cin >> nombreEnt;
    posent = buscaEntidad(nombreEnt);//buscamos la entidad para verificar si esta existe y regresamos la posicion de donde la encontro

    if(posent!=-1)//si si existe la entidad y no tiene más de un atributo clave entonces
    {
        verbloq=verificaAtributo(nombreEnt);//verificamos que no existan bloques
        vercla=verificaclave(posent, atr);//verificamos que la entidad no tenga más de un atributo clave
        if(vercla!=true && verbloq!=false)
        {
           entReal = leeEntidad(posent);//se leen los datos de la entidad y se guardan en el auxiliar de la entidad
                if(buscaAtributo(atr.nombre, entReal.nombre) == -1)//aqui hacemos una comparacion para checar que el atributo no se repita
                {
                    posatr = escribeAtributo(atr);//escribiremos el atributo y regresamos la posicion donde la escribimos
                    insertaAtributo(posatr,posent,atr);//insertamos el atributo a partir de la posicion del atributo, entidad y el atributo
                    resp = true;//el atributo fue insertado de manera correcta
                }else
                    cout<<"\n****Error el atributo ya existe****\n" << endl;//en caso de que el atributo ya exista
        }
        else
        {
            cout<<"\n****Ya existe un atributo ESCLAVE o la entidad ya tiene bloques ****\n"<<endl;
        }

    }
    else
    {
        cout << "\n****Esa entidad no existe ****\n" << endl;//en caso de que ya haya un atributo clave o la entidad no exista
    }
    return resp;//regresamo si el atributo fue insertado de manera correcta
}

bool CDiccionario::insertaAtributo(long posatr, long posEnt, Atributo atr)//regresamos un valor boleono de si se inserto correctamente el atributo, necesita la posicion del atributo, el entero y los datos del atributo
{
	bool res;// declaramos las variables la variable booleana es para ver si se inserto de manera correcta el atributo
	long posAtrAnt; //aqui guardamos la posicion del atributo
	Atributo atrAux,atrAnt; //aqui tenemos dos auxiliares del atributo
	Entidad aux;//en este auxiliar guardamos los datos de la entidad

	//Lee la entidad que encontro en la direccion
	aux = leeEntidad(posEnt);//el auxiliar guarda los datos de la entidad donde insertaremos el atributo
	//4 casos
	//Caso 1: Lista vacia
	if(aux.atr == -1)
    {
		aux.atr = posatr;//la posicion del atributo de la entidad tendra el valor de la posicion del atributo
        rescribeEntidad(posEnt, aux);//reescribimos el atributo en esa posicion
		res = true;//el atributo fue insertado de manera correcta en la entidad
	}else
	{
		atrAux = leeAtributo(aux.atr);//el auxiliar de atributo tendra los valores del atributo que queremos insertar
		//Caso 2: Al inicio con datos
		if(strcmpi(atr.nombre, atrAux.nombre) < 0)//si el atributo que nosotros deseamos eliminar va antes del atributo que se encuentra en cabecera
		{
			atr.sig = aux.atr;//el apuntador al atributo sig se ligara al atributo en el que ahora se encuentra la entidad
			aux.atr = posatr;//el auxiliar de la entidad donde se encuentra la posicion del atributo
			rescribeAtributo(posatr, atr);//reescribimos el atributo dentro de la posicion
			rescribeEntidad(posEnt, aux);//tambien volvemos a escribir la entidad ya que la posiciondel atributo cambio
			res = true;//el atributo se inserto de manera correcta
		}else//si el atributo que nosotros deseamos insertar va despues del atributo que se encuentra en cabecera
		{
			atrAnt = atrAux;//el auxiliar del atributo anterior toma los datos del atributo que se encuentra en la cabecera de la entidad
			posAtrAnt = aux.atr;//posicion del atributo anterior toma la posicion del atributo que se encuentra en la entidad
			while(aux.atr != -1 && strcmpi(atr.nombre, atrAux.nombre) > 0)//esto se va a repetir mientras todavia existan atributos y se va comparando el atributo con los demas
				{
                    atrAux = leeAtributo(aux.atr);//el atributo auxiliar toma los datos del atributo de la entidad
                    atrAnt = atrAux;//el auxiliar atributo anterior toma los datos del auxiliar atributo
                    posAtrAnt = aux.atr;//la posicion del atributo anterior toma la posicion del atributo de la entidad
                    aux.atr = atrAux.sig;//el auxiliar de la entidad donde se encuentra la posicion del atributo se desplaza al siguiente
                    if(aux.atr != -1)//si el auxiliar de la entidad donde estan los atributos no se encuenetra vacia
                        {
                          atrAux = leeAtributo(aux.atr);//el atributo auxiliar toma los datos que se encuentra en la entidad
                        }
                }
			//caso 3: Al final
			if(aux.atr == -1)//si el auxiliar de la entidad donde se guardan los atributos se encuentra vacia
                {
                    atrAnt.sig = posatr;//el auxiliar sig del atributo anterior toma la posicion del atributo
                    rescribeAtributo(posAtrAnt, atrAnt);//se reescribe el atributo
                    res = true;//el atributo se inserto de manera exitosa
                }
			else//Caso 4: En medio
                {
				atr.sig = atrAnt.sig;//el atributo siguiente toma la posicion del apuntador siguiente del atributo anterior
				atrAnt.sig = posatr;//y la posicion del atributo anterior siguien toma la posición del nuevo atributo
				rescribeAtributo(posatr, atr);//se reescribe el nuevo atributo en la posicion que le corresponde
				rescribeAtributo(posAtrAnt, atrAnt);//se reescribe el atributo anterior en su posicion anterior
				res = true;//el atributo se inserto de manera correcta
                }
            }
        }
	return res;//se regresa la bandera

}

//METODOS RESCIRBEATRIBUTO, ELIMINAATR(BAJA ATRIBUTO0), ELIMNINA ATRIBUTO
void CDiccionario::rescribeAtributo(long pos, Atributo atr)//en esta funcion volvemos a reescribir un  atributo diferente dentro del archivo, ingresamos la posicion donde lo queremos reescribir y los datos del atributo
{
    fseek(arch, pos, SEEK_SET);//aqui ponemos el apuntador en la posicion donde reescribiremos el atributo
    fwrite(&atr, sizeof(Atributo), 1, arch);//en esta parte escribimos el atributo
}

void CDiccionario::eliminaAtr()//en esta funcion ingresamos los datos del atributo que vamos a usar para llamar a la funcion eliminar
{
    long posEnt,posAtr;//en estas varibles guardaremos las posiciones de la entidad y del atributo
    Entidad aux;//aqui guardaremos el auxiliar de la entidad junto con sus datos
    Atributo aux2;//en este auxiliar guardaremos los datos del atributo

    cout<<"\nColoca la entidad donde se encuentra el atributo: ";
    fflush(stdin);
    cin>>aux.nombre;//pedimos el nombre de la entidad donde se encuentra el atributo

    posEnt=buscaEntidad(aux.nombre);//buscamos si la entidad existe
    if(posEnt!=-1)//en caso de que exista
    {

        if(verificaAtributo(aux.nombre)!=false)
        {
            cout<<"\nColoca el nombre del atributo: ";
            fflush(stdin);
            cin>>aux2.nombre;//guardamos el nombre del atributo
            aux = leeEntidad(posEnt);//guardamos en el auxiliar los datos de la entidad ya encontrada
            posAtr=buscaAtributo(aux2.nombre,aux.nombre);//buscamos si es que el atributo existe dentro de la entidad

            if(posAtr!=-1)//si existe el atributo
            {
                EliminaAtributo(aux,posEnt,aux2.nombre);//llamamos a la funcion que eliminara el atributo
            }
            else//el atriburo que queremos eliminar no existe
            {
                cout<<"\n****ERROR: El Atributo no existe****\n";
            }
        }
        else
        {
            cout<<"\n****ERROR: existen bloques en la entidad****\n";
        }
    }else//la entidad que buscamos no existe
    {
        cout<<"\n****ERROR: La entidad no existe****\n";
    }
}

long CDiccionario::EliminaAtributo(Entidad entAux,long pos,char *nombre)//esta funcion desenlaza el atributo  que queremos eliminar y necesita del auxiliar entidad, posicion de la entidad donde se encuentra el atributo y su nombre
{
    long  cabAtr,posAtrAnt;//en estas varibales guardaremos la cabecera del atributo y la posicion del atributo anterior
    Atributo atrAux,atrAnt;//guardamos los datos dentro de las auxiliares del atributo, y el atributo anterior

            cabAtr=entAux.atr;//cabecera del atributo tiene la poscion del primer atributo dentro de la entidad
            atrAux=leeAtributo(cabAtr);//el auxiliar del atributo obtiene los datos del atributo de la cabecera
            //caso 1 el atributo se encuentra al inicio
            if(strcmpi(nombre,atrAux.nombre)==0)//caso1 donde el atributo se encuentra en el inicio
            {
                entAux.atr=atrAux.sig;//el atributo que se encuentra en la cabecera pasa a ser el siguiente
                rescribeEntidad(pos,entAux);//se reescribe el atributo
            }
            else//caso 2 en medio
            {
                while(cabAtr != -1 && strcmpi(nombre, atrAux.nombre) != 0)//mientas la cabecera no sea nula y el nombre del atributo a eliminar sea igual al atributo actual de la entidad
                {
                    //atrAux = leeAtributo(cabAtr);//se lee el atributo y se regresan sus datos en el atributo auxiliar
                    atrAnt = atrAux;//el auxiliar del atributo anterior toma los datos del atributo auxiliar
                    posAtrAnt = cabAtr;//la posición del atributo anterior toma la posicion de la cabecera para guardar la posicion
                    cabAtr = atrAux.sig;//la cabecera se desplaza al atributo siguiente
                    if(cabAtr != -1)//esto se hace hasta que la entidad ya no tenga atributos
                    {
                        atrAux = leeAtributo(cabAtr);//se lee el atributo y se guardan sus datos en el atributo auxiliar
                    }

                }//caso 3 al final
                if(strcmpi(nombre,atrAux.nombre)==0)// si el nombre del atributo a eliminar sea igual al atributo actual de la entidad
                {
                    atrAnt.sig = atrAux.sig;//el apuntador a siguiente del atributo anterior tiene la posición del auxiliar atributo siguiente
                }
                rescribeAtributo(posAtrAnt, atrAnt);//se reescribe el atributo anterior en su posicion
            }
             return cabAtr;
}

bool CDiccionario::modificaAtributo()//esta funcion nos regresa un valor booleano ya que regresa si se modifico de manera correcta el atributo o no
{
    Entidad ent;//tenemos un auxiliar de entidad
    Atributo atr,nvoAtr;//obtenemos 2 auxiliares de atributo y del nuevo atributo
    long pos,posNvo,posEnt;//tenemos las variables de la posicion del viejo atributo, nuevo atributo y de la enitdad
    bool res=false, verificacion;//las variables booleanas de la bandera que en su inicio es falsa ya que aun no se modifica y la verificacion
    cout<<"Nombre de la entidad donde se encuentra el atributo: ";
    cin>>ent.nombre;//pedimos el nombre de la entidad donde se encuentran los atributos
    verificacion=verificaAtributo(ent.nombre);//verificamos que el atributo no tenga bloques de lo contrario no se puede modificar
    if(verificacion!=false)//si no se encontraron bloques
    {
        cout<<"Nombre del atributo a modificar: ";
        cin>>atr.nombre;//capturamos el atributo que deseamos modificar
        pos=buscaAtributo(atr.nombre,ent.nombre);//buscamos si el atributo que deseamos modificar existe y guardamos su posición
            if(pos!=-1) //la encontro
            {
                nvoAtr=capturaAtributo();//capturamos los datos del nuevo atributo
                posNvo=buscaAtributo(ent.nombre,nvoAtr.nombre); //verificar que no exista
                if(posNvo==-1) //verifica que no existe el atributo a modificar
                {
                    posEnt=buscaEntidad(ent.nombre);//guardamos la posicion de la entidad donde se encuentra el atributo a modificar
                    ent=leeEntidad(posEnt);//guardamos los datos de la entidad
                    pos=EliminaAtributo(ent,posEnt,atr.nombre);// desenlaza los demas atributos del viejo atributo
                    rescribeAtributo(pos,nvoAtr);//reescribimos el nuevo atributo en la posicion del viejo atributo
                    insertaAtributo(pos,posEnt,nvoAtr); //enlazamos el nuevo atributo
                    res=true;//se modifico de manera correcta
                }
                else//ya exite un atributo llamado igual que otro
                    cout<<"\n***No se modifico por que ya existe el atributo en la entidad.***\n";
            }
            else//no se encontro el archivo
            {
                cout<<"\n****No se encuentra en el archivo.****\n";
            }
    }
    else//la entidad ya contiene bloques
    {
         cout<<"\n****La entidad ya tiene bloques****\n";
    }
    return res;
}

//BLOQUES
//METODOS CAPTURABLOQUE, LEEBLOQUE, CONSULTABLOQUE
void *CDiccionario::capturaBloque(Atributo *arrAtr, long tamBloque)//en esta funcion capturamos los datos de un bloque y los regresamos en un void * y usamos el arreglo de atributos y el tamaño del bloque
{
    void *bloque=NULL;//obtenemos un apuntador a void ya que no sabemos que tipos de datos vamos a guardar
    char *texto=NULL;
    int i,contcarac;//tenemos una variable para recorrer el for
    long pos=0;//tenemos una variable pos para guardar la posicion
    double doub;
    bloque = malloc(tamBloque);//guardamos en la memoria dinamica
    *(long *)((char *)bloque) = -1;//igualamos la longitud total de la cadena a -1 ya que aun no guardamos los datos
    pos=sizeof(long);//la posicion toma el tamaño del tipo long
    for(i=0; pos < tamBloque; i++)//esto se va a ir haciendo mientas la posicion sea menor al tamaño del bloque
    {
        switch((arrAtr[i]).tipo)//en este switch nosotros verificamos que tipo es el atributo
        {
            case 1:
                fflush(stdin);//realiza una limpieza del buffer de entrada para asegurar que se lee correctamente el dato
                texto=(char*)malloc(sizeof(char)*(arrAtr[i].tamano+1));
                cout<<"\nColoca la cadena correspondiente a la columna '"<<(arrAtr[i]).nombre<<"': ";
                cin.getline(texto,arrAtr[i].tamano+1);//guardamos la cadena sumando el tamaño del bloque más su posicion
                fflush(stdin);//realiza una limpieza del buffer de entrada para asegurar que se lee correctamente el dato
                contcarac=strlen(texto);
                texto[contcarac]='\0';

                    strcpy(((char*)(char*)bloque+pos),texto);
                    free(texto);



                break;
            case 2:
                cout<<"\nColoca el valor entero correspondiente a la columna '"<<(arrAtr[i]).nombre<<"': ";
                cin>>(*((int *)((char *)bloque+pos)));//guardamos el entero sumando el tamaño del bloque más su posicion
                break;
            case 3:
                cout<<"\nColoca el valor flotante correspondiente a la columna '"<<(arrAtr[i]).nombre<<"': ";
                cin>>(*((float *)((char *)bloque+pos)));//guardamos el flotante sumando el tamaño del bloque más su posicion
                break;
            case 4:
                cout<<"\nColoca el valor de tipo long correspondiente a la columna '"<<(arrAtr[i]).nombre<<"': ";
                cin>>(*((long *)((char *)bloque+pos)));//guardamos el long sumando el tamaño del bloque más su posicion
                break;
            case 5:
                cout<<"\nColoca el valor de tipo double correspondiente a la columna '"<<(arrAtr[i]).nombre<<"': ";
                cin>>doub;
                *((double *)((char *)bloque + pos)) = doub;//guardamos el double sumando el tamaño del bloque más su posicion

                break;
        }
        pos += (arrAtr[i]).tamano;
    }
    return bloque;//regresamos el bloque
}

void *CDiccionario::leeBloque(long posBlo, long tamBloque)//en esta funcion leemos el bloque a partir del archivo y regresamos sus datos, es de tipo void apuntador ya que no sabemos que tipo es y ahcemos uso de la posicion del bloque y su tamaño
{
    void *bloque;//la variable es tipo void apunador ya que no sabemos que tipo son los datos del archivo
    bloque = malloc(tamBloque);//donde se guarda la memoria dinamica
    fseek(arch,posBlo,SEEK_SET);//aqui ponemos el apuntador en la posicion donde leeremos el bloque
    fread(bloque,tamBloque,1,arch);//leemos los elementos del archivo donde colocamos el apuntador
    return(bloque);//regresamos los datos que leemos
}

void CDiccionario::consultaBloque()//en esta funcion imprimimos los bloques de la entidad
{
    void *bloque=NULL;//la variable es tipo void apunador ya que no sabemos que tipo son los datos
    Entidad ent;//tenemos un auxiliar de entidad
    Atributo *arrAtr;//tenemos un auxiliar tipo apuntador de los atributos
    char nomEnt[30];//donde guardamos el nombre de la entidad de los bloques que vamos a imprimir
    char* texto=NULL;
    int i,contcarac;//esta variable es para poner los datos del if
    int cont=1;//contador del numero de bloque

    long posEnt, cabBlo, pos,tamBloque;//en estas variables guardamos la posicion de la entidad, cabecera bloque, posicion del bloque, tamaño del bloque
    cout<<"Coloca el nombre de la entidad: ";
    cin>>nomEnt;////guardamos el nombre de la entidad donde se encuentran los bloques
    posEnt = buscaEntidad(nomEnt);//buscamos la entdad y regresamos su posicion
    if(posEnt != -1)//Si la entidad existe
    {
        ent = leeEntidad(posEnt);//leemos la entidad y regresamos sus datos
        cabBlo = ent.bloque;//la cabecera bloque toma la posición del primer bloque de la entidad
        if(cabBlo != -1)//Si existe al menos un bloque
        {
            cout<<"\n====BLOQUES====\n";
            arrAtr = getAtributos(ent,&tamBloque);//se consiguen los atributos de la entidad
            while(cabBlo != -1)//esto se hace hasta que ya no exitan bloques
            {

                bloque = leeBloque(cabBlo, tamBloque);//leemos los bloques y los guardamos
                pos=sizeof(long);//la posicion toma el tamaño del long
                cout<<"\n--->Bloque"<<"["<<cont<<"]\n";
                for(i=0; pos<tamBloque; i++)//esto se hace mientras la posicion sea menor al tamaño del bloque
                {

                    switch((arrAtr[i]).tipo)//esto es para verificar del tipo de atributo al que esta asignado cada bloque
                    {
                        case 1:
                            cout<<">>Tipo: Char / Tamaño: "<<arrAtr[i].tamano<<" / Es clave: "<<arrAtr[i].esclave<<" / Bloque: ";
                            texto=(char*)malloc(sizeof(char)*(arrAtr[i].tamano));
                             strcpy(texto,((char*)((char*)bloque+pos)));
                            contcarac=strlen(texto);
                            texto[contcarac]='\0';
                            cout<<(texto)<<"\n";//se imprime el bloque de tipo char
                            free(texto);
                            break;
                        case 2:
                            cout<<">>Tipo: Int / Tamaño: "<<arrAtr[i].tamano<<" / Es clave: "<<arrAtr[i].esclave<<" / Bloque: ";
                            cout<<*((int *)((char *)bloque+pos))<<"\n";//se imprime el bloque de tipo entero
                            break;
                        case 3:
                            cout<<">>Tipo: Float / Tamaño: "<<arrAtr[i].tamano<<" / Es clave: "<<arrAtr[i].esclave<<" / Bloque: ";
                            cout<<*((float *)((char *)bloque+pos))<<"\n";//se imprime el bloque de tipo float
                            break;
                        case 4:
                            cout<<">>Tipo: Long / Tamaño: "<<arrAtr[i].tamano<<" / Es clave: "<<arrAtr[i].esclave<<" / Bloque: ";
                            cout<<*((long *)((char *)bloque+pos))<<"\n";//se imprime el bloque de tipo long
                            break;
                        case 5:
                            cout<<">>Tipo: Double / Tamaño: "<<arrAtr[i].tamano<<" / Es clave: "<<arrAtr[i].esclave<<" / Bloque: ";
                            cout<<*((double *)((char *)bloque + pos))<<"\n";//se imprime el bloque de tipo double
                            break;
                    }
                    pos += (arrAtr[i]).tamano;//la posicion se incrementa a partir del tamaño del tipo de atributo
                }
                cabBlo = *(long *)((char *)bloque+0);//la cabecera bloque incrementa con el tamaño del bloque
                cont++;//aumenta el contador para el numero de bloque
            }
        }
        else
        {
            cout<<"\n****NO EXISTE NINGUN BLOQUE EN LA ENTIDAD****\n";
        }
    }
}

//METODOS BUSCABLOQUE, ESCRIBE, REESCRIBEBLOQUE
long CDiccionario::buscaBloque(Entidad ent,Atributo *arrAtr, void* bloque, long tamBloque)//esta funcion nos ayuda a buscar un bloque y regresa su posicion en la que lo encontro, se pasan los parametros, la entidad, atributo, el bloque y su tamaño
{
    long cabBloque;//utilizamos una posicion de la cabecera de los bloques
    void *auxBloque; //auxiliar para los datos del bloque
    cabBloque = ent.bloque;//la cabecera toma la posicion del primer bloque de la entidad
    while(cabBloque != -1)//mientras todavia existan bloques
    {
        auxBloque = leeBloque(cabBloque,tamBloque);//los datos de los bloques se guardan en el auxiliar del bloque
        if(comparaBloques(arrAtr[0],bloque,auxBloque) == 0)//si al bloque de determinado atributo es igual al bloque que nosotros ingresamos en los parametros son iguales
        {
            return cabBloque;//se regresa la posicion donde lo encontro
        }
        cabBloque = *(long *)((char *)auxBloque);//la cabecera toma la posicion del siguiente bloque
    }
    return -1;//regresa -1 si no lo entcontro
}

long CDiccionario::escribeBloque(void *bloque, long tamBloque)//esta funcion escribe el bloque dentro del archivo y regresa la posiciondonde donde lo escribio, se da como parametro el bloque y su tamaño
{
    long pos;//en esta variable guardamos la posicion donde se escribio el bloque
    fseek(arch, 0, SEEK_END);//es el puntero del archivo
    pos = ftell(arch);//retorna la posicion del puntero
    fwrite(bloque, tamBloque, 1, arch);//escribimos dentro del archivo los datos del bloque
    return pos;//regresamos la posicion donde fue escrito dentro edl archivo
}

void CDiccionario::reescribeBloque(long pos, void *bloque, long tamBloque)//esta funcion reescribe el bloque dentro del archivo utiliza como parametro la posicion del bloque, los datos del bloque y tamaño del bloque
{
    fseek(arch, pos, SEEK_SET);//es el puntero del archivo en la posicion ya dada
    fwrite(bloque, tamBloque, 1, arch);//escribimos dentro del archivo
}

//METODOS GETATRIBUTOS, COMPARABLOQUES
Atributo* CDiccionario::getAtributos(Entidad ent, long *tamBloque)//en esta funcion nosotros obtenemos los tipos de datos de los atributos y de parametros utiliza la entidad y el tamaño del bloque que al mismo tiempo este se modificara
{
    Atributo Aux_atributo;//obtenoemos un auxiliar atributo
    Atributo *arreglo_Atributos;//tenemos un auxiliar tipo apuntador del atributo para el arreglo de este mismo
    long cabecera_atributos;//en esta varibale guardaaremos la posicion de la cabecera atributo
    long contador_Atributos = 0;//en esta variable guardaremos la cantidad de atributos que se encuentran en la entidad
    long contador_bloques = 1;//en esta variable asignada en 1, sera la posicion a partir donde se guardaran los atributos que son NO ESCLAVE en el arreglo
    cabecera_atributos = ent.atr;//la cabecera toma la posicion del primer atributo de la entidad
    //Contabiliza el numero de atributos de la entidad
    while(cabecera_atributos != -1)//mientras existan atributos
    {
        contador_Atributos++;//el contador se le suma uno
        Aux_atributo = leeAtributo(cabecera_atributos);//se lee el atributo de la posicion actual y regresa sus datos en el auxiliar atributo
        cabecera_atributos = Aux_atributo.sig;//la cabecera desplaza su posicion al siguiente atributo
    }
    //Reserva la memoria para el arreglo dinamico de atributos
    arreglo_Atributos = (Atributo *)malloc(sizeof(Atributo)*contador_Atributos);
    //Verifica si el arreglo dinamico de atributos reservo la memoria
    if(arreglo_Atributos == NULL)
    {
        cout<<"\n****MEMORIA NO RESERVADA****\n";
        exit(0);
    }
    //Leemos nuevamente la cabecera de atributos de la entidad
    cabecera_atributos = ent.atr;
    //Recorremos todos los atributos para insertarlos en el arreglo dinamico de atributos
    while(cabecera_atributos != -1)
    {
        //Leemos el atributo guardandolo en una variable de tipo atributo
        //Se usara para guardar cada dato en el arreglo
        Aux_atributo = leeAtributo(cabecera_atributos);
        //Verificamos si el atributo es ESCLAVE
        if(Aux_atributo.esclave == 'Y' || Aux_atributo.esclave == 'y')
        {
            (arreglo_Atributos[0]).esclave = 'Y'; //INSERTA AL INICIO EL ATRIBUTO ESCLAVE
            (arreglo_Atributos[0]).tipo = Aux_atributo.tipo;//se guarda el tipo de dato del atributo
            strcpy(arreglo_Atributos[0].nombre, Aux_atributo.nombre);//se guarda el nombre del atributo
            (arreglo_Atributos[0]).tamano = Aux_atributo.tamano;//se guarda su tamaño
            (arreglo_Atributos[0]).sig = Aux_atributo.sig;//guarda la posicion siguiente al que este apunta
            *tamBloque = *tamBloque + Aux_atributo.tamano;//el tamaño del bloque se suma a si mismo más el tamaño del atributo
        }
        //Si NO es ESCLAVE el atributo se ingresara despues del indice 0 para guardar en la memoria el que sí es
        else
        {
             //Los atributos que no sean ESCLAVE seran guardados despues del indice 0
            (arreglo_Atributos)[contador_bloques].esclave = 'N';//guardamos que no es clave
            (arreglo_Atributos)[contador_bloques].tipo = Aux_atributo.tipo;//guardamos el tipo de dato que es
            strcpy(arreglo_Atributos[contador_bloques].nombre, Aux_atributo.nombre);//guardamos el nombre del atributo
            (arreglo_Atributos)[contador_bloques].tamano = Aux_atributo.tamano;//guardamos el tamaño
            (arreglo_Atributos)[contador_bloques].sig = Aux_atributo.sig;//guarda la posicion siguiente al que este apunta
            *tamBloque = *tamBloque + Aux_atributo.tamano;//el tamaño del bloque se suma a si mismo más el tamaño del atributo
            contador_bloques += 1;//al contador de bloques se le suma uno ya que se guardo un atributo y se avanza al siguiente espacio de memoria

        }
        //La cabecera se cambia al siguiente atributo para leer el siguiente atributo de la entidad
        cabecera_atributos = Aux_atributo.sig;
    }
    //Se regresa por referencia la dimesion total del bloque leido + el tamano de un long para la direccion del siguiente bloque
    *tamBloque=*tamBloque+sizeof(long);
    return (arreglo_Atributos);//regresamos el arrelo de atributos
}

long CDiccionario::comparaBloques(Atributo esclave,void *bloque1, void *bloque2)//en esta funcion comparamos las claves de los bloques ya que estas no pueden repetirse o en caso de querer buscar un bloque de parametros ocupamos el auxiliar de atributo para verificar el tipo de dato de la clave y los dos bloques que vmaos a comparar
{
    char cadena_bloque1[80], cadena_bloque2[80];//en caso de que el tipo de la clave sea char

    int int_bloque1,int_bloque2;//en caso de que el tipo de la clave sea char
    float flotante_bloque1,flotante_bloque2;//en caso de que el tipo de la clave sea char
    long long_bloque1,long_bloque2;//en caso de que el tipo de la clave sea char
    double double_bloque1,double_bloque2;//en caso de que el tipo de la clave sea char

    switch(esclave.tipo)//verificamos de que tipo es la clave
    {
        //Tipo char
        case 1:
            strcpy(cadena_bloque1,(char*)((char*)bloque1+sizeof(long)));
            strcpy(cadena_bloque2,(char*)((char*)bloque2+sizeof(long)));
            //menor a 0 el primero es menor, igual a 0 ambos son iguales, mayor a 0 el segundo es mayor
            return strcmpi(cadena_bloque1,cadena_bloque2);
            break;
        //Tipo int
        case 2:
            int_bloque1 = *((int*)((char*)bloque1+sizeof(long)));
            int_bloque2 = *((int*)((char*)bloque2+sizeof(long)));
            //menor a 0 el primero es menor, igual a 0 ambos son iguales, mayor a 0 el segundo es mayor
            return int_bloque1-int_bloque2;
            break;
        //Tipo float
        case 3:
            flotante_bloque1 = *((float*)((char*)bloque1+sizeof(long)));
            flotante_bloque2 = *((float*)((char*)bloque2+sizeof(long)));
            //menor a 0 el primero es menor, igual a 0 ambos son iguales, mayor a 0 el segundo es mayor
            return flotante_bloque1-flotante_bloque2;
            break;
        //Tipo long
        case 4:
            long_bloque1 = *((long*)((char*)bloque1+sizeof(long)));
            long_bloque2 = *((long*)((char*)bloque2+sizeof(long)));
            //menor a 0 el primero es menor, igual a 0 ambos son iguales, mayor a 0 el segundo es mayor
            return long_bloque1-long_bloque2;
            break;
        //Tipo double
        case 5:
            double_bloque1 = *((double*)((char*)bloque1+sizeof(long)));
            double_bloque2 = *((double*)((char*)bloque2+sizeof(long)));
            //menor a 0 el primero es menor, igual a 0 ambos son iguales, mayor a 0 el segundo es mayor
            return double_bloque1-double_bloque2;
            break;
    }
}

//VERIFICACIONES
bool CDiccionario::verificaclave(long posent, Atributo atr)//en esta funcion verificamos que no exista más de una clave y utilizamos de parametros la posicion del entero y el arreglo de atributos
{
    long auxcab;//utilizamos una variable de tipo long para guardar la posicion de la cabecera
    bool reg=false;//la bandera es falsa ya que aun no se verifica
    Atributo auxatr;//utilizamos un auxiliar atributo
    Entidad ent;//utilizamos un auxiliar entidad
    ent=leeEntidad(posent);//leemos los datos de la entidad
    auxcab=ent.atr;//la cabecera toma el valor del primer atributo de la entidad

    while(auxcab!=-1)//esto se hace hasta que ya no exitan atributos
    {
        auxatr=leeAtributo(auxcab);//se leen los datos del atributo
        if(auxatr.esclave=='Y' || auxatr.esclave=='y')//si el atributo de la entidad actual es clave
        {
            if(atr.esclave=='Y' || atr.esclave=='y')//si el atributo que nosotro pasamos como atributo tambien es clave
            {
                reg=true;//ya existe un atributo clave
                return reg;//retornamos la bandera
            }
        }
        auxcab=auxatr.sig;//la posicion de la cabecera se recorre al siguiente atributo
    }
    return reg;//se regresa la bandera
}

bool CDiccionario::verificaentidad(char nombre[30])//verificamos que la entidad no tenga atributos en caso de querer eliminar o modificar
{
    bool entvac=true;//es la bandera que regresaremos
    long pos;//posicion de la entidad
    Entidad ent;//el auxiliar entidad
    pos=buscaEntidad(nombre);//se busca la entidad y se guarda la poscion donde la encontro
    ent=leeEntidad(pos);//lee los datos de la entidad
    if(ent.atr!=-1)//si la entidad no tiene ningun atributo
    {
        entvac=false;//la banderase cambia a false ya que SI encontro ningn atributo
    }
    return entvac;//se regresa la bandera
}

bool CDiccionario::verificaAtributo(char nombre[30])//en esta funcion verificamos que si se quiere modificar o eliminar un atributo la entidad no tenga bloques
{

    bool entvac=true;//la bandera que regresaremos
    long pos;//en esta variable guardamos la posicion
    Entidad ent;//el auxiliar de la entidad
    pos=buscaEntidad(nombre);//guardamos la posicion donde se encuentra el bloque
    ent=leeEntidad(pos);//leemos el bloque
    if(ent.bloque!=-1)//si la entidad bloque es falsa
    {
        entvac=false;//la bandera se vuelve falso ya que si existen bloque y no puede proceder a la modificación o eliminacion
    }
    return entvac;//regresamos la bandera
}

bool CDiccionario::verificaArchivo()//se verifica si hay un archivo abierto
{
    bool verificacion=false;//la bandera al inicio es falsa ya que aun no verificamos
    if(arch!=NULL)//si el archivo es diferente a nulo
        verificacion=true;//la badera se vuelve true ya que si hay un archivo abierto
    return verificacion;//se regresa la bandera
}

bool CDiccionario::verificasiclave(Entidad ent)//verificamos que exista un atributo clave para ingresar los bloques
{
     long auxcab;//guardamos la cabecera
    bool reg=false;//la bandera que regresaremos al inicio es falsa ya que no se a verificado
    Atributo auxatr;//auxiliar atributo
    auxcab=ent.atr;//la cabecera toma la posicion del primer atributo

    while(auxcab!=-1)//mientras existan atributos
    {
        auxatr=leeAtributo(auxcab);//se leen los atributos y se guardan en el auxiliar atributo
        if(auxatr.esclave=='Y' || auxatr.esclave=='y')//si el atributo actual es clave
        {
                reg=true;//la bandera se vuelve verdadera ya que la entidad si tiene un atributo clave
        }
        auxcab=auxatr.sig;//la cabecera se reccore al atributo siguiente
    }
    return reg;//se regresa la bandera

}

//METODOS ALTABLOQUE, INSERTABLOQUE
bool CDiccionario::AltaBloque()//en esta funcion nosotros damos de alta un bloque esta funcion regresara si se logro insertar de manera correcta o no
{
    Atributo *atr;//tenemos un puntador auxiliar atributo donde guardaremos los datos
    Entidad ent;//tenemos un auxiliar entidad donde guardaremos los datos
    long verbloque, posent,tambloq, posbloque;//en estas vatiables guardaremos las posicion del atributo y de la entidad
    bool res = false, vercla;//estas varibales son verificaciones por si se ingreso de manera correcta o si una entidad ya tiene un atributo clave
    char nombreEnt[30];//guardamos el nombre de la entidad donde guardaremos los atributos
    void* bloque;//donde guardamos los datos del bloque
    cout<<"\nIngrese el nombre de la entidad: ";
    cin>>nombreEnt;//ingresamos el nombre de la entidad donde se encuentra el bloque
    posent=buscaEntidad(nombreEnt);//buscamos si la entidad existe
    if(posent!=-1)//si la entidad existe
    {
        ent=leeEntidad(posent);//leemos la entidad y guardamos sus datos
        vercla=verificasiclave(ent);//verificamos si esta entidad tiene un atributo clave
        if(vercla==true)//si la entidad si tiene un atributo clave
        {
            atr=getAtributos(ent,&tambloq);//obtenemos los datos de los atributos y el tamaño del bloque

            bloque=capturaBloque(atr,tambloq);//capturamos los datos del bloque que queremos insertar
            verbloque=buscaBloque(ent,atr,bloque,tambloq);//verificamos que el bloque que queremos ingresar no repita la clave
            if(verbloque==-1)//si no repite clave
            {
               posbloque=escribeBloque(bloque,tambloq);//escribimos el bloque dentro del archivo y regresamos su posicion
               InsertaBloque(ent,atr,posbloque,posent,tambloq,bloque);//llamamos a la funcion inserta bloque y esta enlazara el bloque
               res=true;//el bloque se inserto de manera correcta

            }else//El bloque se repite
            {
                cout<<"\n**** ya existe ese bloque****\n";
            }

        }else//la entidad no cuenta con atributo clave
        {
          cout<<"\n****No existe un atributo clave****\n";
        }

    }else//no existe la entidad donde se quiere insertar el bloque
    {
        cout<<"\n**** No existe la entidad****\n";
    }

 return res;//regresa la bandera
}

bool CDiccionario::InsertaBloque(Entidad ent, Atributo *atr, long posbloq, long posent, long tambloq, void* bloque)//esta funcion se enlazara el nuevo bloque de parametros utilizamos la entidad, el arreglo de atributos, la posicion de la entidad, el tamaño del bloque junto con sus datos
{
    void *auxbloq, *antbloq;// en esta varibale guardaremos los datos de los bloque
    bool res=false;//la bandera que en su inicio es falsa ya que aun no se inserta el bloque
    long cabbloq,posantbloq;//en esta variables guardaremos la cabecera del bloque y la posicion del bloque anterior

    //caso 1 la entidad aun no cuenta con ningun bloque
    if(ent.bloque==-1)//si el apuntador a bloque de la entidad se encuentra vacia
    {
        ent.bloque=posbloq;//el apuntador a bloque de la entidad toma la posicion del bloque
        rescribeEntidad(posent, ent);//reescribimos la entidad
        res=true;//el bloque se inserta de manera correcta
    }
    else//si ya hay bloques dentro de la entidad
    {
        cabbloq=ent.bloque;//la cabecera toma la posicion del primer bloque que se encuentra en la entidad
        auxbloq=leeBloque(cabbloq,tambloq);//leemos el primer bloque de la entidad y regresamos su datos

        if(comparaBloques(atr[0],bloque,auxbloq)<0)//Caso 2: Al inicio con datos
        {
            *(long*)((char*)bloque)=ent.bloque;//el bloque siguiente es igual a la posicion del primer bloque de la entidad
            ent.bloque=posbloq;//la posicion del primer bloque de la entidad pasa a ser la posicion del nuevo bloque
            reescribeBloque(posbloq,bloque,tambloq);//reescrbimos el bloque en la nueva posicion
            rescribeEntidad(posent,ent);//reescribimos la entidad ya que el apuntador al primer bloque cambio
            res=true;//el bloque se inserto de manera correcta
        }
        else//se inserta depues del primer dato
        {
            antbloq=auxbloq;//el bloque anterior es igual al bloque que deseamos insertar
            posantbloq=cabbloq;//la poscion del bloque anterior es igual a la cabecera bloque
            while(cabbloq!=-1 && comparaBloques(atr[0],bloque,auxbloq)>0)//mientras existan bloques y al comparar el bloque y el bloque auxiliar se regregrese un valor menor a 0
        	{
                auxbloq = leeBloque(cabbloq,tambloq);//el bloque auxiliar es igual al bloque de la entidad
				antbloq = auxbloq;//el bloque anterior es igual al bloque entidad
				posantbloq = cabbloq;//la posicion del bloque anterior es igual a los bloques de la entidad
				cabbloq =  *(long*)((char*)auxbloq);//os bloques de la entidad es igual al bllque siguiente del auxiliar
				if(cabbloq != -1)//si los bloques de la entidad son deiferentes de -1
					auxbloq = leeBloque(cabbloq,tambloq);//el bloque auxiliar es igual al bloque de la entidad
        	}
        	if(cabbloq==-1)//caso 3: Al final
        	{
                *(long*)((char*)antbloq)=posbloq;//bloque siguiente del bloque anterior es igual a la poscion del bloque
                reescribeBloque(posantbloq,antbloq,tambloq);//se reescribe el bloque anterior
                res=true;//se inserto el bloque de manera correcta
        	}
        	else//Caso 4: En medio
        	{
                *(long*)((char*)bloque)= *(long*)((char*)antbloq);//el bloque siguiente es igual al bloque siguiente del bloque anterior
                *(long*)((char*)antbloq)=posbloq;//bloque siguien al bloque anterior es igual a la posicion del bloque
                reescribeBloque(posbloq,bloque,tambloq);//se reescribe el bloque
                reescribeBloque(posantbloq,antbloq,tambloq);//reescribimos el bloque anterior
                res=true;//se inserto el bloque de manera correcta
        	}
        }

    }
     return res;//regresamos la bandera
}

//METODOS CAPTURACLAVE, BAJABLOQUE, ELIMINABLOQUE, MODIFICABLOQUE
void* CDiccionario::capturaClave(Atributo atr,long tambloque, Entidad ent)//en este metodo capturamos la clave en los casos de eliminar o modificar un bloque ya que no sabemos de que tipo es la clave
{
    void* dato, *aux_bloque;//utilizamos las variables dato y aux bloque para guardar los datos del bloque
   int contcarac=0;
    char* texto=NULL;
    char* textobloque=NULL;
    long pos=sizeof(long);//esta variable servira para evitar leer la posicion del bloque siguiente de los bloques que se estaran leyendo.
    long cabBloque;//Se usara para desplazarnos a traves de los bloques que hay en la Entidad
        switch(atr.tipo)//Se aplica un switch para capturar el ESCLAVE que nos ayudara a encontrar el bloque que se eliminara o modificara, el tipo depende del tipo de dato del atributo que es ESCLAVE
        {
            //Caso 1: TIPO CHAR
            case 1:
                fflush(stdin);
                cout<<"\nIngresa la CADENA ESCLAVE del bloque: ";
                texto=(char*)malloc(sizeof(char)*(atr.tamano+1));
                cin.getline(texto,atr.tamano+1);//se imprime el bloque de tipo char
                fflush(stdin);//realiza una limpieza del buffer de entrada para asegurar que se lee correctamente el dato
                cabBloque = ent.bloque;//Asignamos la cabecera del bloque de la entidad a una variable para recorrer todos sus bloques
                textobloque=(char*)malloc(sizeof(char)*(atr.tamano));
                while(cabBloque != -1)//Mientras existan bloques seguira comparando hasta que lo encuentre o no exista
                {

                    aux_bloque = leeBloque(cabBloque, tambloque);//Se lee el primer bloque de la entidad

                    strncpy(textobloque,((char*)((char*)aux_bloque+pos)),atr.tamano);
                    //contcarac=strlen(textobloque);
                    //textobloque[contcarac]='\0';
                    cout<<"\n"<<(textobloque);
                    if(strcmpi((texto),(textobloque)) == 0)//Se compara si el dato que capturamos es igual al dato ESCLAVE del bloque
                        {
                            contcarac=(strcmpi((texto),((char*)(char*)aux_bloque+pos)));

                            cout<<"\n"<<contcarac<<"\n";
                          return aux_bloque;//Se regresa el bloque leido si lo encontramos
                        }

                    cabBloque= *((long *)((char*)aux_bloque));//la cabecera bloque avanza al siguiente si aun no encuentra el bloque
                }
                break;
            //Caso 2: TIPO INT
            case 2:
                cout<<"\nIngresa el ENTERO ESCLAVE del bloque: ";
                fflush(stdin);
                dato = malloc(sizeof(int));//Reservamos la memoria para poder capturar el ESCLAVE del bloque que queremos eliminar o modificar
                cin>>(*(int *)dato);//Capturamos el dato, se hace un cast al tipo de dato que es el atributo ESCLAVE
                cabBloque = ent.bloque;//Asignamos la cabecera del bloque de la entidad a una variable para recorrer todos sus bloques
                while(cabBloque != -1)//Mientras existan bloques seguira comparando hasta que lo encuentre o no exista
                {
                    aux_bloque = leeBloque(cabBloque, tambloque);//Se lee el primer bloque de la entidad
                    if(((*(int *)dato)-(*(int *)((char *)aux_bloque+pos)))==0)//Se compara si el dato que capturamos es igual al dato ESCLAVE del bloque
                        return aux_bloque;//Se regresa el bloque leido si lo encontramos
                    cabBloque= *(long *)((char*)aux_bloque);//la cabecera bloque avanza al siguiente si aun no encuentra el bloque
                }
                break;
            //Caso 3: TIPO FLOAT
            case 3:
                cout<<"\nIngresa el FLOTANTE ESCLAVE del bloque: ";
                fflush(stdin);
                dato = malloc(sizeof(float));//Reservamos la memoria para poder capturar el ESCLAVE del bloque que queremos eliminar o modificar
                cin>>(*(float *)dato);//Capturamos el dato, se hace un cast al tipo de dato que es el atributo ESCLAVE
                cabBloque = ent.bloque;//Asignamos la cabecera del bloque de la entidad a una variable para recorrer todos sus bloques
                while(cabBloque != -1)//Mientras existan bloques seguira comparando hasta que lo encuentre o no exista
                {
                    aux_bloque = leeBloque(cabBloque, tambloque);//Se lee el primer bloque de la entidad
                    if(((*(float *)dato)-(*(float *)((char *)aux_bloque+pos)))==0)//Se compara si el dato que capturamos es igual al dato ESCLAVE del bloque
                        return aux_bloque;//Se regresa el bloque leido si lo encontramos
                    cabBloque= *(long *)((char*)aux_bloque);//la cabecera bloque avanza al siguiente si aun no encuentra el bloque
                }
                break;
            //Caso 4: TIPO LONG
            case 4:
                cout<<"\nIngresa el LONG ESCLAVE del bloque: ";
                fflush(stdin);
                dato = malloc(sizeof(long));//Reservamos la memoria para poder capturar el ESCLAVE del bloque que queremos eliminar o modificar
                cin>>(*(long *)dato);//Capturamos el dato, se hace un cast al tipo de dato que es el atributo ESCLAVE
                cabBloque = ent.bloque;//Asignamos la cabecera del bloque de la entidad a una variable para recorrer todos sus bloques
                while(cabBloque != -1)//Mientras existan bloques seguira comparando hasta que lo encuentre o no exista
                {
                    aux_bloque = leeBloque(cabBloque, tambloque);//Se lee el primer bloque de la entidad
                    if(((*(long *)dato)-(*(long *)((char *)aux_bloque+pos))) == 0)//Se compara si el dato que capturamos es igual al dato ESCLAVE del bloque
                        return aux_bloque;//Se regresa el bloque leido si lo encontramos
                    cabBloque= *(long *)((char*)aux_bloque);//la cabecera bloque avanza al siguiente si aun no encuentra el bloque
                }
                break;
            //Caso 5: TIPO DOUBLE
            case 5:
                cout<<"\nIngresa el DOUBLE ESCLAVE del bloque: ";
                fflush(stdin);
                dato = malloc(sizeof(double));//Reservamos la memoria para poder capturar el ESCLAVE del bloque que queremos eliminar o modificar
                cin>>(*(double *)dato);//Capturamos el dato, se hace un cast al tipo de dato que es el atributo ESCLAVE
                cabBloque = ent.bloque;//Asignamos la cabecera del bloque de la entidad a una variable para recorrer todos sus bloques
                while(cabBloque != -1)//Mientras existan bloques seguira comparando hasta que lo encuentre o no exista
                {
                    aux_bloque = leeBloque(cabBloque, tambloque);//Se lee el primer bloque de la entidad
                    if(((*(double *)dato)-(*(double *)((char *)aux_bloque+pos))) == 0)//Se compara si el dato que capturamos es igual al dato ESCLAVE del bloque
                        return aux_bloque;//Se regresa el bloque leido si lo encontramos
                    cabBloque= *(long *)((char*)aux_bloque);//la cabecera bloque avanza al siguiente si aun no encuentra el bloque
                }
                break;
        }

}

bool CDiccionario::BajaBLoque()//Esta funcion nos regresa un valor booleano ya que nos dice si se elimino de manera correcta el bloque
{
    Atributo *atr;//tenemos un puntador auxiliar atributo donde guardaremos los datos
    Entidad ent;//tenemos un auxiliar entidad donde guardaremos los datos
    long  posent, posbloque, poselimina, tambloq;//en estas vatiables guardaremos las posicion del atributo y de la entidad
    bool res = false;//estas varibales son verificaciones por si se elimino de manera correcta o si una entidad ya tiene un atributo clave
    char nombreEnt[25];//guardamos el nombre de la entidad donde guardaremos los atributos
    void* clave;//en esta variable guardaremos los datos de la clave que deseamos eliminar
    cout<<"\nIngrese el nombre de la entidad: ";
    cin>>nombreEnt;//ingresamos el nombre de la entidad
    posent=buscaEntidad(nombreEnt);//buscamos is la entidad existe y regresamos la posicion donde lo encontro
    if(posent!=-1)//si la entidad existe
    {
        ent=leeEntidad(posent);//leemos los datos de la entidad
        atr=getAtributos(ent,&tambloq);//obtenemmos el atributo y el tamaño del bloque
        clave=capturaClave(atr[0],tambloq,ent);//capturamos la clave que deseamos eliminar
        posbloque=buscaBloque(ent,atr,clave,tambloq);//buscamos si la clave existe y regresamos su posicion
            if(posbloque!=-1)//si la clave existe
            {
                poselimina=EliminaBloque(ent,atr,posbloque,posent,tambloq,clave);//llamamos a la funcion elimina bloque y regresamos la posicion donde lo elimino
                res=true;//se eliminao de manera correcta el bloque

            }
            else//en caso de que le bloque que deseamos eliminar no existe
            {
                cout<<"\n****El bloque no existe****\n";
            }
    }
    else//en caso de que la entidad donde se encuntra el bloque no existe
    {
        cout<<"\n****La entidad no existe****\n";
    }
    return res;//regresamos la bandera


}

long CDiccionario::EliminaBloque(Entidad ent, Atributo *atr, long posbloq, long posent, long tambloq, void* clave)//la funcion elimina bloque desenlaza al bloquede los demas y regreesa la posicion, de parametros utilizamos la entidad, el arreglo de atributos la posicion de la entidad, la posicion del bloque, el tamaño del bloque y la clave del bloque que queremos eliminar
{
    long  cabbloq,posbloqant,cabbloqueelim;//utlizamos la posicion de la cabecera bloque y la posicion del bloque
    void *bloque1, *antbloq;//utilizamos las varibales para guardar los datos del bloque que deseamos eliminar y el bloque anterior
    cabbloq=ent.bloque;//la cabecera toma la posicion del primer bloque que se encuentra en la entidad
    if(cabbloq==posbloq)//si el bloque que deseamos eliminar es el primero de la entidad
    {
        ent.bloque=*(long*)((char*)clave);//la posicion del primer bloque de la entidad pasa a ser del siguiente bloque
        rescribeEntidad(posent,ent);//reescribimos la entidad ya que cmabio su cabecera
        return posbloq;//regresamos la posicion del bloque
    }
    else //el bloque se encuetra en medio
    {
        bloque1=leeBloque(cabbloq,tambloq);//leemos y regresamos el bloque dependiendo de la posicion del la cabecera
        while(cabbloq!=-1 && comparaBloques(atr[0],clave,bloque1)!=0)//mientras existan bloques y al comparar el nuevo bloque con el bloque sea Diferente de 0
        {
            posbloqant=cabbloq;//la posicion del bloque e leimos pasa a ser la posicion del bloque anterior
            //bloque1=leeBloque(*(long*)((char*)bloque1),tambloq);

            antbloq=bloque1;//el bloque que leimos pasa a ser el bloque anterior
            cabbloq=*(long*)((char*)bloque1);//la cabecera se desplaza al bloque siguiente
            if(cabbloq!=-1)//si la posicion del bloque siguiente es diefrente de -1
            {
               bloque1=leeBloque(cabbloq,tambloq);//leemos el bloque
            }
        }
        if(comparaBloques(atr[0],clave,bloque1)==0)//si al comparar el bloque que leimos y el nuevo bloque es igual a 0
        {
            cabbloqueelim=cabbloq;
            cabbloq=*(long*)((char*)bloque1);//aavanzamos la posicion de la cabecera al bloque siguiente
            *(long*)((char*)antbloq)=cabbloq;//enlazmamos el bloque anterior con el bloque que apunta el bloque eliminado
            reescribeBloque(posbloqant,antbloq,tambloq);//reescribimos el bloque
            return cabbloqueelim;//regresamos la posicion del bloque eliminado
        }
    }
}

bool CDiccionario::modifcaBloque()//este metodo nos permite modificar un bloque ya exitente y regresa un valor booleano si se modifico de manera correcta o incorrecta
{

    Atributo *atr;//tenemos un puntador auxiliar atributo donde guardaremos los datos
    Entidad ent;//tenemos un auxiliar entidad donde guardaremos los datos
    long  posent, posbloque, posnuevo, tambloq,poselimina;//en estas vatiables guardaremos las posicion del atributo y de la entidad
    bool res = false;//estas varibales son verificaciones por si se modifico de manera correcta o si una entidad ya tiene un atributo clave
    char nombreEnt[25];//guardamos el nombre de la entidad donde guardaremos los atributos
    void* clave, *bloque;//utilizamos esta varibales para guardar la clave del bloque que deseamos modificar y del bloque por el cual lo vamos a modificar
    cout<<"\nIngrese el nombre de la entidad: ";
    cin>>nombreEnt;//ingresamos el nombre de la entidad donde se encuentra el bloque
    posent=buscaEntidad(nombreEnt);//buscamos la entidad  y regresamos la posicion donde la encontro
    if(posent!=-1)//si la entidad existe
    {
        ent=leeEntidad(posent);//leemos la entidad
        atr=getAtributos(ent,&tambloq);//obtenemos el arreglo de atributos y el tamaño del bloque
        clave=capturaClave(atr[0],tambloq,ent);//capturamos la clave del bloque que deseamos modificar
        posbloque=buscaBloque(ent,atr,clave,tambloq);//buscamos si el bloque existe y regresamos u posicion
        if(posbloque!=-1)//si el bloque existe
        {
            bloque=capturaBloque(atr,tambloq);//caturamos el nuevo bloque por el que deseamos modificar el anterior
            posnuevo=buscaBloque(ent,atr,bloque,tambloq);//buscamos si el nuevo bloque no se repita con los demas
            if(posnuevo==-1 || comparaBloques(atr[0],bloque,clave)==0)//si el bloque no se repite o si su clave es igual a la del bloque que deseamos modificar
            {
                poselimina=EliminaBloque(ent,atr,posbloque,posent,tambloq,clave);//llamamos a la funcion elimina bloque y regresamos su posición
                reescribeBloque(poselimina,bloque,tambloq);//reescribimos el nuevo bloque en la posicion del bloque viejo
                if(poselimina==ent.bloque)//si la posicion donde se elimino el bloque es igual al primer bloque de la entidad
                {
                    ent=leeEntidad(posent);//tenemos que leer de nuevo la entidad ya que si se logro modificar dentro del archivo pero no dentro de la estructura de la entidad
                }
                InsertaBloque(ent,atr,posbloque,posent,tambloq,bloque);//llamamos a la funcion insertar que enlazara el nuevo bloque con los demas bloques
                res=true;//el bloque se logro modificar de manera correcta
            }
            else//si el bloque nuevo se repite con algun otro bloque
            {
                cout<<"\n****El bloque ya existe****\n";
            }

        }
        else//si el bloque que deseamos modificar no existe
            {
                cout<<"\n****El bloque no existe****\n";
            }
    }
    else//si la entidad donde se encuetra el bloque no existe
    {
        cout<<"\n****La entidad no existe****\n";
    }
    return res;//regresamos la bandera

}
void CDiccionario::ConsultaEntidad()//esta funcion nos imprime las entidades y los atributos dentro del archivo
{
    long cab;//posicion de la cabecera
    Entidad ent;//auxiliar de la entidad donde se encuentran los datos
    int i=0;
    cout << "=====Entidades y atributos en el archivo=====" << endl;

    cab=leeCabecera();//la cabecera se pone en la posicion de incio
    while(cab!=-1)//esto se va haciendo hasta que ya no haya mas entidades
    {
        i++;
        cout<<"\n Entidad "<<i<<"\n";
        ent=leeEntidad(cab);//regresa los datos de la posicion donde se encuentra la cabecera

        cout<<">>"<<ent.nombre <<"\n";//imprime el dato de la entidad
        consultaAtributos(ent);

        cab=ent.sig;//la cabecera se recorre para seguir imprimiendo las siguientes entidades
    }


}

void CDiccionario::Maximo()//Regresa el valor máximo de una clave de una entidad dada por el usuario
{
    Atributo *arrAtr;
    Entidad ent;
    long cabent,cabloque,tamBloque, pos=sizeof(long);
    char* texto;
    int i, contcarac=0;
    void *bloque=NULL;
    char nombrent[50];
    cout<<"Ingresa entidad: ";
    cin>>nombrent;
    cabent=buscaEntidad(nombrent);
    if(cabent!=-1)
    {

        ent=leeEntidad(cabent);
        cabloque=ent.bloque;
        if(cabloque!=-1)
        {
            arrAtr=getAtributos(ent,&tamBloque);
            while(cabloque!=-1)
             {
               bloque=leeBloque(cabloque,tamBloque);
               cabloque=*(long*)((char*) bloque);
             }
             cout<<"\n*******El registro maximo es: *******\n";
             for(i=0; pos<tamBloque; i++)//esto se hace mientras la posicion sea menor al tamaño del bloque
                    {
                        switch((arrAtr[i]).tipo)//esto es para verificar del tipo de atributo al que esta asignado cada bloque
                                {
                                case 1:
                                    cout<<">>Tipo: Char / Tamaño: "<<arrAtr[i].tamano<<" / Es clave: "<<arrAtr[i].esclave<<" / Bloque: ";
                                    texto=(char*)malloc(sizeof(char)*(arrAtr[i].tamano));
                                    strcpy(texto,((char*)((char*)bloque+pos)));
                                    contcarac=strlen(texto);
                                    texto[contcarac]='\0';
                                    cout<<(texto)<<"\n";//se imprime el bloque de tipo char
                                    free(texto);
                                        break;
                                case 2:
                                    cout<<">>Tipo: Int / Tamaño: "<<arrAtr[i].tamano<<" / Es clave: "<<arrAtr[i].esclave<<" / Bloque: ";
                                    cout<<*((int *)((char *)bloque+pos))<<"\n";//se imprime el bloque de tipo entero
                                        break;
                                case 3:
                                    cout<<">>Tipo: Float / Tamaño: "<<arrAtr[i].tamano<<" / Es clave: "<<arrAtr[i].esclave<<" / Bloque: ";
                                    cout<<*((float *)((char *)bloque+pos))<<"\n";//se imprime el bloque de tipo float
                                        break;
                                case 4:
                                    cout<<">>Tipo: Long / Tamaño: "<<arrAtr[i].tamano<<" / Es clave: "<<arrAtr[i].esclave<<" / Bloque: ";
                                    cout<<*((long *)((char *)bloque+pos))<<"\n";//se imprime el bloque de tipo long
                                        break;
                                case 5:
                                    cout<<">>Tipo: Double / Tamaño: "<<arrAtr[i].tamano<<" / Es clave: "<<arrAtr[i].esclave<<" / Bloque: ";
                                    cout<<*((double *)((char *)bloque + pos))<<"\n";//se imprime el bloque de tipo double
                                        break;
                                }
                                pos += (arrAtr[i]).tamano;//la posicion se incrementa a partir del tamaño del tipo de atributo
                            }
        }
        else
        {
            cout<<"\n****NO Existen bloques****\n";
        }


    }
    else
    {
      cout<<"\n****NO ENCONTRO LA ENTIDAD****\n";
    }

}

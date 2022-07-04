
#define MAX_BUFFER 200
#define MAX_INTENTOS_READ 4 
#define MS_ENTRE_INTENTOS 250 
#define SI 1 
#define NO 0

#define LONGCAD 20
typedef struct
{
	float temp;
	char dia[LONGCAD];

}TEMPERATURA;

#include <stdio.h> 
#include <stdlib.h> 
#include <locale.h> 
#include <windows.h>
#include <string.h>
#include <conio.h> 
#include "SerialClass.h"

int menu_principal(void); 
void configura(void);
void Talk_with_Arduino(Serial* Arduino);
void Send_to_hw(Serial*, char*);
int Receive_from_hw(Serial* Arduino, char* BufferEntrada);
int Send_and_Receive(Serial* Arduino, const char* msg_out, int valor_out, char* msg_in, int* valor_in);
//void monitorizar_aforo(Serial*);
void mostrar_temp(Serial*);
void luces(Serial*);
void automatico(Serial*);
void apagar_luces(Serial*);
void encender_luces(Serial*);
void mostrar_nivel_bebedero(Serial*);
void elegir_nivel_bebedero(Serial*);
void leer_fichero_temperaturas(TEMPERATURA[], int*, int);
int escribir_fichero_temperaturas(TEMPERATURA*, int );

int main(void)
{
	Serial* Arduino; char puerto[] = "COM3"; //Puerto serie al que está conectado Arduino 
	int opc;
	TEMPERATURA* temperaturas;
	int n = 7;
	temperaturas = (TEMPERATURA*)malloc(sizeof(TEMPERATURA) * n);
	// Tareas de configuración y carga
	configura();
	Arduino = new Serial((char*)puerto);  // Establece la conexión con Arduino

	// Bucle principal de la aplicación
	do
	{
		opc = menu_principal();
		switch (opc)
		{
		case 1:
			mostrar_nivel_bebedero(Arduino);
			break;
		case 2:
			mostrar_temp(Arduino);
			escribir_fichero_temperaturas(temperaturas, n);

			break;
		case 3:
			luces(Arduino);
			break;
		case 4:
			automatico(Arduino);
			break;
		case 6:
			break;
		}
		printf("\n\n");
	} while (opc != 6);

	// Tareas de desconexión y cierre 
	return 0;
}

int menu_principal(void)
{
	int opcion;
	do
	{
		printf("1 - Mostrar el nivel de agua del bebedero\n");
		printf("2 - Leer temperatura de la caseta\n");
		printf("3 - Encendido/Apagado de luces\n");
		printf("4 - Elegir nivel del agua\n");
		printf("5 - Automatico\n");
		printf("6 - Cerrar Aplicación\n");
		printf("Seleccione opción: ");
		scanf_s("%d", &opcion);
		if (opcion < 1 || opcion>6)
			printf("\nOpción inexistente.\n\n");
	} while (opcion < 1 || opcion>6);
	return opcion;
}

void mostrar_temp(Serial* Arduino)
{
	int bytes_recibidos;
	TEMPERATURA* temperaturas;
	int i = 0;
	char mensaje_in[200];
	char mensaje_out[] = "MOSTRAR_TEMP";
	int temp; float temp2;
	bytes_recibidos = Send_and_Receive(Arduino, "MOSTRAR_TEMP", 1, mensaje_in, &temp);

	if (bytes_recibidos != 0)
	{ 
		temp2 = (float)temp / 100;
		(temperaturas + i)->temp = temp2;
		i++;
		printf("\nLa temperatura de la caseta es %.2f grados celsius \n", temp2);
		if (temp2 >= 30) {

			printf("La temperatura de la caseta es muy elevada,revise el estado de su mascota\n");
		}
		else
		{
			printf("La temperatura de la caseta esta a una temperatura correcta\n");
		}
	}
}


void leer_fichero_temperaturas(TEMPERATURA  temperaturas[], int* pnumero, int longitud)
{
	FILE* fichero; // Puntero para manipular el fichero
	int num = 0; // Variable auxiliar para numero de temps leidas3
	int i, pos; // Variable bucle y posicion final cadena
	errno_t cod_error;
	char intro[2];

	cod_error = fopen_s(&fichero, "Temperaturas.txt", "rt"); // Se intenta abrir el fichero de texto
	if (cod_error != 0)  // Si el fichero no se ha podido abrir
		*pnumero = 0;
	else  // Si el fichero ha podido abrirse 
	{
		fscanf_s(fichero, "%d", &num); // Se lee la cantidad de temps
		if (num == 0) // Si esa cantidad es cero
			*pnumero = 0; // La lista estar� vac�a
		else  // Si hay registros para leer (seg�n el entero le�do)
		{
			if (num > longitud) // Si no hay memoria suficiente
			{
				printf("Memoria insuficiente para almacenar los datos del fichero\n");
				*pnumero = 0;
			}
			else // Si hay memoria suficiente
			{
				fgets(intro, 2, fichero); // Saltamos el intro que hay tras el numero (Ascii 10 y 13)
				for (i = 0; i < num; i++)  // Se leen los registros uno por uno
				{
					fgets((temperaturas + i)->dia, LONGCAD, fichero);
					pos = strlen((temperaturas + i)->dia);
					(temperaturas + i)->dia[pos - 1] = '\0';
					fscanf_s(fichero, "%d", &(temperaturas + i)->temp);
					fgets(intro, 2, fichero);
				}
				*pnumero = num;
			}
		}
		fclose(fichero); // Se cierra el fichero
	}
}

int escribir_fichero_temperaturas(TEMPERATURA* temperaturas, int numero)
{
	int i;
	FILE* fichero;
	errno_t err;

	err = fopen_s(&fichero, "Temperaturas.txt", "wt");
	if (err == 0) // Si el fichero se ha podido crear
	{
		fprintf(fichero, "%d\n", numero); // Se graba en el fichero el numero de usuarios
		for (i = 0; i < numero; i++)
		{
			fprintf(fichero, "%s\n", (temperaturas + i)->dia);
			fprintf(fichero, "%f\n", (temperaturas + i)->temp);
		}
		fclose(fichero);
	}
	else
		printf("Se ha producido un problema a la hora de grabar el fichero de usuarios\n");
	return err;
}

void luces(Serial * Arduino)
{
		int bytes;
		char mensaje_in[200];
		char mensaje_out[] = "ILUMINACION";
		int luz;
		bytes = Send_and_Receive(Arduino, "ILUMINACION", 1, mensaje_in, &luz);
		if (bytes != 0)
		{
			if (mensaje_in[0] == '1') {
				int siono;
				printf("\nLas luces de la caseta están encendidas, desea apagarlas?\n\t1.Si\n\t2.No\n");
				scanf_s("%d", &siono);
				switch (siono)
				{
				case 1:
					apagar_luces(Arduino);
					break;
				case 2:
					break;
				}
			}
			else
			{
				int siono;
				printf("\nLas luces de la caseta están apagadas, desea encenderlas?\n\t1.Si\n\t2.No\n");
				scanf_s("%d", &siono);
				switch (siono)
				{
				case 1:
					encender_luces(Arduino);
					break;
				case 2:
					break;
				}
			}
		}
}

	void apagar_luces(Serial * Arduino)
	{
		int bytes;
		char mensaje_entr[200];
		char mensaje_sali[] = "APAGA";
		int valor;
		bytes = Send_and_Receive(Arduino, "APAGA", 1, mensaje_entr, &valor);
	}
	void encender_luces(Serial * Arduino)
	{
		int bytes;
		char mensaje_entr[200];
		char mensaje_sali[] = "ENCIENDE";
		int valor;
		bytes = Send_and_Receive(Arduino, "ENCIENDE", 1, mensaje_entr, &valor);
	}

	void mostrar_nivel_bebedero(Serial * Arduino)
	{
		int bytes_recibidos;
		char mensaje_in[255];
		char mensaje_out[] = "MOSTRAR_NIVEL_BEBEDERO";
		float altura = 13;
		int distancia;
		float nivel2;
		bytes_recibidos = Send_and_Receive(Arduino, "MOSTRAR_NIVEL_BEBEDERO", -1, mensaje_in, &distancia);
		if (bytes_recibidos != 0)
		{
			nivel2 = (float)distancia / 100;
			nivel2 = altura - nivel2;
			printf("\nEl nivel del agua de su bebedero es %.1f cm", nivel2);

		}
	}
	void automatico(Serial * Arduino)
	{
		elegir_nivel_bebedero(Arduino);
		int bytes_recibidos;
		char mensaje_entr[200];
		char mensaje_sal[] = "AUTOMATICO";
		int var;
		char tecla;
		bytes_recibidos = Send_and_Receive(Arduino, "AUTOMATICO", 1, mensaje_entr, &var);
		while (!_kbhit())
		{
			automatico(Arduino);
		}
		tecla = _getch();
	}

	void elegir_nivel_bebedero(Serial * Arduino)
	{
		int bytes_recibidos;
		char mensaje_in[255];
		char mensaje_out[] = "NIVEL_BEBEDERO_CONSIGNA";
		float nivel, x;
		int altura = 12;
		printf("\n");
			printf("\nElija la altura mínima a la que quiere que este el agua de su bebedero (en cm) :\n");
		scanf_s("%d", &x);
		nivel = altura - x;
		bytes_recibidos = Send_and_Receive(Arduino, "MOSTRAR_NIVEL_AGUA", nivel, mensaje_in, &altura);

	}

	void configura(void)
	{
		// Establece juego de caracteres castellano
		// Para que funcione hay que partir de un proyecto vacío 
		// No utilice la plantilla Aplicación de consola C++ setlocale(LC_ALL, "spanish");
	}

	// Ejemplo de función de intercambio de datos con Arduino 
	void Talk_with_Arduino(Serial * Arduino)
	{ //char BufferSalida[MAX_BUFFER]; 
		char BufferEntrada[MAX_BUFFER];
		int bytesReceive, numero_recibido;
		if (Arduino->IsConnected()) // Si hay conexión con Arduino 
		{

			// Para enviar un mensaje y obtener una respuesta se utiliza la función Send_and_Receive
			// El mensaje está formado por un texto y un entero
			// El mensaje que se recibe está formado también por un texto y un entero.
			// Parámetros de la función:
			// El primero es la referencia a Arduino
			// El segundo es el mensaje que se desea enviar
			// El tercero es un entero que complementa al mensaje que se desea enviar
			// El cuarto es el vector de char donde se recibe la respuesta
			// El quinto es la referencia donde se recibe el entero de la respuesta
			// La función devuelve un entero con los bytes recibidos. Si es cero no se ha recibido nada.

			bytesReceive = Send_and_Receive(Arduino, "GET_AFORO_MAX", -1, BufferEntrada, &numero_recibido);
			if (bytesReceive == 0)
				printf("No se ha recibido respuesta al mensaje enviado\n");
			else
				printf("Mensaje recibido %s %d\n", BufferEntrada, numero_recibido);
		}
		else
			printf("La comunicación con la plataforma hardware no es posible en este momento\n"); // Req 3

	}

	// Protocolo de intercambio mensajes entre Pc y platforma hardware 
	// Envío Mensaje valor
	// Recibe Mensaje valor 
	// Retorna bytes de la respuesta (0 si no hay respuesta) 
	int Send_and_Receive(Serial * Arduino, const char* msg_out, int valor_out, char* msg_in, int* valor_in)
	{
		char BufferSalida[MAX_BUFFER];
		char BufferEntrada[MAX_BUFFER];
		char* ptr;
		int bytesReceive;
		sprintf_s(BufferSalida, "%s\n%d\n", msg_out, valor_out);
		Send_to_hw(Arduino, BufferSalida);
		bytesReceive = Receive_from_hw(Arduino, BufferEntrada);
		if (bytesReceive != 0)
		{
			ptr = strpbrk(BufferEntrada, " ");
			if (ptr == NULL)
				*valor_in = -1;
			else
			{
				*valor_in = atoi(ptr);
				*ptr = '\0';
			}
			strcpy_s(msg_in, MAX_BUFFER, BufferEntrada);
		}
		return bytesReceive;
	}

	// Envía un mensaje a la plataforma hardware 
	void Send_to_hw(Serial * Arduino, char* BufferSalida)
	{
		Arduino->WriteData(BufferSalida, strlen(BufferSalida));
	}

	//Recibe (si existe) un mensaje de la plataforma hardware 
	//Realiza MAX_INTENTOS_READ para evitar mensajes recortados 
	int Receive_from_hw(Serial * Arduino, char* BufferEntrada)
	{
		int bytesRecibidos, bytesTotales = 0;
		int intentos_lectura = 0;
		char cadena[MAX_BUFFER];
		BufferEntrada[0] = '\0';
		while (intentos_lectura < MAX_INTENTOS_READ)
		{
			cadena[0] = '\0';
			bytesRecibidos = Arduino->ReadData(cadena, sizeof(char) * (MAX_BUFFER - 1));
			if (bytesRecibidos != -1)
			{
				cadena[bytesRecibidos] = '\0';
				strcat_s(BufferEntrada, MAX_BUFFER, cadena);
				bytesTotales += bytesRecibidos;
			}
			intentos_lectura++;
			Sleep(MS_ENTRE_INTENTOS);
		}
		return bytesTotales;
	}

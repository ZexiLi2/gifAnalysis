// Introducción a C++, Nacho Cabanes
// Ejemplo 08.06:
// Información sobre un fichero BMP (2)
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;
int main()
{
	int tamanyoCabecera = 54;
	char* cabecera = new char[tamanyoCabecera];
	string nombre;
	unsigned int compresion, ancho, alto;
	char marca1, marca2, marca3;
	cout << "GIF Analyzer" << endl;
	cout << "Insert File Name: ";
	getline(cin, nombre);
	ifstream fichero(nombre.c_str(), ifstream::binary);
	if (fichero.fail())
		cout << "File not found" << endl;
	else
	{
		marca1 = fichero.get(); // Leo los tres primeros bytes
		marca2 = fichero.get();
		marca3 = fichero.get();
		if ((marca1 == 'G') && (marca2 == 'I') && (marca3 == 'F')) // Si son BM
		{
			cout << "Analysing [" << nombre << "]" << endl;
			//calcular tamany en bytes
			int begin, end;
			ifstream myfile(nombre, ios::binary);
			begin = myfile.tellg();
			myfile.seekg(0, ios::end);
			end = myfile.tellg();
			myfile.close();

			// Leo toda la cabecera
			fichero.seekg(0, fichero.beg);
			fichero.read(cabecera, tamanyoCabecera);

			unsigned char anchoBytes[2];
			unsigned char altoBytes[2];

			fichero.seekg(6); //saltar els primers 6 bytes
			fichero.read(reinterpret_cast<char*>(anchoBytes), 2);
			fichero.read(reinterpret_cast<char*>(altoBytes), 2);

			//convertir els bytes a valors enters (formato little-endian)
			int ancho = anchoBytes[0] | (anchoBytes[1] << 8);
			int alto = altoBytes[0] | (altoBytes[1] << 8);

			fichero.seekg(10);
			unsigned char colorInfo;
			fichero.read(reinterpret_cast<char*>(&colorInfo), 1);

			//calcular la cantitat de colors
			int numcolor = 1 << ((colorInfo & 0x07) + 1);


			// Leer los primeros 6 bytes que contienen la versión del GIF
			char version[6];
			fichero.read(version, 6);
			if (!fichero) {
				cerr << "Error al leer el archivo GIF." << endl;
				return 1;
			}

			//extreure versió del GIF
			string versionGIF(version, 3);

			//mostrar per pantalla informació
			cout << nombre<< "'s size is: " << (end - begin) << " bytes.\n";
			cout << "Dimensions (width/height): " << ancho << "x" << alto << " pixels" << endl;
			cout << "Number of colors: " << numcolor << endl;
			cout << "Version: " << versionGIF << endl;

			char opcion;
			cout << "¿Deseas hacer una copia del archivo GIF? (Y/N): ";
			cin >> opcion;

			if (opcion == 'Y' || opcion == 'y') {
				// Generar el nombre de la copia del archivo
				string rutaArchivo = "./";
				string nombreCopia = rutaArchivo + nombre+ "_copia.gif";

				// Copiar el archivo
				ifstream archivoOriginal(rutaArchivo, ios::binary);
				ofstream archivoCopia(nombreCopia, ios::binary);

				archivoCopia << archivoOriginal.rdbuf();

				archivoOriginal.close();
				archivoCopia.close();

				cout << "Se ha creado una copia del archivo GIF." << endl;
			}

			fichero.close();
			cout << endl;

		}
		else //si no es gif
			cout << "Not a GIF file" << endl;
	}
	return 0;
}
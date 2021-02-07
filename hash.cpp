//m�todo disperso o hash
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <Windows.h>
using namespace std;

struct encabezado{
	int NRS;
}ed,ec;
struct registro{
	int NR;
	char nombre[20];
	char email[30];
	int SR;
}r,s;

FILE *fdd, *fdc;
int le = sizeof(struct encabezado);
int lr = sizeof(struct registro);
int pos;

int fhash(char nom[20]){
	int suma=0;
	for(int i=0; i<strlen(nom); i++){
		suma += nom[i];
	}
	return suma%10 + 1;
}

void deleteFiles(){
	system("DEL persona.txt, persona.col");
}

void write(registro r){
	int sgte;
	if ((fdd = fopen("persona.txt", "r+")) == NULL){
		if ((fdd = fopen("persona.txt", "w+")) == NULL){
			cout << "no se pudo crear persona.txt" << endl; return;
		}
		ed.NRS = 0;
		fwrite(&ed, le, 1, fdd);
	}
	if ((fdc = fopen("persona.col", "r+")) == NULL){
		if ((fdc = fopen("persona.col", "w+")) == NULL){
			cout << "no se pudo crear persona.col" << endl;return;
		}
		ec.NRS = 0;
		fwrite(&ec, le, 1, fdc);
	}
	fread(&ed, le, 1, fdd); fread(&ec, le, 1, fdc);
	r.SR = -1;
	r.NR = fhash(r.nombre);
	pos = (r.NR - 1) * lr + le;
	fseek(fdd, pos, 0); fread(&s, lr, 1, fdd);
	if (strcmp(s.nombre, "") == 0){
		ed.NRS++;
		fseek(fdd, pos, 0);
		fwrite(&r, lr, 1, fdd);
	}else{
		r.NR = ++ec.NRS;
		pos = (r.NR - 1) * lr + le;
		fseek(fdc, pos, 0); fwrite(&r, lr, 1, fdc);
		if (s.SR == -1){ 
			s.SR = r.NR;
			pos = (s.NR - 1) * lr + le;
			fseek(fdd, pos, 0); fwrite(&s, lr, 1, fdd);
		}else{
			sgte = s.SR;
			while (sgte != -1){
				pos = (sgte - 1) * lr + le;
				fseek(fdc, pos, 0); fread(&s, lr, 1, fdc);
				sgte = s.SR;
			}
			s.SR = r.NR;
			pos = (s.NR - 1) * lr + le;
			fseek(fdc, pos, 0);fwrite(&s, lr, 1, fdc);
		}
	}
	fseek(fdd,0,0); fseek(fdc,0,0);
	fwrite(&ed,le,1,fdd); fwrite(&ec,le,1,fdc);
	fclose(fdd); fclose(fdc);
}

void leerTodo(){
	if((fdd=fopen("persona.txt","rt")) == NULL){
		cout<<"no se pudo abrir persona.txt"<<endl;
		return;
	}
	if((fdc=fopen("persona.col","rt")) == NULL){
		cout<<"no se pudo abrir persona.col"<<endl;
		return;
	}	
	fread(&ed,le,1,fdd); 
	cout<<"Archivo Disperso. NRS:"<<ed.NRS<<endl;
	cout<<"NRS\tnombre\temail\tSR\n";
	while(fread(&s,lr,1,fdd)){
		cout<<s.NR<<"\t"<<s.nombre<<"\t"<<s.email<<"\t"<<s.SR<<endl;
	}
	fread(&ec,le,1,fdc); 
	cout<<"Archivo Colision. NRS:"<<ec.NRS<<endl;
	cout<<"NRS\tnombre\temail\tSR\n";
	while(fread(&s,lr,1,fdc)){
		cout<<s.NR<<"\t"<<s.nombre<<"\t"<<s.email<<"\t"<<s.SR<<endl;
	}
	fclose(fdd); 
	fclose(fdc);
}

void buscar(){
	char nom[20]; int NR, sgte=0;
	if((fdd=fopen("persona.txt","rt")) == NULL){
		cout<<"no se pudo abrir persona.txt"<<endl;
		return;
	}
	if((fdc=fopen("persona.col","rt")) == NULL){
		cout<<"no se pudo abrir persona.col"<<endl;
		return;
	}	
	fflush(stdin);
	cout<<"nombre:"<<endl; 	
	gets(nom);
	NR = fhash(nom); 
	cout<<"Hash:"<<NR<<endl;
	pos = (NR-1)*lr + le;
	fseek(fdd,pos,0);
	fread(&s,lr,1,fdd);
	if(s.SR == 0){
		cout<<"no existe registro con ese hash"<<endl; return;
	}
	if(strcmp(s.nombre,nom) == 0){
		cout<<s.NR<<"\t"<<s.nombre<<"\t"<<s.email<<"\t"<<s.SR<<endl;
		return;
	}	
	cout<<"no existe en el archivo *.txt"<<endl;
	sgte = s.SR;
	while(sgte != -1){
		pos = (sgte -1)*lr + le;
		fseek(fdc,pos,0);
		fread(&s,lr,1,fdc);			
		if(strcmp(nom,s.nombre)==0){
				cout<<s.NR<<"\t"<<s.nombre<<"\t"<<s.email<<"\t"<<s.SR<<endl;
				return;
			}
		sgte = s.SR;
		}
	cout<<"no existe en *.col"<<endl; return;
}

void update(char nom[]){
	int NR, sgte = 0;
	if ((fdd = fopen("persona.txt", "r+t")) == NULL){
		cout << "no se pudo abrir persona.txt" << endl;
		return;
	}
	if ((fdc = fopen("persona.col", "r+t")) == NULL){
		cout << "no se pudo abrir persona.col" << endl;
		return;
	}
	NR = fhash(nom);
	cout << "Hash:" << NR << endl;
	pos = (NR - 1) * lr + le;
	fseek(fdd, pos, 0);
	fread(&s, lr, 1, fdd);
	if (s.SR == 0){
		cout << "no existe registro con ese hash" << endl;
		return;
	}
	if (strcmp(s.nombre, nom) == 0){
		cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR << endl;
		fflush(stdin);
		cout << "ingrese el nuevo email: ";
		gets(s.email);
		fseek(fdd, pos, 0);
		fwrite(&s, lr, 1, fdd);
		fclose(fdd);fclose(fdc);
		return;
	}
	cout << "no existe en el archivo *.txt" << endl;
	sgte = s.SR;
	while (sgte != -1){
		pos = (sgte - 1) * lr + le;
		fseek(fdc, pos, 0);
		fread(&s, lr, 1, fdc);
		if (strcmp(nom, s.nombre) == 0){
			cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR << endl ;
			fflush(stdin);
			cout << "ingrese el nuevo email: ";
			gets(s.email);
			cout<< "nuevo: " << s.email << endl;
			fwrite(&s, lr, 1, fdc);
			return;
		}
			sgte = s.SR;
	}
	cout << "no existe en *.col" << endl;
	return;
}

void deleteR(char nom[]){
    int NR, sgte = 0;
    if ((fdd = fopen("persona.txt", "r+t")) == NULL){
        cout << "no se pudo abrir persona.txt" << endl;
        return;
    }
    if ((fdc = fopen("persona.col", "r+t")) == NULL){
        cout << "no se pudo abrir persona.col" << endl;
        return;
    }
    NR = fhash(nom);
    cout << "Hash:" << NR << endl;
    pos = (NR - 1) * lr + le;
    fseek(fdd, pos, 0);
    fread(&s, lr, 1, fdd);
    if (s.SR == 0){
        cout << "no existe registro con ese hash" << endl;
        return;
    }
    if (strcmp(s.nombre, nom) == 0){
        strcpy(s.nombre, "");
        strcpy(s.email, "");
        if (s.SR == -1){
            s.NR = 0;
            s.SR = 0;
        }
        fseek(fdd, pos, 0);
        fwrite(&s, lr, 1, fdd);
        fclose(fdd); fclose(fdc);
        return;
    }
    cout << "no existe en el archivo *.txt" << endl;
    int auxpos = pos ;
    int leftSR = s.SR; 
    sgte = s.SR;
    if(sgte != -1){
        pos = (sgte - 1) * lr + le;
        fseek(fdc, pos, 0);
        fread(&s, lr, 1, fdc);
        if (strcmp(nom, s.nombre) == 0){
            cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR << endl;
            fseek(fdd, pos, 0);
            fwrite(&s, lr, 1, fdd);

            fwrite(&s, lr, 1, fdc);
            return;
        }
        sgte = s.SR;
    }

    while (sgte != -1){
        pos = (sgte - 1) * lr + le;
        fseek(fdc, pos, 0);
        fread(&s, lr, 1, fdc);
        if (strcmp(nom, s.nombre) == 0){
            cout << s.NR << "\t" << s.nombre << "\t" << s.email << "\t" << s.SR << endl;
            fwrite(&s, lr, 1, fdc);
            return;
        }
        sgte = s.SR;
    }
    cout << "no existe en *.col" << endl;
    return;
}


int main (int argc, char *argv[]){	
	int op;
	do{
		cout<<"1.escribir 2.leerTodo 3.buscar 4.update 5.delete 6.deletefiles"<<endl;
		cin>>op;
		switch(op){
			case 1: {
				char rpta;
				do{
					fflush(stdin);
					cout << "nombre:" << endl;
					gets(r.nombre);
					fflush(stdin);
					cout << "email:" << endl;
					gets(r.email);
					write(r);
					cout << "desea mas registros?" << endl;
					cin >> rpta;
				} while (rpta == 'S' || rpta == 's');
				break;
			}
			case 2: leerTodo(); break;
			case 3: buscar(); break;
			case 4: {
				fflush(stdin);
				cout << "nombre:" << endl;
				gets(r.nombre);
				update(r.nombre);
				break;
			}
			case 5: {
                fflush(stdin);
				cout << "nombre:" << endl;
				gets(r.nombre);
				deleteR(r.nombre);
				break;
            } 
            case 6: deleteFiles();
		}
	} while(op!=-1);
	return 0;	
}


#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"




static Semaphore *readAvail;
static Semaphore *writeDone;

static Semaphore *writeChar;
static Semaphore *readChar;


//static Semaphore *writeString;


static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0); // Console asynchronous
	
	// Un seul processus autorisé à lire et écrire dans la console
	// Exlusion mutuelle
	writeChar = new Semaphore("write char",1);
	readChar = new Semaphore("read char",1);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
	//Principe du sémaphore
	writeChar->P(); // On prend la clé, donc blocage pour les autres processus 

	console->PutChar(ch); // Ecriture 
	
	writeDone->P(); // On bloque quand on a fini d'écrire

	writeChar->V(); // On libere la ressource
}

char SynchConsole::SynchGetChar()
{
	// Même principe que pour le SynchPutChar
	readChar->P(); 
	
	readAvail->P();
	
	char c = console->GetChar(); // on sauvegarde la lettre

	readChar->V();
	return c;
}




void SynchConsole::SynchPutString(const char s[])
{
	
	int i = 0;
	while(*(s+i)!='\0'){
		SynchPutChar(*(s+i));
		i++;
	}
}

void SynchConsole::SynchGetString(char *s, int n)
{
	//readString->P();

	int i = 0;
	char c;
	while((i<n)&&((c=SynchGetChar())!=EOF)&&(c!='\n')){
		*(s+i)=c;
		i++;
	}
	*(s+i) = '\0';

	//readString->V();
}

void SynchConsole::CopyMachineFromString(char* from, int to, unsigned size){
	unsigned i = 0;
	int res;

	while((i<size)&&(*(from+i)!='\0')){
		res = *(from+i);
		machine->WriteMem(to+i,1,res);
		i++;
	}
	machine->WriteMem(to+i,1,'\0');

}
/*
 * void SynchPutInt( int n) qui écrit un entier signé en
utilisant la fonction snprintf pour en obtenir l’écriture externe décimale 
 */
void SynchConsole::SynchPutInt(int n){

	char* string = new char[MAX_STRING_SIZE];
	snprintf(string,MAX_STRING_SIZE,"%d",n); //ecrit n dans string 
	SynchPutString(string);

	delete [] string;
}

/*
 * Idem dans l’autre sens avec
void SynchGetInt( int *n) et la fonction sscanf .
 */ 
void SynchConsole::SynchGetInt( int *n){
	int* i = new int;
	char* string = new char[MAX_STRING_SIZE];
	SynchGetString(string,MAX_STRING_SIZE);
	sscanf(string,"%d",i);

	machine->WriteMem(*n,4,*i);
	delete [] string;
	delete i;
}
#endif // CHANGED

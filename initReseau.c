#include <stdint.h>
#include <stdio.h>


void lireFichier(const char *f)
{
    FILE *fichier=NULL; 
    fichier=fopen(f,"r");
    char buffer[1024];
    char *ligne;
    ligne=fgets(buffer,1024,fichier);
    while(ligne!=NULL)
    {
        //if(strstr( buffer, %d+" "+%d ))
        
    }
}
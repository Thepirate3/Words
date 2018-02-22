// Mocanu Alexandru George 315 CB
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct nod{
	unsigned char litera;
	int end;
	int nr_fii;
	struct nod **children;
	}Nod,*TNod;

void* init()
{
	//se aloca spatiu pentru nodul curent
	TNod nod = calloc(1,sizeof(Nod));
	if(!nod) return 0;
	//se initializeaza valorile noului nod
	nod->children = NULL;
	nod->litera = ' ';
	nod->end=0;
	nod->nr_fii=0;
	return nod;
}

int caut_litera(TNod nod,char litera)
{
	int i=0;
	
	//verific daca litera se afla printre fii 
    	for(i=0;i< nod->nr_fii;i++)
	 {
		
		if( nod->children[i]->litera == litera ) return i;
		
	 }
	return -1;
}



void sortare(TNod arb)
{
	int j,i,flag=1;
	
	//repet operatia de un numar egal cu fii 
	for(j=0;j< (arb->nr_fii);j++)
	 {
		//daca vectorul a fost deja sortat oprim rularea
  		if(flag == 0) break;
		
		flag = 0;
		
		//pentru fiecare valoare din vectorul de fii 
		for(i=0 ; i < (arb->nr_fii-1 ); i++)
		 {		
			//verific daca vecinii repescta relatia de ordine
			if(arb->children[i]->litera > arb->children[i+1]->litera) 
			{
			  TNod o =NULL;
			  o=arb->children[i];
			  arb->children[i] = arb->children[i+1];
			  arb->children[i+1] = o;
			  flag = 1;
			}

		 }
	}

}
void insert(TNod arbore, char *cuvant)
{

	TNod p = arbore;
	int i,lungime = strlen(cuvant);
	
	//pentru fiecare litera din cuvant 
	for(i = 0; i< lungime;i++)
	{
	     //daca nodul curent nu are fii 
	     if( p->nr_fii == 0) 
	       	{
	      	  p->children = calloc(1,sizeof(Nod));
		  p->children[0] = init();
	     	  p->children[0]->litera = cuvant[i];
		  p->nr_fii++;
		  p = p->children[0];
	   	}
	    else
	  	{  
		   //daca litera nu se gaseste printre fii
	  	   if( caut_litera(p,cuvant[i]) == -1)
             	    {
			p->children = realloc(p->children,(p->nr_fii+1)*sizeof(Nod));
			p->children[p->nr_fii] = init();
	     	  	p->children[p->nr_fii]->litera = cuvant[i];
			p->nr_fii++;
		  	p = p->children[p->nr_fii-1];
		    }
	    	   else
		    {
			//altfel continui avansul in arbore
			p = p->children[caut_litera(p,cuvant[i])];
			
		    }
	 	}
	 //sortez vectorul de fii pentru a fi in ordine alfabetica
	 sortare(p);
	}
	//marchez finalul cuvantului
	p->end=1;
	

}

void reorganizare(TNod arb,int count)
{
	int i,j=0;
	TNod *o= calloc(count-1,sizeof(Nod));
	//pentru numarul de elemente dat
	for(i=0;i<count;i++)
	  {
		if(arb->children[i] != NULL)
		 {
		   //atribui noului vector de fii doar fii nenuli
		   o[j]=arb->children[i];
		   j++;
		 }
	  }
	//realoc vectorul de fii 
	arb->children = realloc(arb->children,(count-1)*sizeof(Nod));
	for(i=0;i<count-1;i++)
	 {
		//reconstruiesc vectorul de fii 
	   	arb->children[i]=o[i];
	 }
	//eliberez vectorul auxiliar
	free(o);

}
void remove1(TNod arb,char *cuvant,int *eliminat)
{
	TNod p = arb;

	int i = caut_litera(p,cuvant[0]), count = p->nr_fii;
	
	//daca gasesc litera in vectoru de fii
	if( caut_litera(p,cuvant[0]) != -1 )
	{
	  //modific cuvantul
	  cuvant = cuvant+1;
	  //continui avansul in arbore
	  remove1(p->children[i],cuvant,eliminat);
	}
		
	else
	 {
		//daca cuvantul nu este nul 
		if( strlen(cuvant) != 0 ) return;
		
		else
		 {
			//altfel nodul il marchez ca fiind de eliminat
	    		if(p->nr_fii == 0) *eliminat=1;
			
			//altfel doar ii elimin proprietatea de a fi final de cuvant
			else p->end =0;	
			return;
		 }
	 }
	
	//daca recurenta a ajuns in nodul parinte
	if( p->litera == ' ' ) return;
	 
	//daca nodul copil este de eliminat
	if(*eliminat == 1 )	
	 {
  		free(p->children[i]);
		p->children[i]=NULL;
		reorganizare(p,count);
		p->nr_fii--;
	 }
	//daca nodul curent nu poate fi sters
	if( p->nr_fii != 0 || p->end == 1) *eliminat=0;
	
	
}

void find(TNod arb, char *cuvant,FILE *f)
{
	TNod p = arb;
	int i,lungime = strlen(cuvant);
	
	//pentru fiecare pozitie din cuvantul de cautat
	for( i = 0; i< lungime; i++)
	 {
	   //daca am ajuns la ultima litera din cuvant
	   if( i == lungime-1 )
 	    {
		//daca litera se gaseste in vectoru de fii si este sfarsit de cuvant
		if( caut_litera(p,cuvant[i]) != -1 && 
		   p->children[caut_litera(p,cuvant[i])]->end == 1  )
			//avansez pe ultima pozitie
			p=p->children[caut_litera(p,cuvant[i])];

	  	 else
		 {	//altfel cuvantul nu este gasit
			fprintf(f,"False\n");
		 	return;
		 }
	    }

	   else
	    {
		//daca litera se gaseste in vectorul de fii 
	    	if(caut_litera(p,cuvant[i]) != -1 )
			//avansez
			p=p->children[caut_litera(p,cuvant[i])];
	   	else
	   	  {
		    //altfel cuvantul nu se afla in arbore
	            fprintf( f,"False\n" );
		    return;
		  }
	    }
	 }
	//daca avansul se incheie inseamna ca am gasit cuvantul
	fprintf(f,"True\n");

}

void find_prefix(TNod arb,char *cuvant,FILE *f)
{
	TNod p = arb;
	int lungime = strlen(cuvant),i,aux = 0;
    	char *rez = calloc(2,sizeof(char));
	
	//pentru fiecare litera din cuvant
    	for(i=0;i<lungime;i++)
    	 {
		//daca litera nu este gasita se opreste rularea
        	if( caut_litera(p,cuvant[i]) == -1 ) break;
          	//realocam si reactualizam prefixul
		rez = realloc(rez,strlen(rez)+2);
	  	rez[i]=cuvant[i];
	  	rez[i+1]='\0';
		//continuam avansul in trie
         	p = p->children[caut_litera(p,cuvant[i])];
    	 }
    	//daca rez este 0 inseamna ca nu am gasit prefix
	if( strlen(rez) == 0) fprintf(f,"None\n");
    	//altfel printam prefixul maxim
	else
    	fprintf(f,"%s\n",rez);
 	//eliberez prefixul construit
	free(rez);
}


void printare(TNod arb,char *cuv,char *cuvinit,FILE *f)
{
	int i = 0,k = strlen(cuv);
	//daca nodul curent este sfarsit de cuvant afisam cuvantul construit
	if( arb->end == 1 ) fprintf(f,"%s ",cuv);
	//sortam vectorul de fii al nodului curent
	sortare(arb);
	//pentru fiecare fiu al nodului
	for(i=0;i<arb->nr_fii;i++)
	 {	
		//actualizam cuvantul
		cuv[k] = arb->children[i]->litera;
		sortare(arb->children[i]);
		//continuam avansul recursiv
		printare(arb->children[i],cuv,cuvinit,f);
		//revenim la cuvantul format inaintea apelului
		char *o = calloc(strlen(cuv),sizeof(char));
		strncpy(o,cuv,strlen(cuv)-1);
		strcpy(cuv,o);	
		free(o);
		
 	  }
	
}
void find_words(TNod arb, char *cuvant,FILE *f)
{
	TNod p=arb;
	int i;
	int lungime = strlen(cuvant);
    	
	//daca prefixul este ""
	if(lungime == 0)
	 {
		sortare(p);
		//pentru fiecare fiu al nodului initial parinte		
		for(i=0;i<arb->nr_fii;i++)
		 {	
			//se actualizeaza prefixul
			char *pref = calloc(50,sizeof(char));
			cuvant = calloc(2,sizeof(char));
	   		pref[0] = p->children[i]->litera;	
			pref[1] = '\0';
			strcpy(cuvant,pref);
			//se afiseaza toate cuvintele ce se leaga de fiecare fiu al nodului
   	 	 	printare(p->children[i],pref,cuvant,f);
		   	free(pref);
			free(cuvant);
		  }


	 }
	else
	 {
	  //altfel avansam pana la sfarsitul prefixului	
    	  for(i=0;i<lungime;i++)
       	   {
		//daca litera curenta nu este gasita in arbore 
		if( caut_litera(p,cuvant[i]) == -1 ) 
		 {
		   fprintf(f,"None");
		   return;
		 }
		//altfel se face avansul 
		p = p->children[caut_litera(p,cuvant[i])];
       	   }
	   //se construieste prefixul
    	   char *pref = calloc(3000,sizeof(char));
	   strcpy(pref,cuvant);
	   sortare(p);
	   //se afiseaza toate cuvintele ce contin prefixul
	   sortare(p->children[0]);
    	   printare(p,pref,cuvant,f);
	   free(pref);
       	 }
	


}
char *read(FILE *f)
{
	char a;
	char *cuv = calloc(2,sizeof(char));
	fscanf(f,"%c",&a);
	cuv[0]=a;
	cuv[1]='\0';
	//daca litera citita este finalul linie
	if( cuv[0] == '\n' )
	 {	
		//se opreste citirea din fisier
		strcpy(cuv,"");
		return cuv;
	 }
	//se citeste urmatoarea litera
	fscanf(f,"%c",&a);
	
	//atat timp cat caracterul curent citit respeta conditiile si nu s-a ajuns la final
	while(a != '\r' && a != ' ' && a != '\n' && feof(f) ==0 )
	 {
		//se actualizeaza cuvantul 
		int j = strlen(cuv);
		cuv = realloc(cuv,(j+2)*sizeof(char));
		cuv[j]=a;
		cuv[j+1]='\0';
		fscanf(f,"%c",&a);
	 }
	//daca se citesc operatiile si urmatorul caracter este final de rand
	if( strcmp(cuv,"find_all_with_prefix") == 0 && a != '\r' && a == '\n')
	//pregatim urmatoarea citire
	fseek(f,-1,SEEK_CUR);	
	if( strcmp(cuv,"mean_length") == 0 && a != '\r' && a == '\n')
	fseek(f,-1,SEEK_CUR);
	
	return cuv;

}

void suma_cuv(TNod arb,int *suma,int *nr_cuv,int *lungime)
{
	int i;

	//daca nodul curent este sfarsit de cuvant
	if(arb->end == 1) 
	{	
	  //se actualizeaza suma lungimilor cuvintelor
	  *suma = *suma + *lungime;
	  //se incrementeaza numarul de cuvinte gasite
	  *nr_cuv=*nr_cuv+1; 
	}
	//pentru fiecare fii
	for(i=0;i<arb->nr_fii;i++)
	 {	
		//se actualizeaza lungimea 
		*lungime=*lungime+1;
		//se continua avansul recursiv in arbore
		suma_cuv(arb->children[i],suma,nr_cuv,lungime);
		//se decrementeaza lungimea pentru urmatorul fiu
		*lungime = *lungime -1;	
 	  }
	

}

void mean_length(TNod arb,char *prefix,FILE *f)
{

	TNod p = arb;
	int suma = 0,nr_cuv = 0,lungime = strlen(prefix);
	int i;
	//daca prefixul este ""
	if(lungime == 0)
	 {
		//se cauta lungimile si numarul de cuvinte pentru intreg arborele
		lungime =1;
		for(i=0;i< arb->nr_fii;i++)
		 {
		  suma_cuv(arb->children[i],&suma,&nr_cuv,&lungime);
		 }
	 	lungime=0;
	 }
	else
	 {
		//pentru fiecare litera din prefix		
		for(i=0;i<lungime;i++)
       	   	 {
			//daca litera nu se gaseste
			if( caut_litera(p,prefix[i]) == -1 ) 
		 	 {
		   		fprintf(f,"0\n");
		 	  	return;
		 	 }
			//altfel se avanseaza
		  	p = p->children[caut_litera(p,prefix[i])];
       	   	 }
	  //se cauta suma si numarul de cuvinte incepand de la nodul curent
	  suma_cuv(p,&suma,&nr_cuv,&lungime);
	 }
	
	float rez = (float)suma/nr_cuv;
	fprintf(f,"%.3f\n",rez);

}
void Eliberare(TNod arb)
{
	
	int i;
	//pentru fiecare fiu din arbore
	for(i=0;i<arb->nr_fii;i++)
	 {	
		//se avanaseaza recursiv 
		Eliberare(arb->children[i]);
		//se elibereaza fiul curent
		free(arb->children[i]);
		
 	  }
	//se elibereaza nodul tata
	free(arb->children);

}
int main(int *argc,char *argv[])
{


	int i=0,nr_operatii=0;;
	
	FILE *f = fopen(argv[1],"r");
	FILE *g = fopen(argv[2],"w+");
 
 	fscanf(f,"%d\n",&nr_operatii);
 	
	TNod arb = init();
	char *operatie;

	for(i=0; i < nr_operatii;i++)
	 {
		operatie = read(f);
		if( strcmp(operatie,"add") == 0)
		 {	
			char *o = read(f);
			insert(arb,o);
			free(o);
		 }
		if( strcmp(operatie,"remove") == 0)
		 {	
			int j=0;
			char *o = read(f);
			remove1(arb,o,&j);
			free(o);
		 }
		if( strcmp(operatie,"find") == 0)
		 {	
			char *o = read(f);	
			find(arb,o,g);
			free(o);
		 }
		if( strcmp(operatie,"find_longest_prefix") == 0)
		 {	
			char *o = read(f);
			find_prefix(arb,o,g);
			free(o);
		 }
		if( strcmp(operatie,"find_all_with_prefix") == 0)
		 {	
			char *o = read(f);
			find_words(arb,o,g);
			fprintf(g,"\n");
			free(o);
		 }
		if( strcmp(operatie,"mean_length") == 0)
		 {	
			char *o = read(f);
			mean_length(arb,o,g);
			free(o);
		 }
		
	   	free(operatie);
	 }
	
	Eliberare(arb);
	free(arb);
	fclose(f);
	fclose(g);
	
}

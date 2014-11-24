/*
 *  delaunay.c
 *  
 */

#include "delaunay.h"
#define Error(a)   Error(a,__LINE__,__FILE__)
#define Warning(a) Warning(a,  __LINE__, __FILE__)


void triangulation(char *FileName, const char *ResultName)
{
printf("on est dans triangulation \n");

Triangulation *theTriangulation = TriangulationCreate(FileName);// doit aussi initialiser le triangle p0,p-1,p-2
printf("X[0]: %f \n",   theTriangulation->points[0].x);// OK 
printf("Y[0]: %f \n",   theTriangulation->points[0].y);// OK

// TODO: ComputeRandom: un vecteur avec les nombres de 1 à n+1 mélangés pour voir dans quel ordre on ajoute les pts
// ce vecteur s'appelle de random. 
int *random = ComputeRandom(theTriangulation->nNode-1);

int i=0;
for(i=0; i<theTriangulation->nNode; i++)
{
	//AddPoint(theTriangulation->points[random[i]], theTriangulation);
	AddPoint(&(theTriangulation->points[random[i]]), theTriangulation);
}



// TODO: RemoveExtraPoints (enlever les points -1 et -2 ainsi que les edges qui les touchent)

TriangulationWrite(ResultName, theTriangulation);
TriangulationFree(theTriangulation);
}





//////////////////////////////////////////////////

int IsLegal(Edge *edge, Triangulation *theTriangulation)
{

return 1; // true
}
/////////////////////////////////////////////////////////////////////////////////
void AddPoint(Point *point, Triangulation *theTriangulation)
{
	triangle *trig = PointLocate(point,theTriangulation); // TODO : attention on attend un triangle mais après faudra 
						            // traiter le cas special ou on ajoute un point sur une edge ..
						            
	// Il faut faire quelques LegalizeEdge :-) TODO
	

}
////////////////////////////////////////////////////////////////////////////////////////////////
int *ComputeRandom(int n)
{// returns a vector of int value from 1 to n in a random order
int *tab= malloc(sizeof(int)*n);
int i;
for(i=0;i<n;i++)// valeur par defaut :-)
	{tab[i]=i+1;}
return tab; 
}
/////////////////////////////////////////////////////////////////////////////////////////////
triangle *PointLocate(Point *point,Triangulation *theTriangulation)
{// Après faudra pouvoir renvoyer si on est sur une edge ou un triangle ..

triangle *trig = &(theTriangulation->elem[0]);// par defaut, le premier triangle initialisé 
return trig; // trig est un pointeur vers un triangle du tableau elem de la structure theTriangulation :-)
}
////////////////////////////////////////////////////////////////////////////////////////////
void LegalizeEdge(double X, double Y, Edge *edge, Triangulation *theTriangulation)
{// TODO
	// est ce que c'est une bonne facon de faire de passer X et Y comme ça (c'est le point Pr)
	// ou bien on fait une structure pour un node .. ?
	if( IsLegal(edge,theTriangulation)==0 )// si c'est false
	{
	// 1) on switch
	// 2) on refait des LegalizeEdge 
	}

}
////////////////////////////////////////////////////////////////////////////////////////////////
Triangulation *TriangulationCreate(char *FileName)
{
// lire le fichier data :-)

Triangulation *theTriangulation = malloc(sizeof(Triangulation));

    int i,trash;
    
    FILE* file = fopen(FileName,"r");
    //if (file == NULL) Error("No data file !",35,"something");

    fscanf(file, "Number of nodes %d \n", &theTriangulation->nNode);
    int nNode=theTriangulation->nNode;
    
    theTriangulation->X = malloc(sizeof(double)*(nNode+2));// on met p-1 et p-2 à la fin du vecteur :-)
    theTriangulation->Y = malloc(sizeof(double)*(nNode+2));
    theTriangulation->points = malloc(sizeof(Point)*(nNode+2));
    for (i = 0; i < nNode; ++i) {
        fscanf(file,"%d : %le %le \n",&trash,&theTriangulation->X[i],&theTriangulation->Y[i]);
        theTriangulation->points[i].x=theTriangulation->X[i];// redondant, là on a les pts accesibles de 2 facons différentes
        theTriangulation->points[i].y=theTriangulation->Y[i];
         }
    theTriangulation->nElem = 1;    // valeur au départ
    
    findP0(theTriangulation);
        
    theTriangulation->elem  = malloc(sizeof(triangle)*(2*nNode -2));//TODO ajuster ce nombre normalement ce sont des bornes sup 
    theTriangulation->edges = malloc(sizeof(Edge)*(3*nNode -3));// sur la taille possible des trucs (k=0 Thm9.1)
    
    // TODO traiter p-1 et p-2 de manière symbolique 
	
	// init p-1 en (0.0 ,-1.0)
	theTriangulation->points[nNode].x= 0.0;
	theTriangulation->points[nNode].y=-1.0;
	
	// init p-2 en (-10.0 , 10.0 )
	theTriangulation->points[nNode+1].x=-10.0;
	theTriangulation->points[nNode+1].y= 10.0;
	
	// init le triangle[0]	
	theTriangulation->elem[0].nodes[0]=0;
        theTriangulation->elem[0].nodes[1]=nNode;
        theTriangulation->elem[0].nodes[2]=nNode+1;	
        
    fclose(file);    
    return theTriangulation;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void findP0(Triangulation *theTriangulation)
{

//1) on cherche l'indice de P0
int ind=0;
int i;
for(i=1;i<theTriangulation->nNode; i++)
{
	if(theTriangulation->points[i].y > theTriangulation->points[ind].y)
	{
		ind =i;
	}
	else if(theTriangulation->points[i].y == theTriangulation->points[ind].y && theTriangulation->points[i].x > theTriangulation->points[ind].x ){
		ind = i;
	}
}

// 2) switch pour mettre P0 devant :-)
Point trash = theTriangulation->points[0]; 
theTriangulation->points[0]=theTriangulation->points[ind];
theTriangulation->points[ind]=trash;

}



void TriangulationFree(Triangulation *theTriangulation)
{
    free(theTriangulation->X);
    free(theTriangulation->Y);
    free(theTriangulation->edges);
    free(theTriangulation->points);
    free(theTriangulation->elem);
    free(theTriangulation);
}

void TriangulationWrite(const char *ResultName,Triangulation *theTriangulation)
{
// ecrit la solution :-)

}

void test(int n)
{
printf("test:  %d \n",n);
}






/*
femMesh *femMeshRead(const char *filename)
{
    femMesh *theMesh = malloc(sizeof(femMesh));

    int i,trash,*elem;
    
    FILE* file = fopen(filename,"r");
    if (file == NULL) Error("No mesh file !");

    fscanf(file, "Number of nodes %d \n", &theMesh->nNode);
    theMesh->X = malloc(sizeof(double)*theMesh->nNode);
    theMesh->Y = malloc(sizeof(double)*theMesh->nNode);
    for (i = 0; i < theMesh->nNode; ++i) {
        fscanf(file,"%d : %le %le \n",&trash,&theMesh->X[i],&theMesh->Y[i]); }
    
    char str[256]; fgets(str, sizeof(str), file);
    if (!strncmp(str,"Number of triangles",19))  { 
        sscanf(str,"Number of triangles %d \n", &theMesh->nElem);
        theMesh->elem = malloc(sizeof(int)*3*theMesh->nElem);
        theMesh->nLocalNode = 3;
        for (i = 0; i < theMesh->nElem; ++i) {
            elem = &(theMesh->elem[i*3]);
            fscanf(file,"%d : %d %d %d\n", &trash,&elem[0],&elem[1],&elem[2]); }}
    else if (!strncmp(str,"Number of quads",15))  { 
        sscanf(str,"Number of quads %d \n", &theMesh->nElem);  
        theMesh->elem = malloc(sizeof(int)*4*theMesh->nElem);
        theMesh->nLocalNode = 4;
        for (i = 0; i < theMesh->nElem; ++i) {
            elem = &(theMesh->elem[i*4]);
        fscanf(file,"%d : %d %d %d %d\n", &trash,&elem[0],&elem[1],&elem[2],&elem[3]); }}
  
    fclose(file);
    return theMesh;
}

void femMeshFree(femMesh *theMesh)
{
    free(theMesh->X);
    free(theMesh->Y);
    free(theMesh->elem);
    free(theMesh);
}

void femMeshWrite(const femMesh *theMesh, const char *filename)
{
    int i,*elem;
    
    FILE* file = fopen(filename,"w");
    
    fprintf(file, "Number of nodes %d \n", theMesh->nNode);
    for (i = 0; i < theMesh->nNode; ++i) {
        fprintf(file,"%6d : %14.7e %14.7e \n",i,theMesh->X[i],theMesh->Y[i]); }
    
    if (theMesh->nLocalNode == 4) {
        fprintf(file, "Number of quads %d \n", theMesh->nElem);  
        for (i = 0; i < theMesh->nElem; ++i) {
            elem = &(theMesh->elem[i*4]);
            fprintf(file,"%6d : %6d %6d %6d %6d \n", i,elem[0],elem[1],elem[2],elem[3]);   }}
    else if (theMesh->nLocalNode == 3) {
        fprintf(file, "Number of triangles %d \n", theMesh->nElem);  
        for (i = 0; i < theMesh->nElem; ++i) {
            elem = &(theMesh->elem[i*3]);
            fprintf(file,"%6d : %6d %6d %6d \n", i,elem[0],elem[1],elem[2]);   }}
    
    fclose(file);
}
  */
  
  /*
void femEdgesPrint(femEdges *theEdges)
{
    int i;    
    for (i = 0; i < theEdges->nEdge; ++i) {
        printf("%6d : %4d %4d : %4d %4d \n",i,
               theEdges->edges[i].node[0],theEdges->edges[i].node[1],
               theEdges->edges[i].elem[0],theEdges->edges[i].elem[1]); }
}

void femEdgesFree(femEdges *theEdges)
{
    free(theEdges->edges);
    free(theEdges);
}
*/
/*
void Error(char *text, int line, char *file)                                  
{ 
    printf("\n-------------------------------------------------------------------------------- ");
    printf("\n  Error in %s at line %d : \n  %s\n", file, line, text);
    printf("--------------------------------------------------------------------- Yek Yek !! \n\n");
    exit(69);                                                 
}

void femWarning(char *text, int line, char *file)                                  
{ 
    printf("\n-------------------------------------------------------------------------------- ");
    printf("\n  Warning in %s at line %d : \n  %s\n", file, line, text);
    printf("--------------------------------------------------------------------- Yek Yek !! \n\n");                                              
}

*/


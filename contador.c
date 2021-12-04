//Conta quantos dos valores de um vetor estão dentro de um intervalo
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

long long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
double *vetor; //vetor de entrada com dimensao dim 
float limiteInferior; //Limite inferior do intervalo desejado
float limiteSuperior; //Limite superior do intervalo desejado

//fluxo das threads
void * tarefa(void * arg) {
   long int id = (long int) arg; //identificador da thread
   long int contadorLocal = 0.0; //variavel local da contador
   long int tamBloco = dim/nthreads;  //tamanho do bloco de cada thread 
   long int ini = id * tamBloco; //elemento inicial do bloco da thread
   
   long int fim; //elemento final(nao processado) do bloco da thread
   if(id == nthreads-1) fim = dim;
   else fim = ini + tamBloco; //trata o resto se houver
   
   //Conta elementos do bloco da thread que atendem ao intervalo desejado
   for(long int i=ini; i<fim; i++)
      if(limiteInferior < vetor[i] && vetor[i] < limiteSuperior){
      	contadorLocal = contadorLocal+1;
      }
   //retorna o contador
   pthread_exit((void *) contadorLocal); 
}

//fluxo principal
int main(int argc, char *argv[]) {
   double contadorSeq= 0; //Contador sequencial
   double contadorConc= 0; //Contador concorrente
   double ini, fim; //tomada de tempo
   pthread_t *tid; //identificadores das threads no sistema
   long int retorno; //valor de retorno das threads
   
    srand((unsigned int)time(NULL)); //Permite gerar floats aleatórios

   //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads, limite inferior, limite superior)
   if(argc < 5) {
       fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads> <limite inferior> <limite superior>\n", argv[0]);
       return 1; 
   }

   dim = atoll(argv[1]);
   nthreads = atoi(argv[2]);
   limiteInferior = atof(argv[3]);
   limiteSuperior = atof(argv[4]);
   
   if(limiteInferior > limiteSuperior) {
   	printf("Limite inferior é maior que limite superior. Não é possível calcular\n");
   	return 1;
   }
  
   //aloca o vetor de entrada
   vetor = (double*) malloc(sizeof(double)*dim);
   if(vetor == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //preenche o vetor de entrada
   for(long int i=0; i<dim; i++)
      vetor[i] = ((float)rand()/(float)(RAND_MAX)) * 100; //Gera floats aleatórios entre 0 e 100
  
   //Faz a contagem sequencialmente
   GET_TIME(ini);
   for(long int i=0; i<dim; i++){
   	if(limiteInferior < vetor[i] && vetor[i] < limiteSuperior){
   		contadorSeq++;
   	}
   }  
   GET_TIME(fim);
   printf("Tempo sequencial:  %lf\n", fim-ini);

   //Faz a contagem concorrentemente
   GET_TIME(ini);
   tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //criar as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
   }
   //aguardar o termino das threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), (void**) &retorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
  
      //Contagem global
      contadorConc += retorno;
   }
   GET_TIME(fim);
   printf("Tempo concorrente:  %lf\n", fim-ini);

   //exibir os resultados
   printf("Contador seq:  %.0lf\n", contadorSeq);
   printf("Contador conc: %.0lf\n", contadorConc);

   //libera as areas de memoria alocadas
   free(vetor);
   free(tid);

   return 0;
}

# Contador

O programa aceita 4 parâmetros de entrada: 
(int) DIM
(int) Quantidade de Threads
(float) Limite inferior
(float) Limite superior

O programa irá gerar um vetor de floats aleatórios de tamanho DIM.
Irá contar quantos elementos do vetor possuem valor entre limite inferior e superior
Em seguira irá repetir a contagem, mas de forma concorrente.

Abaixo estão medições realizadas em uma Máquina Virtual Rodando Linux com 4 processadores 

![image](https://user-images.githubusercontent.com/32869667/144722394-e7c9d24a-f141-45d9-b083-d129d8a2f1ec.png)

Como é possível perceber, vetor com dimensão da ordem de 10⁵ possuem desempenho sequencial melhor. O tempo gasto com a criação das threads não compensa a economia de tempo no cálculo

Para vetores com dimensão da ordem de 10⁷, há um ganho de desempenho, porém abaixo do esperado. É esperada uma aceleração de 2, quando executado com 2 threads e uma aceleração de 4, quando executado com 4 threads. Essa difereça também pode ser explicada com o tempo consumido para a criação das Threads.

No caso de vetores com dimensão da ordem de 10⁸ é possível observer um ganho de desempenho dentro do esperado.

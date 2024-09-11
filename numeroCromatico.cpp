#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

int **matriz = NULL; // ponteiro para a matriz de adjac�ncia
int *cores = NULL; // ponteiro para o vetor com as cores de cada v�rtice
bool *cor_disponivel = NULL; // ponteiro para o vetor com as cores dispon�veis para cada v�rtice
int numVertices = 0; // vari�veis para atribuir o n�mero de v�rtices
int maiorCor = 0; // vari�vel para atribuir o retorno da fun��o numeroCromatico()

// fun��o para ler a matriz do arquivo e pass�-la para a vari�vel matriz
void lerMatriz(const char *nome_arquivo){
    FILE *arquivo; // vari�vel para a leitura do arquivo
    char linha[1000]; 

    arquivo = fopen(nome_arquivo, "r"); // abrindo o arquivo
    if(arquivo == NULL){ // verifica se o arquivo foi aberto corretamente
        printf("\nERROR 404: N�o foi poss�vel abrir o arquivo.\n\n"); // se n�o, exibe mensagem de erro
        goto fecharArquivo; // e envia para linha de fechamento do arquivo
    }else{
    	printf("\nArquivo lido com sucesso.\n"); // se sim, tamb�m informa ao usu�rio
	}
	
	/* a fun��o fgets � respons�vel por fazer a leitura de cada linha completa do arquivo, uma
	por vez desde que o tamanho da linha esteja dentro do limite definido na vari�vel 'linha'.
	Os par�metros recebidos por essa fun��o s�o:
	linha = vari�vel que ser� armazenada a linha que estiver sendo lida
	sizeof(linha) = o tamanho m�ximo permitido para a cada linha 
	arquivo = ponteiro de onde dever� ler cada linha*/	
	// loop para identifica o n�mero de v�rtices
    while(fgets(linha, sizeof(linha), arquivo) != NULL){ // enquanto a linha lida n�o for nula
        numVertices++; // conta um novo v�rtice
    }
	
	// se os v�rtices excederem o limite de 100
	if(numVertices > 100){ 
    	printf("\nLimite de v�rtices excedido.\n"); // exibir� uma mensagem de aviso
    	goto fecharArquivo; // e enviar� para linha de uma nova exibi��o do menu
	}
		 
	// realocando o ponteiro da matriz com as dimens�es corretas, com base na quantidade de v�rtices identificados acima
    matriz = (int**)malloc(numVertices * sizeof(int*));
    cores = (int*)malloc(numVertices * sizeof(int)); // alocando o ponteiro do vetor das cores de cada v�rtice
    cor_disponivel = (bool *)malloc(numVertices * sizeof(bool)); // alocando o ponteiro das cores dispon�veis
	
	/* a fun��o fseek � respons�vel por redefinir a posi��o do ponteiro, nesse caso,
	como j� foi lido todo o arquivo para descobrir o n�mero de linhas e colunas,
	redefiniremos a posi��o dele para o in�cio do arquivo. Para isso, passaremos os
	seguintes par�metros para a fseek:
	arquivo = ponteiro do arquivo que ser� realizada a modifica��o
	0 = indica a posi��o 0 da linha, nesse caso, da 1� linha
	SEEK_SET = comando respons�vel por definir o in�cio do arquivo, ou seja, a 1� linha*/
    fseek(arquivo, 0, SEEK_SET); // redefinindo o ponteiro para o in�cio do arquivo, ou seja, para a posi��o 0
    
    // atribui��o da matriz do arquivo para a vari�vel matriz[numVertices][numVertices]
    for(int i = 0; i < numVertices; i++){
        matriz[i] =(int *)malloc(numVertices * sizeof(int));
        for(int j = 0; j < numVertices; j++){
            fscanf(arquivo, "%d", &matriz[i][j]);
        }
    }
fecharArquivo:
    fclose(arquivo); //fecha o arquivo
}

// fun��o para imprimir a matriz
void imprimirMatriz(){
    printf("\n======== MATRIZ PRESENTE NO TXT ========");
    for(int i = 0; i < numVertices; i++){
        printf("\n");
        for(int j = 0; j < numVertices; j++){
            printf("%d ", matriz[i][j]);
        }
    }
}

// fun��o para liberar a mem�ria alocada para a matriz, para as cores e redefinindo os v�rtices para 0
void liberarMemoria(){
    free(matriz);
    free(cores);
    free(cor_disponivel);
    numVertices = 0;
}

/* fun��o para verificar se um v�rtice � adjacente a outro. Recebe como par�metros:
verticeAtual: o �ndice do v�rtice que desejamos verificar se � adjacente ao atual
cor: cor do v�rtice que desejamos verificar se � adjacente ao atual*/
int verificarAdjacencia(int verticeAtual, int cor){ 
    for(int i = 0; i < numVertices; i++) {
        if(matriz[verticeAtual][i] && cor == cores[i]){
			/* 'matriz[verticeAtual][i]' acessa o espa�o da matriz que representaria a conex�o,
			ou seja, a exist�ncia de uma aresta entre 'verticeAtual', que representa o v�rtice
			atual, e 'i' representa o v�rtice que desejamos testar se � adjacente a ele.
			Sabendo disso, a condi��o do if ser� lida da seguinte maneira:
			se houver uma aresta entre 'verticeAtual' e 'i' e a cor desejada para atribuir ao
			v�rtice 'verticeAtual' for igual a cor do v�rtice 'i'*/
            return 0; // retornar� 0, indicando que h� uma adjac�ncia entre os v�rtices e que n�o � permitido color�-los com a mesma cor
        }
    }
    return 1;
}

/* fun��o recursiva para atribuir cores aos v�rtices do grafo.
Recebe o �ndice do v�rtice atual como par�metro.*/
int colorirGrafo(int verticeAtual){  
    if(verticeAtual == numVertices){ // se o �ndice do v�rtice atual for igual ao n�mero de numVertices, significa que j� coloriu todos os v�rtices
        return 1;// logo, encerrar� a colora��o de v�rtices
    } 
    for(int cor = 1; cor <= numVertices; cor++){
        if(verificarAdjacencia(verticeAtual, cor)){ 
            /* se o retorno da fun��o verificarAdjacencia() for 1, ou seja, se indicar que
			� permitido colorir tal v�rtice, logo, atribuir� a cor atual ao v�rtice*/
			cores[verticeAtual] = cor;
            if(colorirGrafo(verticeAtual + 1)) // recurs�o para verificar se todos os v�rtices j� foram coloridos
                return 1; // se sim, encerrar� a colora��o de v�rtices
            //cores[verticeAtual] = 0;
        }
    }
    return 0;
}

// fun��o para calcular o n�mero crom�tico do grafo
int numeroCromatico(){
	int maiorCor = -1; // vari�vel para atribuir o retorno da fun��o numeroCromatico()
	for(int i = 0; i < numVertices; i++){ // a cor de maior valor � equivalente ao n�mero crom�tico, logo,
	        if(cores[i] > maiorCor){ 
	            /* se a cor atual for maior que o valor atribu�do a vari�vel 'numeroCromatico', o valor de
				'numeroCromatico' passar� a ser igual ao valor da cor atual*/
	            maiorCor = cores[i];
	        }
	}
	return maiorCor;
}

/* backtracking para verificar se o n�mero crom�tico � o menor poss�vel ou se ainda � poss�vel reduz�-lo
Recebe o �ndice do v�rtice atual como par�metro.*/
int maiorCorBacktracking = numeroCromatico(); // nova vari�vel para atribuir o retorno da fun��o numeroCromatico() para ser usado na fun��o backtracking()
int backtracking(int verticeAtual){
	if(verticeAtual == numVertices){ // se o �ndice do v�rtice atual for igual ao n�mero de v�rtices, significa que j� coloriu todos os v�rtices
        // logo, descobrir� o n�mero crom�tico e encerrar� a colora��o de v�rtices
		return 1;	
	}
	
	for(int i = 0; i < numVertices; i++){
		/* 'matriz[verticeAtual][i]' acessa o espa�o da matriz que representaria a conex�o,
		ou seja, a exist�ncia de uma aresta entre 'verticeAtual', que representa o v�rtice
		atual, e 'i' que representa o v�rtice que desejamos testar se � adjacente a ele.
		Sabendo disso, a condi��o do if ser� lida da seguinte maneira:
		se houver uma aresta entre 'verticeAtual' e 'i' e j� tiver sido atribu�do alguma cor ao v�rtice i*/
		if(matriz[verticeAtual][i] && cores[i] != -1) 
			cor_disponivel[i] = false;		
	}
	
	for(int cor = 1; cor < maiorCorBacktracking; cor++){
		if(cor_disponivel[cor] = true){
			cores[verticeAtual] = cor;
			backtracking(verticeAtual + 1);
			cores[verticeAtual] = -1;	
		}
	}
}

void exibirResultado(){
    for(int i = 0; i < numVertices; i++){
        cores[i] = -1; // inicializando as cores de todos os v�rtices com 0
        cor_disponivel[i] = true; // inicializando as cores dispon�veis como verdadeiras
    }
    
    colorirGrafo(0);
    backtracking(0);
    // exibindo o n�mero crom�tico e as cores de cada v�rtice
    printf("\n\n============= N� CROM�TICO =============");
    printf("\nO n�mero crom�tico do grafo lido � %d.\n", numeroCromatico());
    printf("\n========= COR DE CADA V�RTICE ==========\n");
    for(int i = 0; i < numVertices; i++){
        printf("V�rtice %d: cor %d\n", i + 1, cores[i]);
    }
}

char menu(){ // fun��o para saber se o usu�rio deseja ler um arquivo
	char retorno_menu[100]; // vari�vel para atribuir a resposta do usu�rio
	printf("================= MENU ================="
	"\n1 - Sim\n0 - N�o"
	"\n\nDeseja fazer a leitura de um arquivo? ");
	fgets(retorno_menu, sizeof(retorno_menu), stdin);
	return retorno_menu[0]; // retorna apenas o primeiro caractere da string
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    char nome_arquivo[50]; // vari�vel para leitura do nome do arquivo
    char txt[] = ".txt"; // vari�vel para concatena��o com o nome do arquivo que ser� informado pelo usu�rio
    char deseja_ler = menu(); // atribuindo a fun��o menu() para a vari�vel deseja_ler
	
	// de acordo com o retorno de menu, entrar� ou n�o no loop
    if(deseja_ler == '0'){ // se o retorno de menu() for 0, ou seja, se o usu�rio n�o desejar ler nenhum arquivo, encerrar� o programa
        printf("\nPrograma encerrado.\n");
        return 0;
    }else if(deseja_ler != '1'){ // se n�o for 0 e nem 1, exibe mensagem de erro e exibe o menu novamente
        printf("\nOp��o inv�lida. Tente novamente.\n\n");
        goto menu;
    }

    while(deseja_ler == '1'){ // se o retorno de menu() for 1, ou seja, se o usu�rio desejar ler um arquivo, entrar� no loop
        fflush(stdin); // limpa o buffer de entrada
        printf("\nInforme o nome do arquivo a ser lido: "); // solicita��o do nome do arquivo
        gets(nome_arquivo);
        strcat(nome_arquivo, txt); // fun��o para unir 2 strings, no caso, o nome do arquivo solicitado com a sua extens�o ".txt"
        lerMatriz(nome_arquivo); // chamada da fun��o lerMatriz()
        
        if(numVertices == 0){ // se o arquivo estiver vazio ou n�o for lido, exibir� o menu novamente
            goto menu;
        }else{ // sen�o, far� a chamada das fun��es imprimirMatriz(), colorirGrafo() e liberarMemoria(), respectivamente
        	imprimirMatriz();
			exibirResultado();
        	liberarMemoria();
		}

        printf("\n");
        menu:
        deseja_ler = menu(); // exibindo o menu novamente
        if(deseja_ler == '0'){ // se o retorno de menu() for 0, ou seja, se o usu�rio n�o desejar ler nenhum arquivo, encerrar� o programa
            printf("\nPrograma encerrado.");
        }else if(deseja_ler != '1'){ // se n�o for 0 e nem 1, exibe mensagem de erro e exibe o menu novamente
            printf("\nOp��o inv�lida. Tente novamente.\n\n"); 
            goto menu;
        }
    }
    return 0;
}


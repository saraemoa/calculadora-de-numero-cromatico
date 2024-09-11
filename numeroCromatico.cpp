#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

int **matriz = NULL; // ponteiro para a matriz de adjacência
int *cores = NULL; // ponteiro para o vetor com as cores de cada vértice
bool *cor_disponivel = NULL; // ponteiro para o vetor com as cores disponíveis para cada vértice
int numVertices = 0; // variáveis para atribuir o número de vértices
int maiorCor = 0; // variável para atribuir o retorno da função numeroCromatico()

// função para ler a matriz do arquivo e passá-la para a variável matriz
void lerMatriz(const char *nome_arquivo){
    FILE *arquivo; // variável para a leitura do arquivo
    char linha[1000]; 

    arquivo = fopen(nome_arquivo, "r"); // abrindo o arquivo
    if(arquivo == NULL){ // verifica se o arquivo foi aberto corretamente
        printf("\nERROR 404: Não foi possível abrir o arquivo.\n\n"); // se não, exibe mensagem de erro
        goto fecharArquivo; // e envia para linha de fechamento do arquivo
    }else{
    	printf("\nArquivo lido com sucesso.\n"); // se sim, também informa ao usuário
	}
	
	/* a função fgets é responsável por fazer a leitura de cada linha completa do arquivo, uma
	por vez desde que o tamanho da linha esteja dentro do limite definido na variável 'linha'.
	Os parâmetros recebidos por essa função são:
	linha = variável que será armazenada a linha que estiver sendo lida
	sizeof(linha) = o tamanho máximo permitido para a cada linha 
	arquivo = ponteiro de onde deverá ler cada linha*/	
	// loop para identifica o número de vértices
    while(fgets(linha, sizeof(linha), arquivo) != NULL){ // enquanto a linha lida não for nula
        numVertices++; // conta um novo vértice
    }
	
	// se os vértices excederem o limite de 100
	if(numVertices > 100){ 
    	printf("\nLimite de vértices excedido.\n"); // exibirá uma mensagem de aviso
    	goto fecharArquivo; // e enviará para linha de uma nova exibição do menu
	}
		 
	// realocando o ponteiro da matriz com as dimensões corretas, com base na quantidade de vértices identificados acima
    matriz = (int**)malloc(numVertices * sizeof(int*));
    cores = (int*)malloc(numVertices * sizeof(int)); // alocando o ponteiro do vetor das cores de cada vértice
    cor_disponivel = (bool *)malloc(numVertices * sizeof(bool)); // alocando o ponteiro das cores disponíveis
	
	/* a função fseek é responsável por redefinir a posição do ponteiro, nesse caso,
	como já foi lido todo o arquivo para descobrir o número de linhas e colunas,
	redefiniremos a posição dele para o início do arquivo. Para isso, passaremos os
	seguintes parâmetros para a fseek:
	arquivo = ponteiro do arquivo que será realizada a modificação
	0 = indica a posição 0 da linha, nesse caso, da 1ª linha
	SEEK_SET = comando responsável por definir o início do arquivo, ou seja, a 1ª linha*/
    fseek(arquivo, 0, SEEK_SET); // redefinindo o ponteiro para o início do arquivo, ou seja, para a posição 0
    
    // atribuição da matriz do arquivo para a variável matriz[numVertices][numVertices]
    for(int i = 0; i < numVertices; i++){
        matriz[i] =(int *)malloc(numVertices * sizeof(int));
        for(int j = 0; j < numVertices; j++){
            fscanf(arquivo, "%d", &matriz[i][j]);
        }
    }
fecharArquivo:
    fclose(arquivo); //fecha o arquivo
}

// função para imprimir a matriz
void imprimirMatriz(){
    printf("\n======== MATRIZ PRESENTE NO TXT ========");
    for(int i = 0; i < numVertices; i++){
        printf("\n");
        for(int j = 0; j < numVertices; j++){
            printf("%d ", matriz[i][j]);
        }
    }
}

// função para liberar a memória alocada para a matriz, para as cores e redefinindo os vértices para 0
void liberarMemoria(){
    free(matriz);
    free(cores);
    free(cor_disponivel);
    numVertices = 0;
}

/* função para verificar se um vértice é adjacente a outro. Recebe como parâmetros:
verticeAtual: o índice do vértice que desejamos verificar se é adjacente ao atual
cor: cor do vértice que desejamos verificar se é adjacente ao atual*/
int verificarAdjacencia(int verticeAtual, int cor){ 
    for(int i = 0; i < numVertices; i++) {
        if(matriz[verticeAtual][i] && cor == cores[i]){
			/* 'matriz[verticeAtual][i]' acessa o espaço da matriz que representaria a conexão,
			ou seja, a existência de uma aresta entre 'verticeAtual', que representa o vértice
			atual, e 'i' representa o vértice que desejamos testar se é adjacente a ele.
			Sabendo disso, a condição do if será lida da seguinte maneira:
			se houver uma aresta entre 'verticeAtual' e 'i' e a cor desejada para atribuir ao
			vértice 'verticeAtual' for igual a cor do vértice 'i'*/
            return 0; // retornará 0, indicando que há uma adjacência entre os vértices e que não é permitido colorí-los com a mesma cor
        }
    }
    return 1;
}

/* função recursiva para atribuir cores aos vértices do grafo.
Recebe o índice do vértice atual como parâmetro.*/
int colorirGrafo(int verticeAtual){  
    if(verticeAtual == numVertices){ // se o índice do vértice atual for igual ao número de numVertices, significa que já coloriu todos os vértices
        return 1;// logo, encerrará a coloração de vértices
    } 
    for(int cor = 1; cor <= numVertices; cor++){
        if(verificarAdjacencia(verticeAtual, cor)){ 
            /* se o retorno da função verificarAdjacencia() for 1, ou seja, se indicar que
			é permitido colorir tal vértice, logo, atribuirá a cor atual ao vértice*/
			cores[verticeAtual] = cor;
            if(colorirGrafo(verticeAtual + 1)) // recursão para verificar se todos os vértices já foram coloridos
                return 1; // se sim, encerrará a coloração de vértices
            //cores[verticeAtual] = 0;
        }
    }
    return 0;
}

// função para calcular o número cromático do grafo
int numeroCromatico(){
	int maiorCor = -1; // variável para atribuir o retorno da função numeroCromatico()
	for(int i = 0; i < numVertices; i++){ // a cor de maior valor é equivalente ao número cromático, logo,
	        if(cores[i] > maiorCor){ 
	            /* se a cor atual for maior que o valor atribuído a variável 'numeroCromatico', o valor de
				'numeroCromatico' passará a ser igual ao valor da cor atual*/
	            maiorCor = cores[i];
	        }
	}
	return maiorCor;
}

/* backtracking para verificar se o número cromático é o menor possível ou se ainda é possível reduzí-lo
Recebe o índice do vértice atual como parâmetro.*/
int maiorCorBacktracking = numeroCromatico(); // nova variável para atribuir o retorno da função numeroCromatico() para ser usado na função backtracking()
int backtracking(int verticeAtual){
	if(verticeAtual == numVertices){ // se o índice do vértice atual for igual ao número de vértices, significa que já coloriu todos os vértices
        // logo, descobrirá o número cromático e encerrará a coloração de vértices
		return 1;	
	}
	
	for(int i = 0; i < numVertices; i++){
		/* 'matriz[verticeAtual][i]' acessa o espaço da matriz que representaria a conexão,
		ou seja, a existência de uma aresta entre 'verticeAtual', que representa o vértice
		atual, e 'i' que representa o vértice que desejamos testar se é adjacente a ele.
		Sabendo disso, a condição do if será lida da seguinte maneira:
		se houver uma aresta entre 'verticeAtual' e 'i' e já tiver sido atribuído alguma cor ao vértice i*/
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
        cores[i] = -1; // inicializando as cores de todos os vértices com 0
        cor_disponivel[i] = true; // inicializando as cores disponíveis como verdadeiras
    }
    
    colorirGrafo(0);
    backtracking(0);
    // exibindo o número cromático e as cores de cada vértice
    printf("\n\n============= Nº CROMÁTICO =============");
    printf("\nO número cromático do grafo lido é %d.\n", numeroCromatico());
    printf("\n========= COR DE CADA VÉRTICE ==========\n");
    for(int i = 0; i < numVertices; i++){
        printf("Vértice %d: cor %d\n", i + 1, cores[i]);
    }
}

char menu(){ // função para saber se o usuário deseja ler um arquivo
	char retorno_menu[100]; // variável para atribuir a resposta do usuário
	printf("================= MENU ================="
	"\n1 - Sim\n0 - Não"
	"\n\nDeseja fazer a leitura de um arquivo? ");
	fgets(retorno_menu, sizeof(retorno_menu), stdin);
	return retorno_menu[0]; // retorna apenas o primeiro caractere da string
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    char nome_arquivo[50]; // variável para leitura do nome do arquivo
    char txt[] = ".txt"; // variável para concatenação com o nome do arquivo que será informado pelo usuário
    char deseja_ler = menu(); // atribuindo a função menu() para a variável deseja_ler
	
	// de acordo com o retorno de menu, entrará ou não no loop
    if(deseja_ler == '0'){ // se o retorno de menu() for 0, ou seja, se o usuário não desejar ler nenhum arquivo, encerrará o programa
        printf("\nPrograma encerrado.\n");
        return 0;
    }else if(deseja_ler != '1'){ // se não for 0 e nem 1, exibe mensagem de erro e exibe o menu novamente
        printf("\nOpção inválida. Tente novamente.\n\n");
        goto menu;
    }

    while(deseja_ler == '1'){ // se o retorno de menu() for 1, ou seja, se o usuário desejar ler um arquivo, entrará no loop
        fflush(stdin); // limpa o buffer de entrada
        printf("\nInforme o nome do arquivo a ser lido: "); // solicitação do nome do arquivo
        gets(nome_arquivo);
        strcat(nome_arquivo, txt); // função para unir 2 strings, no caso, o nome do arquivo solicitado com a sua extensão ".txt"
        lerMatriz(nome_arquivo); // chamada da função lerMatriz()
        
        if(numVertices == 0){ // se o arquivo estiver vazio ou não for lido, exibirá o menu novamente
            goto menu;
        }else{ // senão, fará a chamada das funções imprimirMatriz(), colorirGrafo() e liberarMemoria(), respectivamente
        	imprimirMatriz();
			exibirResultado();
        	liberarMemoria();
		}

        printf("\n");
        menu:
        deseja_ler = menu(); // exibindo o menu novamente
        if(deseja_ler == '0'){ // se o retorno de menu() for 0, ou seja, se o usuário não desejar ler nenhum arquivo, encerrará o programa
            printf("\nPrograma encerrado.");
        }else if(deseja_ler != '1'){ // se não for 0 e nem 1, exibe mensagem de erro e exibe o menu novamente
            printf("\nOpção inválida. Tente novamente.\n\n"); 
            goto menu;
        }
    }
    return 0;
}


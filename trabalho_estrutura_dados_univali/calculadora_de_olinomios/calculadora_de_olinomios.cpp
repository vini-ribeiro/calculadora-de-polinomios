#include <iostream>

#include <string>

using namespace std;

struct No {
	double constante;
	int expoente;
	No *proximo;
	No *anterior;
};

struct Lista {
	No *comeco;
	No *fim;
};

No* criarNo(double valorConstante, int valorExpoente)
{
	No *novoNo = new No;

	if (novoNo == NULL) return NULL;

	novoNo->anterior = NULL;
	novoNo->proximo = NULL;
	novoNo->constante = valorConstante;
	novoNo->expoente = valorExpoente;

	return novoNo;
}

void inicializarLista(Lista &lista)
{
	lista.comeco = NULL;
	lista.fim = NULL;
}

bool listaEstaVazia(Lista &lista)
{
	if (lista.comeco == NULL) return true;

	return false;
}

bool elementoExiste(Lista *lista, int grauMonomio)
{
	if (listaEstaVazia(*lista)) return false;

	No *aux = lista->comeco;

	while (aux != NULL) {
		if (aux->expoente == grauMonomio) return true;
		aux = aux->proximo;
	}

	return false;
}

No* removerDaLista(Lista *lista, int grauMonomio)
{
	if (listaEstaVazia(*lista)) return NULL;

	No *aux = lista->comeco;

	while (aux != NULL) {
		if (aux->expoente == grauMonomio) break;
		aux = aux->proximo;
	}

	if (aux == NULL) return NULL;

	if (lista->comeco->expoente == aux->expoente && lista->fim->expoente == aux->expoente) {
		lista->comeco = NULL;
		lista->fim = NULL;

		return aux;
	}

	if (lista->comeco->expoente == aux->expoente) {
		lista->comeco = aux->proximo;
		lista->comeco->anterior = NULL;

		aux->proximo = NULL;

		return aux;
	}

	if (lista->fim->expoente == aux->expoente) {
		lista->fim = aux->anterior;
		lista->fim->proximo = NULL;

		aux->anterior = NULL;

		return aux;
	}

	aux->anterior->proximo = aux->proximo;
	aux->proximo->anterior = aux->anterior;

	aux->anterior = NULL;
	aux->proximo = NULL;

	return aux;
}

bool inserirFimLista(Lista *lista, double cons, int exp)
{
	if (cons == 0.0) return true;

	No *novoElemento = criarNo(cons, exp);

	if (novoElemento == NULL) return false;

	if (listaEstaVazia(*lista)) {
		lista->comeco = novoElemento;
		lista->fim = novoElemento;

		return true;
	}

	novoElemento->anterior = lista->fim;
	lista->fim->proximo = novoElemento;
	lista->fim = novoElemento;

	return true;
}

bool inserirListaEmOrdem(Lista *lista, double cons, int exp)
{
	if (cons == 0.0) return true;

	if (listaEstaVazia(*lista)) {
		No *novoElemento = criarNo(cons, exp);

		if (novoElemento == NULL) return false;

		lista->comeco = novoElemento;
		lista->fim = novoElemento;

		return true;
	}

	No *aux = lista->comeco;

	while (aux != NULL) {
		if (aux->expoente <= exp) break;
		aux = aux->proximo;
	}

	if (aux == NULL) {
		return inserirFimLista(lista, cons, exp);
	}

	if (aux->expoente == exp) {
		aux->constante += cons;

		if (aux->constante == 0.0) removerDaLista(lista, exp);

		return true;
	}

	if (aux->expoente == lista->comeco->expoente) {
		No *novoElemento = criarNo(cons, exp);

		if (novoElemento == NULL) return false;

		novoElemento->proximo = aux;
		aux->anterior = novoElemento;
		lista->comeco = novoElemento;

		return true;
	}

	No *novoElemento = criarNo(cons, exp);

	if (novoElemento == NULL) return false;

	novoElemento->anterior = aux->anterior;
	novoElemento->proximo = aux;
	aux->anterior->proximo = novoElemento;
	aux->anterior = novoElemento;

	return true;
}

void imprimirLista(Lista *lista)
{
	No *aux = lista->comeco;
	while (aux != NULL) {
		cout << "(" << aux->constante << ")x^(" << aux->expoente << ") ";

		if (aux->proximo != NULL) cout << "+ ";

		aux = aux->proximo;
	}
}

No* retirarInicioLista(Lista *lista) /// FUNCAO UTILITARIA
{
	if (listaEstaVazia(*lista)) return NULL;

	if (lista->comeco == lista->fim) {
		No *elementoRetirar = lista->comeco;
		lista->comeco = NULL;
		lista->fim = NULL;

		return elementoRetirar;
	}

	No *elementoRetirar = lista->comeco;
	lista->comeco = lista->comeco->proximo;
	lista->comeco->anterior = NULL;
	elementoRetirar->proximo = NULL;

	return elementoRetirar;
}

/// FUNCOES RELACIONADAS COM AS OPERACOES COM POLINOMIOS
Lista* multiplicarPolinomios(Lista *lista1, Lista *lista2)
{
	Lista *produto = new Lista;

	inicializarLista(*produto);

	No *aux1 = lista1->comeco, *aux2;

	while (aux1 != NULL) {
		aux2 = lista2->comeco;
		while (aux2 != NULL) {
			inserirListaEmOrdem(produto, (aux1->constante) * (aux2->constante), (aux1->expoente) + (aux2->expoente));
			aux2 = aux2->proximo;
		}

		aux1 = aux1->proximo;
	}

	return produto;
}

/// FUNCOES RELACIONADAS COM AS OPERACOES COM POLINOMIOS

int main()
{
	Lista lista;

	inicializarLista(lista);

	inserirListaEmOrdem(&lista, -2, 0);
	inserirListaEmOrdem(&lista, 3, 1);
	/*inserirListaEmOrdem(&lista, 6, 2);
	inserirListaEmOrdem(&lista, 1, 4);
	inserirListaEmOrdem(&lista, 4, 3);
	inserirListaEmOrdem(&lista, 4, 3);*/

	Lista lista2;

	inicializarLista(lista2);

	inserirListaEmOrdem(&lista2, 2, 0);
	inserirListaEmOrdem(&lista2, 3, 1);
	inserirListaEmOrdem(&lista2, 5, 2);

	Lista *produto = multiplicarPolinomios(&lista, &lista2);

	/// REFAZER EM FORMA DE FUNÇÃO
	cout << "Produto de dois polinomios: ";
	imprimirLista(produto);

	return 0;
}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar:
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar
//   arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do
//   código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras
//   mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de
//   código, ou Projeto > Adicionar Item Existente para adicionar arquivos de
//   código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir >
//   Projeto e selecione o arquivo. sln

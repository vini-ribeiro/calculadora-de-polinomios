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
	if (lista == NULL) return false;

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
	if (lista == NULL) return NULL;

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
	if (lista == NULL) return false;

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
	if (lista == NULL) return false;

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
	if (lista == NULL) return ;

	No *aux = lista->comeco;

	if (listaEstaVazia(*lista)) cout << "0";

	while (aux != NULL) {
		cout << "(" << aux->constante << ")x^(" << aux->expoente << ") ";

		if (aux->proximo != NULL) cout << "+ ";

		aux = aux->proximo;
	}
}

No* retirarInicioLista(Lista *lista)
{
	if (lista == NULL) return NULL;

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
bool multiplicarPolinomios(Lista *produto, Lista *pol1, Lista *pol2)
{
	if (pol1 == NULL || pol2 == NULL || produto == NULL) return false;

	if (!listaEstaVazia(*produto)) return false;

	if (listaEstaVazia(*pol1) || listaEstaVazia(*pol2)) return true;

	No *aux1 = pol1->comeco, *aux2;

	while (aux1 != NULL) {
		aux2 = pol2->comeco;
		while (aux2 != NULL) {
			inserirListaEmOrdem(produto, (aux1->constante) * (aux2->constante), (aux1->expoente) + (aux2->expoente));
			aux2 = aux2->proximo;
		}

		aux1 = aux1->proximo;
	}

	return true;
}

bool subtrairDoisPolinomios(Lista *resultado, Lista *pol1, Lista *pol2)
{
	if (pol1 == NULL || pol2 == NULL || resultado == NULL) return false;

	if (!listaEstaVazia(*resultado)) return false;

	No *aux = pol1->comeco;

	while (aux != NULL) {
		inserirListaEmOrdem(resultado, aux->constante, aux->expoente);
		aux = aux->proximo;
	}

	aux = pol2->comeco;
	while (aux != NULL) {
		inserirListaEmOrdem(resultado, (aux->constante)*(-1), aux->expoente);
		aux = aux->proximo;
	}

	return true;
}

bool somarDoisPolinomios(Lista *resultado, Lista *pol1, Lista *pol2)
{
	if (pol1 == NULL || pol2 == NULL || resultado == NULL) return false;

	if (!listaEstaVazia(*resultado)) return false;

	No *aux = pol1->comeco;

	while (aux != NULL) {
		inserirListaEmOrdem(resultado, aux->constante, aux->expoente);
		aux = aux->proximo;
	}

	aux = pol2->comeco;
	while (aux != NULL) {
		inserirListaEmOrdem(resultado, (aux->constante), aux->expoente);
		aux = aux->proximo;
	}

	return true;
}

bool dividirPolinomios(Lista *resultado, Lista *pol1, Lista *pol2)
{
	if (pol1 == NULL || pol2 == NULL || resultado == NULL) return false;

	if (!listaEstaVazia(*resultado)) return false;

	if (listaEstaVazia(*pol2)) return false;

	if (listaEstaVazia(*pol1)) return true;

	if (pol1->comeco->expoente < pol2->comeco->expoente) return true;

	return true;
}
/// FUNCOES RELACIONADAS COM AS OPERACOES COM POLINOMIOS

int main()
{
	Lista lista;

	inicializarLista(lista);

	inserirListaEmOrdem(&lista, -2, 0);
	inserirListaEmOrdem(&lista, 1, 1);
	/*inserirListaEmOrdem(&lista, 6, 2);
	inserirListaEmOrdem(&lista, 1, 4);
	inserirListaEmOrdem(&lista, 4, 3);
	inserirListaEmOrdem(&lista, 4, 3);*/

	Lista lista2;

	inicializarLista(lista2);

	inserirListaEmOrdem(&lista2, 4, 0);
	inserirListaEmOrdem(&lista2, 2, 1);
	inserirListaEmOrdem(&lista2, 1, 2);

	cout << "A(x) = ";
	imprimirLista(&lista);
	cout << endl;
	cout << "B(x) = ";
	imprimirLista(&lista2);
	cout << endl;

	Lista *produto = new Lista;
	inicializarLista(*produto);

	multiplicarPolinomios(produto, &lista, &lista2);
	
	Lista *sub = new Lista;
	inicializarLista(*sub);

	subtrairDoisPolinomios(sub, &lista, &lista2);

	Lista *sum = new Lista;
	inicializarLista(*sum);

	somarDoisPolinomios(sum, &lista, &lista2);

	cout << "Produto de dois polinomios: ";
	imprimirLista(produto);
	cout << endl;

	cout << "Subtracao de dois polinomios: ";
	imprimirLista(sub);
	cout << endl;

	cout << "Soma de dois polinomios: ";
	imprimirLista(sum);
	cout << endl;

	return 0;
}
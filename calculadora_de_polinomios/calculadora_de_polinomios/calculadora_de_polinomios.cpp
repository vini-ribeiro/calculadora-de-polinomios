#include <iostream>
#include <string>

using namespace std;

/// FUNCOES RELACIONADAS COM O FUNCIONAMENTO DA LISTA ABAIXO
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
	/// funcao para alocar memoria e atribuir os valores ao No
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
	/// inicia as variaveis da lista
	lista.comeco = NULL;
	lista.fim = NULL;
}

bool listaEstaVazia(Lista &lista)
{
	/// retorna true se a lista estiver vazia (false caso nao esteja)
	if (lista.comeco == NULL) return true;

	return false;
}

bool elementoExiste(Lista *lista, int grauMonomio)
{
	/// retorna true caso exista um monomio com o mesmo grau de grauMonomio
	if (lista == NULL) return false;

	if (listaEstaVazia(*lista)) return false;

	No *aux = lista->comeco;

	/// varre a lista ate encontrar
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

	if (exp < 0) return false;

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
	/// os proximos dois ifs verificam se os argumentos são validos
	if (lista == NULL) return false;

	if (exp < 0) return false;

	/// caso especial de um monomio (a constante zerada é como se não existisse)
	if (cons == 0.0) return true;

	/// inserção de um elemento em uma lista vazia
	if (listaEstaVazia(*lista)) {
		No *novoElemento = criarNo(cons, exp);

		if (novoElemento == NULL) return false;

		lista->comeco = novoElemento;
		lista->fim = novoElemento;

		return true;
	}

	/// O while pesquisa o lugar do novo No na lista
	No *aux = lista->comeco;

	while (aux != NULL) {
		if (aux->expoente <= exp) break;
		aux = aux->proximo;
	}

	/// o novo No possui o menor expoente da lista
	if (aux == NULL) {
		return inserirFimLista(lista, cons, exp);
	}

	/// ja existe o um monomio com o mesmo expoente (somamos as duas constantes)
	if (aux->expoente == exp) {
		aux->constante += cons;

		/// caso a soma seja zero, excluimos o monomio da lista (qualquer numero multiplicado por zero da zero)
		if (aux->constante == 0.0) removerDaLista(lista, exp);

		return true;
	}

	/// o novo No tem o maior expoente e, por causa disso, precisamos inseri-lo de uma maneira diferente
	if (aux->expoente == lista->comeco->expoente) {
		No *novoElemento = criarNo(cons, exp);

		if (novoElemento == NULL) return false;

		novoElemento->proximo = aux;
		aux->anterior = novoElemento;
		lista->comeco = novoElemento;

		return true;
	}

	/// o novo No precisa ser inserido no meio e possui um expsoente diferente de qualquer outro da lista
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
	if (lista == NULL) return;

	No *aux = lista->comeco;

	/// esta funcao considera uma lista sem elementos como um polinomio nulo
	if (listaEstaVazia(*lista)) {
		cout << "0";
		return;
	}

	/// precorre a lista imprimindo cada monomio da maneira mais adequada
	while (aux != NULL) {
		if (aux->expoente > 1)
			cout << "(" << aux->constante << ")x^(" << aux->expoente << ") ";
		else if (aux->expoente == 1)
			cout << "(" << aux->constante << ")x ";
		else
			cout << "(" << aux->constante << ") ";

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
/// FUNCOES RELACIONADAS COM O FUNCIONAMENTO DA LISTA ACIMA

/// FUNCOES AUXILIARES ABAIXO
bool apagaPolinomio(Lista *polinomio)
{
	if (polinomio == NULL) return false;

	if (listaEstaVazia(*polinomio)) return true;

	No *aux = polinomio->comeco;
	No *monomio_apagar = aux;

	/// varre a lista encontrando os Nos e apagando cada um deles
	while (aux != NULL) {
		aux = aux->proximo;
		if (monomio_apagar != NULL) delete monomio_apagar;
		monomio_apagar = aux;
	}

	return true;
}

double potencia(double base, int expoente)
{
	/// funcao recursiva que calcula a potencia de um numero
	if (expoente >= 1) {
		return base * potencia(base, expoente - 1);
	}

	return 1;
}
/// FUNCOES AUXILIARES ACIMA

/// FUNCOES RELACIONADAS COM AS OPERACOES COM POLINOMIOS ABAIXO
bool multiplicarPolinomios(Lista *produto, Lista *pol1, Lista *pol2)
{
	/// verifica a validade dos argumentos
	if (pol1 == NULL || pol2 == NULL || produto == NULL) return false;

	/// a lista apontada por produto precisa
	if (!listaEstaVazia(*produto)) {
		if (!apagaPolinomio(produto)) return false;
	}

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

bool multiplicarPolinomios(Lista *produto, Lista *pol, double escalar)
{
	if (pol == NULL || produto == NULL) return false;

	if (!listaEstaVazia(*produto)) {
		if (!apagaPolinomio(produto)) return false;
	}

	if (listaEstaVazia(*pol)) return true;

	No *aux = pol->comeco;
	while (aux != NULL) {
		inserirListaEmOrdem(produto, (aux->constante) * escalar, (aux->expoente));

		aux = aux->proximo;
	}

	return true;
}

bool subtrairDoisPolinomios(Lista *resultado, Lista *pol1, Lista *pol2)
{
	if (pol1 == NULL || pol2 == NULL || resultado == NULL) return false;

	if (!listaEstaVazia(*resultado)) {
		if (!apagaPolinomio(resultado)) return false;
	}

	No *aux = pol1->comeco;

	while (aux != NULL) {
		inserirListaEmOrdem(resultado, aux->constante, aux->expoente);
		aux = aux->proximo;
	}

	aux = pol2->comeco;
	while (aux != NULL) {
		inserirListaEmOrdem(resultado, (aux->constante) * (-1), aux->expoente);
		aux = aux->proximo;
	}

	return true;
}

bool somarDoisPolinomios(Lista *resultado, Lista *pol1, Lista *pol2)
{
	if (pol1 == NULL || pol2 == NULL || resultado == NULL) return false;

	if (!listaEstaVazia(*resultado)) {
		if (!apagaPolinomio(resultado)) return false;
	}

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

bool dividirPolinomios(Lista *quociente, Lista *resto, Lista *dividendo, Lista *divisor)
{
	if (dividendo == NULL || divisor == NULL || quociente == NULL || resto == NULL) return false;

	if (!listaEstaVazia(*quociente)) {
		if (!apagaPolinomio(quociente)) return false;
	}

	if (!listaEstaVazia(*resto)) {
		if (!apagaPolinomio(resto)) return false;
	}

	if (listaEstaVazia(*divisor)) return false;

	if (listaEstaVazia(*dividendo)) return true;

	if (dividendo->comeco->expoente < divisor->comeco->expoente) {
		No *aux = dividendo->comeco;
		while (aux != NULL) {
			inserirListaEmOrdem(resto, aux->constante, aux->expoente);
			aux = aux->proximo;
		}

		return true;
	}

	double constante_quociente;
	int expoente_quociente;

	No *primeiro_monomio_divisor = divisor->comeco;
	No *primeiro_monomio_dividendo = dividendo->comeco;

	Lista *aux_resto = new Lista;
	inicializarLista(*aux_resto);

	do {
		constante_quociente = (double) (primeiro_monomio_dividendo->constante) / (primeiro_monomio_divisor->constante);
		expoente_quociente = (primeiro_monomio_dividendo->expoente) - (primeiro_monomio_divisor->expoente);

		inserirListaEmOrdem(quociente, constante_quociente, expoente_quociente);
		multiplicarPolinomios(aux_resto, divisor, quociente);
		subtrairDoisPolinomios(resto, dividendo, aux_resto);

		if (listaEstaVazia(*resto)) break;

		primeiro_monomio_dividendo = resto->comeco;
	} while (primeiro_monomio_dividendo->expoente >= primeiro_monomio_divisor->expoente);

	apagaPolinomio(aux_resto);
	delete aux_resto;

	return true;
}

double valorNumericoPolinomio(Lista *polinomio, double x)
{
	double fx = 0.0;
	No *aux = polinomio->comeco;
	while (aux != NULL) {
		fx += (aux->constante) * potencia(x, aux->expoente);
		aux = aux->proximo;
	}

	return fx;
}
/// FUNCOES RELACIONADAS COM AS OPERACOES COM POLINOMIOS ACIMA

int main()
{
	Lista Ax;

	inicializarLista(Ax);

	inserirListaEmOrdem(&Ax, 1, 2);
	inserirListaEmOrdem(&Ax, -2, 1);
	inserirListaEmOrdem(&Ax, 3, 0);
	inserirListaEmOrdem(&Ax, 1, 1);
	inserirListaEmOrdem(&Ax, -2, 0);

	Lista Bx;

	inicializarLista(Bx);

	inserirListaEmOrdem(&Bx, 6, 6);
	inserirListaEmOrdem(&Bx, 5, 5);
	inserirListaEmOrdem(&Bx, 2, 4);
	inserirListaEmOrdem(&Bx, -3, 3);
	inserirListaEmOrdem(&Bx, 0, 2);
	inserirListaEmOrdem(&Bx, 1, 1);
	inserirListaEmOrdem(&Bx, -1, 0);
	inserirListaEmOrdem(&Bx, 1, 2);
	inserirListaEmOrdem(&Bx, 2, 1);
	inserirListaEmOrdem(&Bx, 4, 0);

	cout << "A(x) = ";
	imprimirLista(&Ax);
	cout << endl;
	cout << "B(x) = ";
	imprimirLista(&Bx);
	cout << endl;

	///TESTE DE MULTIPLICACAO DOS POLINOMIOS
	Lista *produto = new Lista;
	inicializarLista(*produto);
	multiplicarPolinomios(produto, &Ax, &Bx);

	cout << "\nProduto de dois polinomios: A(x) * B(x) = ";
	imprimirLista(produto);
	cout << endl;

	///TESTE DE SUBTRACAO DOS POLINOMIOS
	Lista *sub = new Lista;
	inicializarLista(*sub);
	subtrairDoisPolinomios(sub, &Ax, &Bx);

	cout << "\nSubtracao de dois polinomios: A(x) - B(x) = ";
	imprimirLista(sub);
	cout << endl;

	///TESTE DE SOMA DOS POLINOMIOS
	Lista *sum = new Lista;
	inicializarLista(*sum);
	somarDoisPolinomios(sum, &Ax, &Bx);

	cout << "\nSoma de dois polinomios: A(x) + B(x) = ";
	imprimirLista(sum);
	cout << endl;

	///TESTE DE DIVISAO DE POLINOMIOS
	Lista *quociente = new Lista;
	Lista *resto = new Lista;
	inicializarLista(*quociente);
	inicializarLista(*resto);
	dividirPolinomios(quociente, resto, &Bx, &Ax);

	cout << "\nDivisao de dois polinomios: B(x)/A(x): ";
	cout << "\nQuociente: ";
	imprimirLista(quociente);
	cout << "\nResto: ";
	imprimirLista(resto);
	cout << endl;

	Lista *produtoEscalar = new Lista;
	inicializarLista(*produtoEscalar);
	multiplicarPolinomios(produtoEscalar, quociente, 5);

	cout << "\nProduto do quociente por um escalar: ";
	imprimirLista(produtoEscalar);
	cout << endl;

	cout << "\nEncontrando o valor numeico de um polinomio: \nB(-7) = " << valorNumericoPolinomio(&Bx, -7) << endl;

	return 0;
}
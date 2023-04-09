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
	///verifica se tudo ocorre bem, inicializa os ponteiros e atribui os valores as variaveis
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
	/// inicializa os ponteiros
	lista.comeco = NULL;
	lista.fim = NULL;
}

bool listaEstaVazia(Lista &lista)
{
	/// se nao tem ninguem no comeco retorna true
	if (lista.comeco == NULL) return true;

	return false;
}

bool elementoExiste(Lista *lista, int grauMonomio)
{
	/// os ifs verificam a validade dos argumentos
	if (lista == NULL) return false;

	if (listaEstaVazia(*lista)) return false;

	No *aux = lista->comeco;

	/// percorre a lista procurando pelo elemento
	while (aux != NULL) {
		if (aux->expoente == grauMonomio) return true;
		aux = aux->proximo;
	}

	return false;
}

No* removerDaLista(Lista *lista, int grauMonomio)
{
	/// os ifs verificam a validade dos argumentos
	if (lista == NULL) return NULL;

	if (listaEstaVazia(*lista)) return NULL;

	No *aux = lista->comeco;

	while (aux != NULL) {
		if (aux->expoente == grauMonomio) break;
		aux = aux->proximo;
	}

	/// se aux eh null entao o elemento nao existe
	if (aux == NULL) return NULL;

	/// a lista so tem um elemento
	if (lista->comeco->expoente == aux->expoente && lista->fim->expoente == aux->expoente) {
		lista->comeco = NULL;
		lista->fim = NULL;

		return aux;
	}

	/// faz a retirada de um elemento que esta no comeco
	if (lista->comeco->expoente == aux->expoente) {
		lista->comeco = aux->proximo;
		lista->comeco->anterior = NULL;

		aux->proximo = NULL;

		return aux;
	}

	/// faz a retirada de um elemento que esta no final
	if (lista->fim->expoente == aux->expoente) {
		lista->fim = aux->anterior;
		lista->fim->proximo = NULL;

		aux->anterior = NULL;

		return aux;
	}

	/// faz a retirada de um elemto que esta no meio 
	aux->anterior->proximo = aux->proximo;
	aux->proximo->anterior = aux->anterior;

	aux->anterior = NULL;
	aux->proximo = NULL;

	return aux;
}

bool inserirFimLista(Lista *lista, double cons, int exp)
{
	/// os ifs verificam a validade dos argumentos
	if (lista == NULL) return false;

	if (exp < 0 || cons > 1000) return false;

	if (cons == 0.0) return true;

	No *novoElemento = criarNo(cons, exp);

	if (novoElemento == NULL) return false;

	/// insere numa lista vazia
	if (listaEstaVazia(*lista)) {
		lista->comeco = novoElemento;
		lista->fim = novoElemento;

		return true;
	}

	/// insere numa lista que nao esta vazia
	novoElemento->anterior = lista->fim;
	lista->fim->proximo = novoElemento;
	lista->fim = novoElemento;

	return true;
}

bool inserirListaEmOrdem(Lista *lista, double cons, int exp)
{
	/// os ifs verificam a validade dos argumentos
	if (lista == NULL) return false;

	if (exp < 0 || cons > 1000) return false;

	if (cons == 0.0) return true;

	if (listaEstaVazia(*lista)) {
		No *novoElemento = criarNo(cons, exp);

		if (novoElemento == NULL) return false;

		lista->comeco = novoElemento;
		lista->fim = novoElemento;

		return true;
	}

	No *aux = lista->comeco;

	/// procura o lugar do novo elemento na lista
	while (aux != NULL) {
		if (aux->expoente <= exp) break;
		aux = aux->proximo;
	}

	/// o elemento tem o menor grau
	if (aux == NULL) {
		return inserirFimLista(lista, cons, exp);
	}

	/// ja existe um elemento com o mesmo grau que o novo (apenas somamos as constantes e, se der zero, excluimos da lista)
	if (aux->expoente == exp) {
		aux->constante += cons;

		if (aux->constante == 0.0) removerDaLista(lista, exp);

		return true;
	}

	/// o elemento tem o maior grau
	if (aux->expoente == lista->comeco->expoente) {
		No *novoElemento = criarNo(cons, exp);

		if (novoElemento == NULL) return false;

		novoElemento->proximo = aux;
		aux->anterior = novoElemento;
		lista->comeco = novoElemento;

		return true;
	}

	/// elemento vai no meio da lista
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

	if (listaEstaVazia(*lista)) {
		cout << "0";
		return;
	}

	/// percorre a lista imprimindo os elementos
	while (aux != NULL) {
		if (aux->expoente > 1) /// imprime monomio completo caso seja maior que 1
			cout << "(" << aux->constante << ")x^(" << aux->expoente << ") ";
		else if (aux->expoente == 1) /// imprime somente a constante e a variavel
			cout << "(" << aux->constante << ")x ";
		else /// imprime somente a constante
			cout << "(" << aux->constante << ") ";

		/// imprime o sinal de operacao ate o penultimo monomio
		if (aux->proximo != NULL) cout << "+ ";

		aux = aux->proximo;
	}
}
/// FUNCOES RELACIONADAS COM O FUNCIONAMENTO DA LISTA ACIMA

/// FUNCOES AUXILIARES ABAIXO
bool apagaPolinomio(Lista *polinomio)
{
	/// os ifs verificam a validade dos argumentos
	if (polinomio == NULL) return false;

	if (listaEstaVazia(*polinomio)) return true;

	No *monomio_removido;
	int i = 0;

	/// procura os monomios com cada indice de 0 a 1000 (lmite para o programa)
	while (!listaEstaVazia(*polinomio) && i <= 1000) {
		if (elementoExiste(polinomio, i)) {
			monomio_removido = removerDaLista(polinomio, i);
			delete monomio_removido;
		}
		++i;
	}

	return true;
}

double potencia(double base, int expoente)
{
	if (expoente >= 1) {
		return base * potencia(base, expoente - 1);
	}

	return 1;
}
/// FUNCOES AUXILIARES ACIMA

/// FUNCOES RELACIONADAS COM AS OPERACOES COM POLINOMIOS ABAIXO
bool multiplicarPolinomios(Lista *produto, Lista *pol1, Lista *pol2)
{
	/// os ifs verificam a validade dos argumentos
	if (pol1 == NULL || pol2 == NULL || produto == NULL) return false;

	if (!listaEstaVazia(*produto)) {
		if (!apagaPolinomio(produto)) return false;
	}

	if (listaEstaVazia(*pol1) || listaEstaVazia(*pol2)) return true;

	No *aux1 = pol1->comeco, *aux2;

	/// faz a permutacao dos polinomios guardando cada resultado na lista produto
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
	/// os ifs verificam a validade dos argumentos
	if (pol == NULL || produto == NULL) return false;

	if (!listaEstaVazia(*produto)) {
		if (!apagaPolinomio(produto)) return false;
	}

	if (listaEstaVazia(*pol)) return true;

	/// percorre a lista passando cada monomio multiplicado pelo escalar para a lista produto
	No *aux = pol->comeco;
	while (aux != NULL) {
		inserirListaEmOrdem(produto, (aux->constante) * escalar, (aux->expoente));

		aux = aux->proximo;
	}

	return true;
}

bool subtrairDoisPolinomios(Lista *resultado, Lista *pol1, Lista *pol2)
{
	/// os ifs verificam a validade dos argumentos
	if (pol1 == NULL || pol2 == NULL || resultado == NULL) return false;

	if (!listaEstaVazia(*resultado)) {
		if (!apagaPolinomio(resultado)) return false;
	}

	No *aux = pol1->comeco;

	/// passa pol1 para a lista resultado
	while (aux != NULL) {
		inserirListaEmOrdem(resultado, aux->constante, aux->expoente);
		aux = aux->proximo;
	}

	/// passa pol2 para a lista resultado com o sinal invertido (a funcao de insercao vai fazer o trabalho de subtrair)
	aux = pol2->comeco;
	while (aux != NULL) {
		inserirListaEmOrdem(resultado, (aux->constante) * (-1), aux->expoente);
		aux = aux->proximo;
	}

	return true;
}

bool somarDoisPolinomios(Lista *resultado, Lista *pol1, Lista *pol2)
{
	/// os ifs verificam a validade dos argumentos
	if (pol1 == NULL || pol2 == NULL || resultado == NULL) return false;

	if (!listaEstaVazia(*resultado)) {
		if (!apagaPolinomio(resultado)) return false;
	}

	No *aux = pol1->comeco;

	/// passa o pol1 para resultado
	while (aux != NULL) {
		inserirListaEmOrdem(resultado, aux->constante, aux->expoente);
		aux = aux->proximo;
	}

	/// passa o pol2 para resultado (a funcao de insercao ira somar os valres)
	aux = pol2->comeco;
	while (aux != NULL) {
		inserirListaEmOrdem(resultado, (aux->constante), aux->expoente);
		aux = aux->proximo;
	}

	return true;
}

bool dividirPolinomios(Lista *quociente, Lista *resto, Lista *dividendo, Lista *divisor)
{
	/// os ifs abaixo verificam a validade dos argumentos
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
	////// os ifs acima verificam a validade dos argumentos

	double constante_quociente;
	int expoente_quociente;

	No *primeiro_monomio_divisor = divisor->comeco;
	No *primeiro_monomio_dividendo = dividendo->comeco;

	Lista *aux_resto = new Lista;
	inicializarLista(*aux_resto);

	do {
		/// faz o calculo do monomio que vai no quociente
		constante_quociente = (double) (primeiro_monomio_dividendo->constante) / (primeiro_monomio_divisor->constante);
		expoente_quociente = (primeiro_monomio_dividendo->expoente) - (primeiro_monomio_divisor->expoente);

		/// insere o monomio no quociente. Depois multiplica pelo divisor e subtrai do dividendo
		/// os monomios sao guardados no quociente, sendo que o resto eh sempre apagado
		inserirListaEmOrdem(quociente, constante_quociente, expoente_quociente);
		multiplicarPolinomios(aux_resto, divisor, quociente);
		subtrairDoisPolinomios(resto, dividendo, aux_resto);

		/// caso a subtracao nao deixe resto, significa que encontramos o quociente
		if (listaEstaVazia(*resto)) break;

		/// sempre aponta para o comeco do resto (que muda a cada iteracao e na regra de divisao sempre temos ele como referencia)
		primeiro_monomio_dividendo = resto->comeco;

		/// verifica se da para continuar a divisao comparando os grau do resto com o do divisor
	} while (primeiro_monomio_dividendo->expoente >= primeiro_monomio_divisor->expoente);

	apagaPolinomio(aux_resto);
	delete aux_resto;

	return true;
}

double valorNumericoPolinomio(Lista *polinomio, double x)
{
	/// percorre a lista fazendo a potencia de x e depois multiplicando pela constante
	double fx = 0.0;
	No *aux = polinomio->comeco;
	while (aux != NULL) {
		fx += (aux->constante) * potencia(x, aux->expoente);
		aux = aux->proximo;
	}

	/// retorna a soma
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
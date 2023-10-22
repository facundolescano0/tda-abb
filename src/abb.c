#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>

struct vector_llenar {
	void **vector;
	size_t tope;
	size_t tamanio;
};

abb_t *abb_crear(abb_comparador comparador)
{
	if (comparador == NULL)
		return NULL;

	abb_t *abb = malloc(sizeof(abb_t));
	abb->comparador = comparador;
	abb->tamanio = 0;
	abb->nodo_raiz = NULL;
	return abb;
}

nodo_abb_t *crear_nodo(void *elemento)
{
	nodo_abb_t *nodo = malloc(sizeof(nodo_abb_t));
	nodo->elemento = elemento;
	nodo->izquierda = NULL;
	nodo->derecha = NULL;
	return nodo;
}

void *insertar_rec(nodo_abb_t *raiz, void *elemento, abb_comparador comparador)
{
	if (raiz == NULL) {
		nodo_abb_t *nodo_insertar = crear_nodo(elemento);
		if (!nodo_insertar)
			return NULL;
		return nodo_insertar;
	}
	int comparacion = comparador(elemento, raiz->elemento);
	if (comparacion <= 0) {
		raiz->izquierda =
			insertar_rec(raiz->izquierda, elemento, comparador);
	} else if (comparacion > 0)
		raiz->derecha =
			insertar_rec(raiz->derecha, elemento, comparador);
	return raiz;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;
	nodo_abb_t *nodo_insertar;
	if (arbol->nodo_raiz == NULL) {
		nodo_insertar = crear_nodo(elemento);
		arbol->nodo_raiz = nodo_insertar;

	} else {
		nodo_insertar = insertar_rec(arbol->nodo_raiz, elemento,
					     arbol->comparador);
	}
	if (!nodo_insertar)
		return NULL;
	arbol->tamanio++;
	return arbol;
}

nodo_abb_t *buscar_predecesor(nodo_abb_t *actual, void **predecesor)
{
	if (actual->derecha == NULL) {
		nodo_abb_t *izquierda = actual->izquierda;
		*predecesor = actual->elemento;
		free(actual);
		return izquierda;
	}
	actual->derecha = buscar_predecesor(actual->derecha, predecesor);
	return actual;
}

nodo_abb_t *quitar_recursivo(nodo_abb_t *raiz, void *elemento,
			     abb_comparador comparador, void **eliminado,
			     int *se_elimino)
{
	if (raiz == NULL)
		return NULL;
	int comparacion = comparador(elemento, raiz->elemento);
	if (comparacion == 0) {
		nodo_abb_t *izquierda = raiz->izquierda;
		nodo_abb_t *derecha = raiz->derecha;
		*eliminado = raiz->elemento;
		*se_elimino = 1;

		if (izquierda != NULL && derecha != NULL) {
			void *elemento_predecesor = NULL;
			raiz->izquierda = buscar_predecesor(
				raiz->izquierda, &elemento_predecesor);
			raiz->elemento = elemento_predecesor;
			return raiz;
		} else {
			free(raiz);
			if (izquierda == NULL)
				return derecha;
			return izquierda;
		}

	} else if (comparacion < 0)
		raiz->izquierda = quitar_recursivo(raiz->izquierda, elemento,
						   comparador, eliminado,
						   se_elimino);
	else
		raiz->derecha = quitar_recursivo(raiz->derecha, elemento,
						 comparador, eliminado,
						 se_elimino);

	return raiz;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;
	void *eliminado = NULL;
	int se_elimino = 0;
	arbol->nodo_raiz = quitar_recursivo(arbol->nodo_raiz, elemento,
					    arbol->comparador, &eliminado,
					    &se_elimino);
	if (se_elimino == 1)
		arbol->tamanio--;
	return eliminado;
}

void *buscar_rec(nodo_abb_t *raiz, void *elemento, abb_comparador comparador)
{
	if (!raiz) {
		return NULL;
	}
	int comparacion = comparador(elemento, raiz->elemento);
	if (comparacion < 0) {
		return buscar_rec(raiz->izquierda, elemento, comparador);
	} else if (comparacion > 0) {
		return buscar_rec(raiz->derecha, elemento, comparador);
	}
	return raiz->elemento;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol)
		return NULL;
	return buscar_rec(arbol->nodo_raiz, elemento, arbol->comparador);
}

bool abb_vacio(abb_t *arbol)
{
	if (!arbol || arbol->tamanio == 0)
		return true;
	return false;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol)
		return 0;
	return arbol->tamanio;
}

void abb_destruir_nodos(nodo_abb_t *raiz)
{
	if (!raiz)
		return;
	abb_destruir_nodos(raiz->izquierda);
	abb_destruir_nodos(raiz->derecha);
	free(raiz);
}

void abb_destruir(abb_t *arbol)
{
	if (!arbol)
		return;
	if (abb_tamanio(arbol) > 0)
		abb_destruir_nodos(arbol->nodo_raiz);
	free(arbol);
}

void abb_destruir_elemento(nodo_abb_t *raiz, void (*destructor)(void *))
{
	if (!raiz)
		return;
	abb_destruir_elemento(raiz->izquierda, destructor);
	abb_destruir_elemento(raiz->derecha, destructor);
	destructor(raiz->elemento);
	free(raiz);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol)
		return;
	if (abb_tamanio(arbol) > 0) {
		if (!destructor)
			abb_destruir_nodos(arbol->nodo_raiz);
		else
			abb_destruir_elemento(arbol->nodo_raiz, destructor);
	}
	free(arbol);
}

bool con_cada_inorder(nodo_abb_t *raiz, bool (*funcion)(void *, void *),
		      void *aux, size_t *contador)
{
	if (!raiz)
		return true;

	bool continuar =
		con_cada_inorder(raiz->izquierda, funcion, aux, contador);
	if (!continuar)
		return false;

	(*contador)++;
	if (!funcion(raiz->elemento, aux)) {
		return false;
	}

	return con_cada_inorder(raiz->derecha, funcion, aux, contador);
}
bool con_cada_preorder(nodo_abb_t *raiz, bool (*funcion)(void *, void *),
		       void *aux, size_t *contador)
{
	if (!raiz)
		return true;

	(*contador)++;
	if (!funcion(raiz->elemento, aux))
		return false;

	bool continuar =
		con_cada_preorder(raiz->izquierda, funcion, aux, contador);

	if (!continuar)
		return false;
	return con_cada_preorder(raiz->derecha, funcion, aux, contador);
}

bool con_cada_postorder(nodo_abb_t *raiz, bool (*funcion)(void *, void *),
			void *aux, size_t *contador)
{
	if (!raiz)
		return true;

	bool continuar =
		con_cada_postorder(raiz->izquierda, funcion, aux, contador);
	if (!continuar)
		return false;

	continuar = con_cada_postorder(raiz->derecha, funcion, aux, contador);

	(*contador)++;
	if (!funcion(raiz->elemento, aux)) {
		return false;
	}
	return continuar;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol || !funcion)
		return 0;
	size_t contador = 0;
	if (recorrido == INORDEN)
		con_cada_inorder(arbol->nodo_raiz, funcion, aux, &contador);

	else if (recorrido == PREORDEN)
		con_cada_preorder(arbol->nodo_raiz, funcion, aux, &contador);

	else if (recorrido == POSTORDEN)
		con_cada_postorder(arbol->nodo_raiz, funcion, aux, &contador);

	return contador;
}

bool llenar_vector(void *nuevo, void *vector)
{
	struct vector_llenar *vec = vector;
	if (vec->tamanio >= vec->tope)
		return false;
	vec->vector[vec->tamanio] = nuevo;
	vec->tamanio++;
	return true;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!arbol)
		return 0;
	struct vector_llenar aux;
	aux.vector = array;
	aux.tope = tamanio_array;
	aux.tamanio = 0;

	if (recorrido == INORDEN)
		abb_con_cada_elemento(arbol, recorrido, llenar_vector, &aux);

	else if (recorrido == PREORDEN)
		abb_con_cada_elemento(arbol, recorrido, llenar_vector, &aux);

	else if (recorrido == POSTORDEN)
		abb_con_cada_elemento(arbol, recorrido, llenar_vector, &aux);

	return aux.tamanio;
}

#include "pa2m.h"
#include "src/abb.h"
#include "src/abb_estructura_privada.h"

struct vector_tope {
	void **vector;
	int tope;
};

int comparador(void *actual, void *arbol)
{
	int *nuevo = actual;
	int *el_arbol = arbol;
	return *nuevo - *el_arbol;
}

bool seguir_iterando(void *elemento, void *raiz)
{
	int *actual = elemento;
	nodo_abb_t *nodo_actual = raiz;
	int *valor_raiz = (nodo_actual->elemento);
	int suma = *actual + *valor_raiz;
	if (suma < 200)
		return true;

	return false;
}

void pruebas_abb_comparador_null()
{
	int el_prueba = 0;
	void *array[5];
	abb_t *arbol = abb_crear(NULL);
	pa2m_afirmar(arbol == NULL, "arbol con comparador invalido es NULL");
	pa2m_afirmar(abb_tamanio(arbol) == 0, "arbol nulo tiene tamaño 0");
	pa2m_afirmar(abb_vacio(arbol), "arbol nulo esta vacio");
	pa2m_afirmar(abb_quitar(arbol, &el_prueba) == NULL,
		     "No se puede quitar de un arbol nulo");
	size_t iterador =
		abb_con_cada_elemento(arbol, INORDEN, seguir_iterando, NULL);
	pa2m_afirmar(iterador == 0, "No se puede iterar un arbol nulo");
	size_t recorridos = abb_recorrer(arbol, POSTORDEN, array, 2);
	pa2m_afirmar(recorridos == 0, "No se puede recorrer un arbol nulo");
	abb_destruir(arbol);
}

void pruebas_abb_vacio()
{
	int el_prueba = 0;
	void *array[5];
	abb_t *arbol = abb_crear(comparador);
	pa2m_afirmar(arbol != NULL, "arbol vacio es dintinto de NULL");
	pa2m_afirmar(abb_tamanio(arbol) == 0, "arbol vacio tiene tamaño 0");
	pa2m_afirmar(abb_vacio(arbol), "arbol vacio esta vacio");
	pa2m_afirmar(abb_quitar(arbol, &el_prueba) == NULL,
		     "No se puede quitar de un arbol vacio");
	size_t iterador = abb_con_cada_elemento(arbol, INORDEN, seguir_iterando,
						arbol->nodo_raiz);
	pa2m_afirmar(iterador == 0, "No se puede iterar un arbol vacio");
	size_t recorridos = abb_recorrer(arbol, POSTORDEN, array, 2);
	pa2m_afirmar(recorridos == 0, "No se puede recorrer un arbol vacio");
	abb_destruir(arbol);
}

void prueba_crear_abb()
{
	int elemento1 = 5;
	int elemento2 = 9;
	int elemento3 = 4;
	abb_t *arbol = abb_crear(comparador);
	pa2m_afirmar(arbol != NULL, "arbol credo es dintinto de NULL");
	pa2m_afirmar(abb_tamanio(arbol) == 0, "arbol credo tiene tamaño 0");
	pa2m_afirmar(abb_vacio(arbol), "arbol credo esta vacio");
	pa2m_afirmar(abb_insertar(arbol, &elemento1) != NULL,
		     "insertar no devuelve NULL");
	pa2m_afirmar(abb_tamanio(arbol) == 1, "arbol tiene tamaño 1");
	pa2m_afirmar(!abb_vacio(arbol), "arbol no esta vacio");
	pa2m_afirmar(abb_buscar(arbol, &elemento1) == &elemento1,
		     "elemento insertado es encontrado");
	pa2m_afirmar(abb_insertar(arbol, &elemento2) != NULL,
		     "insertar no devuelve NULL");
	pa2m_afirmar(abb_tamanio(arbol) == 2, "arbol tiene tamaño 2");
	pa2m_afirmar(abb_buscar(arbol, &elemento2) == &elemento2,
		     "elemento insertado es encontrado");
	pa2m_afirmar(abb_insertar(arbol, &elemento3) != NULL,
		     "insertar no devuelve NULL");
	pa2m_afirmar(abb_tamanio(arbol) == 3, "arbol tiene tamaño 3");
	pa2m_afirmar(abb_buscar(arbol, &elemento3) == &elemento3,
		     "elemento insertado es encontrado");
	pa2m_afirmar(abb_quitar(arbol, &elemento1) == &elemento1,
		     "quitar devuelve elemento borrado");
	pa2m_afirmar(abb_tamanio(arbol) == 2, "arbol tiene tamaño 2");
	pa2m_afirmar(abb_buscar(arbol, &elemento1) == NULL,
		     "elemento borrado no es encontrado");
	abb_destruir(arbol);
}

void destructor(void *elemento)
{
	return;
}

void prueba_destruir()
{
	int elemento1 = 10;
	int elemento2 = 9;
	int elemento3 = 4;
	int elemento4 = 15;
	int elemento5 = 13;
	int elemento6 = 17;
	int elemento7 = 1;
	int elemento8 = 11;
	abb_t *arbol = abb_crear(comparador);
	pa2m_afirmar(arbol != NULL, "se crea un arbol");
	abb_insertar(arbol, &elemento1);
	abb_insertar(arbol, &elemento2);
	abb_insertar(arbol, &elemento3);
	abb_insertar(arbol, &elemento4);
	abb_insertar(arbol, &elemento5);
	abb_insertar(arbol, &elemento6);
	abb_insertar(arbol, &elemento7);
	abb_insertar(arbol, &elemento8);
	pa2m_afirmar(abb_tamanio(arbol) == 8,
		     "se inserta elementos y se destruye");
	abb_destruir(arbol);

	abb_t *arbol2 = abb_crear(comparador);
	pa2m_afirmar(arbol2 != NULL, "se crea un arbol");
	abb_insertar(arbol2, &elemento1);
	abb_insertar(arbol2, &elemento2);
	abb_insertar(arbol2, &elemento3);
	abb_insertar(arbol2, &elemento4);
	abb_insertar(arbol2, &elemento5);
	abb_insertar(arbol2, &elemento6);
	abb_insertar(arbol2, &elemento7);
	abb_insertar(arbol2, &elemento8);
	pa2m_afirmar(abb_tamanio(arbol2) == 8,
		     "se inserta elementos y se destruye todo");
	abb_destruir_todo(arbol2, destructor);
}

bool llenar_vector_tope(void *elemento, void *array)
{
	struct vector_tope *vec = array;
	vec->vector[vec->tope] = elemento;
	vec->tope++;
	return true;
}

bool llenar_vector_corte(void *elemento, void *array)
{
	struct vector_tope *vec = array;
	vec->vector[vec->tope] = elemento;
	vec->tope++;
	if (vec->tope == 5) {
		return false;
	}
	return true;
}

void prueba_iterador_abb()
{
	void *array[10];
	struct vector_tope vec;
	vec.vector = array;
	vec.tope = 0;
	int elemento1 = 10;
	int elemento2 = 9;
	int elemento3 = 4;
	int elemento4 = 15;
	int elemento5 = 13;
	int elemento6 = 17;
	int elemento7 = 1;
	int elemento8 = 11;
	abb_t *arbol = abb_crear(comparador);
	abb_insertar(arbol, &elemento1);
	abb_insertar(arbol, &elemento2);
	abb_insertar(arbol, &elemento3);
	abb_insertar(arbol, &elemento4);
	abb_insertar(arbol, &elemento5);
	abb_insertar(arbol, &elemento6);
	abb_insertar(arbol, &elemento7);
	abb_insertar(arbol, &elemento8);
	size_t iterados =
		abb_con_cada_elemento(arbol, INORDEN, llenar_vector_tope, &vec);
	pa2m_nuevo_grupo("Pruebas iterador INORDEN");
	pa2m_afirmar(iterados == 8, "se iteraron todos los elementos");
	pa2m_afirmar((vec.vector[0]) == &elemento7,
		     "el primer elemento iterrado es correcto");
	pa2m_afirmar((vec.vector[1]) == &elemento3,
		     "el segundo elemento iterrado es correcto");
	pa2m_afirmar((vec.vector[2]) == &elemento2,
		     "el tercero elemento iterrado es correcto");
	pa2m_afirmar((vec.vector[3]) == &elemento1,
		     "el cuarto elemento iterrado es correcto");
	pa2m_afirmar((vec.vector[4]) == &elemento8,
		     "el quinto elemento iterrado es correcto");
	pa2m_afirmar((vec.vector[5]) == &elemento5,
		     "el sexto elemento iterrado es correcto");
	pa2m_afirmar((vec.vector[6]) == &elemento4,
		     "el septimo elemento iterrado es correcto");
	pa2m_afirmar((vec.vector[7]) == &elemento6,
		     "el octavo elemento iterrado es correcto");

	void *array_preorden[10];
	struct vector_tope vec_preorden;
	vec_preorden.vector = array_preorden;
	vec_preorden.tope = 0;
	iterados = abb_con_cada_elemento(arbol, PREORDEN, llenar_vector_tope,
					 &vec_preorden);
	pa2m_nuevo_grupo("Pruebas iterador PREORDEN");
	pa2m_afirmar(iterados == 8, "se iteraron todos los elementos");
	pa2m_afirmar((vec_preorden.vector[0]) == &elemento1,
		     "el primer elemento iterado es correcto");
	pa2m_afirmar((vec_preorden.vector[1]) == &elemento2,
		     "el segundo elemento iterado es correcto");
	pa2m_afirmar((vec_preorden.vector[2]) == &elemento3,
		     "el tercero elemento iterado es correcto");
	pa2m_afirmar((vec_preorden.vector[3]) == &elemento7,
		     "el cuarto elemento iterado es correcto");
	pa2m_afirmar((vec_preorden.vector[4]) == &elemento4,
		     "el quinto elemento iterado es correcto");
	pa2m_afirmar((vec_preorden.vector[5]) == &elemento5,
		     "el sexto elemento iterado es correcto");
	pa2m_afirmar((vec_preorden.vector[6]) == &elemento8,
		     "el septimo elemento iterado es correcto");
	pa2m_afirmar((vec_preorden.vector[7]) == &elemento6,
		     "el octavo elemento iterado es correcto");

	void *array_postorden[10];
	struct vector_tope vec_postorden;
	vec_postorden.vector = array_postorden;
	vec_postorden.tope = 0;
	iterados = abb_con_cada_elemento(arbol, POSTORDEN, llenar_vector_tope,
					 &vec_postorden);
	pa2m_nuevo_grupo("Pruebas iterador POSTORDEN");
	pa2m_afirmar(iterados == 8, "se iteraron todos los elementos");
	pa2m_afirmar((vec_postorden.vector[0]) == &elemento7,
		     "el primer elemento iterado es correcto");
	pa2m_afirmar((vec_postorden.vector[1]) == &elemento3,
		     "el segundo elemento iterado es correcto");
	pa2m_afirmar((vec_postorden.vector[2]) == &elemento2,
		     "el tercero elemento iterado es correcto");
	pa2m_afirmar((vec_postorden.vector[3]) == &elemento8,
		     "el cuarto elemento iterado es correcto");
	pa2m_afirmar((vec_postorden.vector[4]) == &elemento5,
		     "el quinto elemento iterado es correcto");
	pa2m_afirmar((vec_postorden.vector[5]) == &elemento6,
		     "el sexto elemento iterado es correcto");
	pa2m_afirmar((vec_postorden.vector[6]) == &elemento4,
		     "el septimo elemento iterado es correcto");
	pa2m_afirmar((vec_postorden.vector[7]) == &elemento1,
		     "el octavo elemento iterado es correcto");

	void *array_corte[10];
	struct vector_tope vec_corte;
	vec_corte.vector = array_corte;
	vec_corte.tope = 0;
	iterados = abb_con_cada_elemento(arbol, INORDEN, llenar_vector_corte,
					 &vec_corte);
	pa2m_nuevo_grupo("Pruebas iterador con funcion que corta");
	pa2m_afirmar(iterados == 5,
		     "se iteraron todos los elementos hasta su corte");

	abb_destruir(arbol);
}

void prueba_recorrer_abb()
{
	void *array[8];
	size_t tamanio_array = 8;
	int elemento1 = 10;
	int elemento2 = 9;
	int elemento3 = 4;
	int elemento4 = 15;
	int elemento5 = 13;
	int elemento6 = 17;
	int elemento7 = 1;
	int elemento8 = 11;
	abb_t *arbol = abb_crear(comparador);
	abb_insertar(arbol, &elemento1);
	abb_insertar(arbol, &elemento2);
	abb_insertar(arbol, &elemento3);
	abb_insertar(arbol, &elemento4);
	abb_insertar(arbol, &elemento5);
	abb_insertar(arbol, &elemento6);
	abb_insertar(arbol, &elemento7);
	abb_insertar(arbol, &elemento8);
	size_t recorridos = abb_recorrer(arbol, INORDEN, array, tamanio_array);
	pa2m_nuevo_grupo("Pruebas recorridos INORDEN");
	pa2m_afirmar(recorridos == 8, "se iteraron todos los elementos");
	pa2m_afirmar((array[0]) == &elemento7,
		     "el primer elemento iterrado es correcto");
	pa2m_afirmar((array[1]) == &elemento3,
		     "el segundo elemento iterrado es correcto");
	pa2m_afirmar((array[2]) == &elemento2,
		     "el tercero elemento iterrado es correcto");
	pa2m_afirmar((array[3]) == &elemento1,
		     "el cuarto elemento iterrado es correcto");
	pa2m_afirmar((array[4]) == &elemento8,
		     "el quinto elemento iterrado es correcto");
	pa2m_afirmar((array[5]) == &elemento5,
		     "el sexto elemento iterrado es correcto");
	pa2m_afirmar((array[6]) == &elemento4,
		     "el septimo elemento iterrado es correcto");
	pa2m_afirmar((array[7]) == &elemento6,
		     "el octavo elemento iterrado es correcto");

	void *array_preorden[8];
	recorridos =
		abb_recorrer(arbol, PREORDEN, array_preorden, tamanio_array);
	pa2m_nuevo_grupo("Pruebas recorridos PREORDEN");
	pa2m_afirmar(recorridos == 8, "se iteraron todos los elementos");
	pa2m_afirmar((array_preorden[0]) == &elemento1,
		     "el primer elemento iterado es correcto");
	pa2m_afirmar((array_preorden[1]) == &elemento2,
		     "el segundo elemento iterado es correcto");
	pa2m_afirmar((array_preorden[2]) == &elemento3,
		     "el tercero elemento iterado es correcto");
	pa2m_afirmar((array_preorden[3]) == &elemento7,
		     "el cuarto elemento iterado es correcto");
	pa2m_afirmar((array_preorden[4]) == &elemento4,
		     "el quinto elemento iterado es correcto");
	pa2m_afirmar((array_preorden[5]) == &elemento5,
		     "el sexto elemento iterado es correcto");
	pa2m_afirmar((array_preorden[6]) == &elemento8,
		     "el septimo elemento iterado es correcto");
	pa2m_afirmar((array_preorden[7]) == &elemento6,
		     "el octavo elemento iterado es correcto");

	void *array_postorden[8];
	recorridos =
		abb_recorrer(arbol, POSTORDEN, array_postorden, tamanio_array);
	pa2m_nuevo_grupo("Pruebas recorridos POSTORDEN");
	pa2m_afirmar(recorridos == 8, "se iteraron todos los elementos");
	pa2m_afirmar((array_postorden[0]) == &elemento7,
		     "el primer elemento iterado es correcto");
	pa2m_afirmar((array_postorden[1]) == &elemento3,
		     "el segundo elemento iterado es correcto");
	pa2m_afirmar((array_postorden[2]) == &elemento2,
		     "el tercero elemento iterado es correcto");
	pa2m_afirmar((array_postorden[3]) == &elemento8,
		     "el cuarto elemento iterado es correcto");
	pa2m_afirmar((array_postorden[4]) == &elemento5,
		     "el quinto elemento iterado es correcto");
	pa2m_afirmar((array_postorden[5]) == &elemento6,
		     "el sexto elemento iterado es correcto");
	pa2m_afirmar((array_postorden[6]) == &elemento4,
		     "el septimo elemento iterado es correcto");
	pa2m_afirmar((array_postorden[7]) == &elemento1,
		     "el octavo elemento iterado es correcto");

	void *array2[4];
	size_t tamanio_array2 = 4;
	recorridos = abb_recorrer(arbol, INORDEN, array2, tamanio_array2);
	pa2m_nuevo_grupo(
		"Pruebas recorrido con array menor a tamanio de arbol");
	pa2m_afirmar(recorridos == 4,
		     "la cantidad cargada es igual al tope del array");

	abb_destruir(arbol);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas crear abb, insertar y quitar");
	prueba_crear_abb();
	pa2m_nuevo_grupo("Pruebas destruir abb");
	prueba_destruir();
	pa2m_nuevo_grupo("Pruebas abb vacio");
	pruebas_abb_vacio();
	pa2m_nuevo_grupo("Pruebas abb con comparador invalido");
	pruebas_abb_comparador_null();
	pa2m_nuevo_grupo("Pruebas iterador");
	prueba_iterador_abb();
	pa2m_nuevo_grupo("Pruebas recorridos");
	prueba_recorrer_abb();

	return pa2m_mostrar_reporte();
}

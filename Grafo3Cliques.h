#include "BibGrafos.h"
#pragma once


/*###### INCOMPLETO -- EM DESENVOLVIMENTO #######*/
namespace BibGrafos
{
	class Grafo3Cliques : public GrafoCordal
	{
	public:
		Grafo3Cliques();
		Grafo3Cliques(int n);
		Grafo3Cliques(int n, int** matAdj);
		Grafo3Cliques(int n, int caso);

		static Grafo* ConstruirAleatorio(int n);

		void ConstruirCaso1(int n); //
		void ConstruirCaso2(int n); //
		void ConstruirCaso3(int n); //
		void ConstruirCaso4(int n); //
		//void ConstruirCaso5(int n); //

		void ConstruirGrafo(int n, bool H[7]);
		void ConstruirGrafo(int n, int R[7]);

		int identificarCaso();
	};
}

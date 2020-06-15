#include "BibGrafos.h"
#pragma once


namespace BibGrafos
{
	class ListaAdjacencia
	{
		//	map<Vertice,set<Vertice>,Vertice::comparator > adjs;	
		std::map<Vertice, std::set<Vertice> > adjs;
	public:
		void AdicionarVertice(Vertice v);
		void RemoverVertice(Vertice v);
		bool AdicionarAdjacencia(Vertice v, Vertice u);
		bool RemoverAdjacencia(Vertice v, Vertice u);
		void LerMatrizAdjacencia(int n, int**, set<Vertice> vertices);
		bool VerificarAdjacencia(Vertice v, Vertice u) const;
		set<Vertice> operator[](Vertice v) const;
		ListaAdjacencia();
		ListaAdjacencia(const ListaAdjacencia &obj);
		int M() const;
		int ObterGrauMaximo() const;
		int ObterGrauMinimo() const;
		vector<pair<Vertice,Vertice>> const ObterListaAdjacencias() const;
	};
}

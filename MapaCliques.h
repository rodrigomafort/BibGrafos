#include "BibGrafos.h"
#pragma once

namespace BibGrafos
{
	class MapaCliques
	{
	public:

		MapaCliques(Grafo* G);
		~MapaCliques();

		vector<set<Vertice>*> cliques;
		map<Vertice,set<Vertice>*> mapa;
	};
}

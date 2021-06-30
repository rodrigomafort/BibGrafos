#include "BibGrafos.h"

namespace BibGrafos
{
	MapaCliques::~MapaCliques()
	{
		while (cliques.size() > 0)
		{
			set<Vertice>* pt = cliques.front();
			cliques.erase(cliques.begin());
			delete(pt);
		}
	}

	MapaCliques::MapaCliques(Grafo* G)
	{
		Grafo* glocal = new Grafo(G);
		map<int,set<Vertice>> listaGraus;
		for (int i = 0; i < glocal->N(); i++)
		{
			set<Vertice> set;
			listaGraus.insert(make_pair(i,set));
		}
		map<Vertice,int> mapaGraus;
		for (Vertice v : glocal -> Vertices)
		{
			int gv = glocal->Grau(v);
			listaGraus[gv].insert(v);
			mapaGraus.insert(make_pair(v,gv));
		}

		while (glocal->Vertices.size() > 0)
		{
			while (listaGraus[0].size() > 0)
			{
				Vertice v = *(listaGraus[0].begin());

				set<Vertice>* clique = new set<Vertice>();
				clique->insert(v);
				cliques.push_back(clique);
				mapa.insert(make_pair(v,clique));

				for (Vertice u : glocal -> Adjacentes(v))
				{
					listaGraus[mapaGraus[u]].erase(u);
					mapaGraus[u] = mapaGraus[u] - 1;
					listaGraus[mapaGraus[u]].insert(u);
				}
				glocal->RemoverVertice(v);
				listaGraus[0].erase(v);
			}

			int i = 1;
			while (listaGraus[i].size() == 0) i++;

			Vertice v = *(listaGraus[i].begin()); //Grau mínimo

			set<Vertice>* clique = new set<Vertice>();

			clique->insert(v);
			mapa.insert(make_pair(v,clique));

			set<Vertice> NClique = glocal->Adjacentes(v);
			while (NClique.size() > 0)
			{
				map<Vertice,set<Vertice>> adjs;
				for (Vertice u : NClique)
				{
					set<Vertice> adjU = glocal -> Adjacentes(u);
					set<Vertice> comum;
					set_intersection(NClique.begin(), NClique.end(), adjU.begin(), adjU.end(), inserter(comum, comum.end()));
					adjs.insert(pair<Vertice,set<Vertice>>(u,comum));
				}
				vector<Vertice> ord(NClique.begin(), NClique.end());
				auto sortAdj = [&](Vertice a, Vertice b)-> bool {
					if (adjs[a].size() != adjs[b].size())
					{
						return adjs[a].size() > adjs[b].size();
					}
					else
					{
						return glocal->Grau(a) < glocal->Grau(b);
					}
				};
				sort(ord.begin(), ord.end(), sortAdj);

				Vertice w = ord[0];

				clique->insert(w);
				mapa.insert(make_pair(w,clique));

				NClique = adjs[w];
			}

			/*for (auto it=clique->begin(); it != clique->end(); ++it)
			{
				Vertice v = *it;
				for (auto it2=next(it); it2 != clique->end(); ++it2)
				{
					Vertice u = *it2;
					if (glocal->RemoverAdjacencia(v,u))
					{
						listaGraus[mapaGraus[u]].erase(u);
						mapaGraus[u] = mapaGraus[u] - 1;
						listaGraus[mapaGraus[u]].insert(u);
					}
				}
			}

			for (auto it=clique->begin(); it != clique->end(); ++it)
			{
				Vertice v = *it;
				if (glocal->Grau(v) == 0)
				{
					listaGraus[mapaGraus[v]].erase(v);
					glocal->RemoverVertice(v);
				}
			}*/


			for (auto it=clique->begin(); it != clique->end(); ++it)
			{
				Vertice u = *it;
				set<Vertice> adju = glocal -> Adjacentes(u);
				for (Vertice x : adju)
				{
					listaGraus[mapaGraus[x]].erase(x);
					mapaGraus[x] = mapaGraus[x] - 1;
					listaGraus[mapaGraus[x]].insert(x);
				}
			}

			for (auto it=clique->begin(); it != clique->end(); ++it)
			{
				Vertice u = *it;

				glocal->RemoverVertice(u);
				listaGraus[mapaGraus[u]].erase(u);
			}


			cliques.push_back(clique);
		}
	}
}


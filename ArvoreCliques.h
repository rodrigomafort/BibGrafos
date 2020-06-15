#include "BibGrafos.h"
#pragma once

namespace BibGrafos
{
	class NoClique
	{
		private:
			NoClique();
			NoClique(const set<Vertice> vertices, NoClique *ancestral);
			friend class ArvoreCliques;

		public:

			bool operator== (const NoClique& outro) const;
			bool operator!= (const NoClique& outro) const;
			bool operator< (const NoClique& outro) const;

			set<Vertice> vertices;

			NoClique* pai;
			vector<NoClique*> filhos;
	};

	class ArvoreCliques
	{
		public:
			vector<NoClique*> cliques;
			NoClique* raiz;

			ArvoreCliques();
			NoClique* BuscarNo(const set<Vertice> vertices);
			NoClique* AdicionarClique(set<Vertice> vertices, NoClique *pai);
			NoClique* AdicionarClique(set<Vertice> vertices, set<Vertice> ancestral);
			NoClique* AdicionarRaiz(set<Vertice> c);
			bool RemoverClique(NoClique* c);

			string ObterString();

			bool AjustarCaminho();
			void InverterCaminho();
	};
}

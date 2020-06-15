#include "BibGrafos.h"
namespace BibGrafos
{
	NoClique::NoClique()
	{
		pai = nullptr;
		filhos.clear();
	}

	NoClique::NoClique(const set<Vertice> cVertices, NoClique *ancestral)
	{
		vertices = cVertices;
		pai = ancestral;
		filhos.clear();
	}

	bool NoClique::operator== (const NoClique& outro) const
	{
		return vertices == outro.vertices;
	}

	bool NoClique::operator!= (const NoClique& outro) const
	{
		return !operator==(outro);
	}

	bool NoClique::operator< (const NoClique& outro) const
	{
		return vertices < outro.vertices;
	}

	ArvoreCliques::ArvoreCliques()
	{
		raiz = NULL;
	}

	NoClique* ArvoreCliques::BuscarNo(const set<Vertice> vertices)
	{
		for (NoClique* c : cliques)
		{
			if (c->vertices == vertices)
			{
				return c;
			}
		}
		return NULL;
	}

	NoClique* ArvoreCliques::AdicionarClique(set<Vertice> vertices, set<Vertice> ancestral)
	{
		NoClique *noAncestral = BuscarNo(ancestral);
		return AdicionarClique(vertices, noAncestral);
	}

	NoClique* ArvoreCliques::AdicionarClique(set<Vertice> vertices, NoClique *ancestral)
	{
		NoClique* c = new NoClique(vertices, ancestral);
		cliques.push_back(c);
		ancestral->filhos.push_back(c);
		return c;
	}

	NoClique* ArvoreCliques::AdicionarRaiz(set<Vertice> vertices)
	{
		NoClique* c = new NoClique(vertices, NULL);
		cliques.push_back(c);
		raiz = c;
		return c;
	}

	bool ArvoreCliques::RemoverClique(NoClique* c)
	{
		//Verificar se c não é ancestral de nenhuma clique
		for (NoClique* i : cliques)
		{
			if (i->pai == c)
			{
				return false;
			}
		}

		//clique c não é ancestral de nenhuma outra clique, logo pode ser removida
		cliques.erase(remove(cliques.begin(), cliques.end(), c), cliques.end());
		return true;
	}

	string ArvoreCliques::ObterString()
	{
		stringstream ss;

		queue<NoClique*> fila;
		fila.push(raiz);
		map<NoClique*,int> mapa;
		int i = 0;

		while (fila.empty() == false)
		{
			NoClique* c = fila.front();
			fila.pop();

			mapa.insert(make_pair(c,i));
			i = i + 1;

			ss << mapa[c] << " - " ;
			if (mapa[c] > 0)
			{
				ss << mapa[c->pai] << " - ";
			}
			else
			{
				ss << "-" << " - ";
			}
			ss << Grafo::ObterString(c->vertices);
			ss << endl;

			for (NoClique* f : c->filhos)
			{
				fila.push(f);
			}
		}

		return ss.str();
	}

	bool ArvoreCliques::AjustarCaminho()
	{
		//Transforma uma árvore com duas folhas em um caminho
		//		O
		//	   / \			==>		O-> O -> O -> O
		//	  O	  O
		//	 /
		// 	O
		int violaCaminho = 0;
		NoClique* no = raiz;
		while (no != NULL)
		{
			if (no->filhos.size() > 1)
			{
				violaCaminho++;
				break;
			}
			else
			{
				if (no->filhos.size() == 1)
				{
					no = no->filhos[0];
				}
				else
				{
					break;
				}
			}
		}


		if (violaCaminho > 1) //nenhum nó além da raiz pode ter dois filhos
		{
			return false;
		}
		else
		{
			if (violaCaminho == 0)
			{
				return true; //já é caminho
			}
			else
			{
				//Existe somente um nó com dois filhos
				if (violaCaminho == 1 && raiz->filhos.size() > 1)
				{
					NoClique* filho = raiz->filhos[0];

					while (filho!= NULL)
					{
						NoClique* prox = NULL;

						int i = 0;
						while (i < filho->filhos.size() && filho->filhos[i] == raiz) i++;
						if (i < filho->filhos.size()) prox = filho->filhos[i];

						i = 0;
						while (i < raiz->filhos.size() && raiz->filhos[i] != filho) i++;
						if (i < raiz->filhos.size())
						{
							raiz->filhos.erase(raiz->filhos.begin() + i);
						}

						raiz->pai = filho;
						filho->filhos.push_back(raiz);

						raiz = filho;
						filho = prox;
					}
					raiz->pai = NULL;
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}

	void ArvoreCliques::InverterCaminho()
	{
		if (AjustarCaminho() == true)
		{
			NoClique* filho = NULL;
			if (raiz->filhos.size() > 0) filho = raiz->filhos[0];

			while(filho != NULL)
			{
				NoClique* aux = NULL;
				if (filho->filhos.size() > 0) aux = filho->filhos[0];

				raiz->pai = filho;
				int i = 0;
				while (i < raiz->filhos.size() && raiz->filhos[i] != filho) i++;
				if (i < raiz->filhos.size()) raiz->filhos.erase(raiz->filhos.begin() + i);

				filho->filhos.clear();
				filho->filhos.push_back(raiz);

				raiz = filho;
				filho = aux;
			}
		}
	}
}

#include "BibGrafos.h"
namespace BibGrafos
{
	ListaAdjacencia::ListaAdjacencia()
	{

	}

	ListaAdjacencia::ListaAdjacencia(const ListaAdjacencia &obj) : adjs(obj.adjs.begin(), obj.adjs.end())
	{

	}

	void ListaAdjacencia::AdicionarVertice(Vertice v)
	{
		set<Vertice> cV;
		adjs[v] = cV;
	}

	void ListaAdjacencia::RemoverVertice(Vertice v)
	{
		for (Vertice u : adjs[v])
			adjs[u].erase(v);
		adjs.erase(v);
	}

	bool ListaAdjacencia::AdicionarAdjacencia(Vertice v, Vertice u)
	{
		pair<set<Vertice>::iterator,bool> r1 = adjs[v].insert(u);
		pair<set<Vertice>::iterator,bool> r2 = adjs[u].insert(v);
		return r1.second && r2.second;
	}

	bool ListaAdjacencia::RemoverAdjacencia(Vertice v, Vertice u)
	{
		if (binary_search(adjs[v].begin(), adjs[v].end(), u))
		{
			adjs[v].erase(u);
			adjs[u].erase(v);
			return true;
		}
		else
		{
			return false;
		}
	}

	static Vertice Find(set<Vertice> Vertices, int id)
	{
		set<Vertice>::iterator results = find_if(
			Vertices.begin(),
			Vertices.end(),
			[&](Vertice const& vertice) {
			return vertice.Id() == id;
		});
		return *results;
	}

	void ListaAdjacencia::LerMatrizAdjacencia(int n, int** adj, set<Vertice> vertices)
	{
		for (int i = 0; i < n; i++)
		{
			Vertice v = Find(vertices, i + 1);
			for (int j = 0; j < n; j++)
			{
				if (adj[i][j] == 1)
				{
					Vertice u = Find(vertices, j + 1);
					AdicionarAdjacencia(v, u);
				}
			}
		}
	}

	set<Vertice> ListaAdjacencia::operator[](const Vertice v) const
	{
		try
		{
			return adjs.at(v);
		}
		catch (...)
		{
			cout << "ERRO: ListaAdjacencia::Operador[] - " << v.Id() << endl;
			return set<Vertice>();
		}
	}

	bool ListaAdjacencia::VerificarAdjacencia(const Vertice v, const Vertice u) const
	{
		try
		{
			return (binary_search(adjs.at(v).cbegin(), adjs.at(v).cend(), u));
		}
		catch (...)
		{
			cout << "ERRO: ListaAdjacencia::VerificarAdjacencia() - " << v.Id() << " - " << u.Id() << endl;
			return false;
		}
	}

	int ListaAdjacencia::M() const
	{
		int ct = 0;
		for (pair<Vertice, set<Vertice>> p : adjs)
		{
			ct = ct + p.second.size();
		}
		return ct / 2;
	}

	int ListaAdjacencia::ObterGrauMaximo() const
	{
		int max = 0;
		for (auto p : adjs) if (p.second.size() > max) max = p.second.size();
		return max;
	}

	int ListaAdjacencia::ObterGrauMinimo() const
	{
		int min = adjs.size() + 1;
		for (auto p : adjs) if (p.second.size() < min) min = p.second.size();
		return min;
	}

	vector<pair<Vertice,Vertice>> const ListaAdjacencia::ObterListaAdjacencias() const
	{
		vector<pair<Vertice,Vertice>> res;
		for (pair<Vertice,set<Vertice>> p : adjs)
		{
			Vertice v = p.first;
			for (Vertice u : p.second)
			{
				if (v.Id() < u.Id())
				{
					res.push_back(make_pair(v,u));
				}
			}
		}
		return res;
	}
}

// IC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const int INFINITE_VAL = 999999;

void OutOfMemoryError()
{
	printf("Out of memory\n");
	getchar();
	exit(1);
}

template<class T> class Matrix
{
public:
	int N, M;
	T **matrix;

	Matrix() { }

	Matrix(int N, int M)
	{
		Create(N, M);
	}

	void Create(int N, int M)
	{
		this->N = N;
		this->M = M;

		matrix = (T**)malloc(N * sizeof(T*));

		if (!matrix)
			OutOfMemoryError();

		for (int i = 0; i < N; i++)
		{
			matrix[i] = (T*)malloc(M * sizeof(T));

			if (!matrix[i])
				OutOfMemoryError();

			//for (int j = 0; j < M; j++)
			//	matrix[i][j] = 0;
		}
	}

	void Fill(T val)
	{
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				matrix[i][j] = val;
	}

	~Matrix()
	{
		for (int i = 0; i < N; i++)
			free(matrix[i]);
		free(matrix);
	}

	T*& operator[](const int &index) const
	{
		return matrix[index];
	}
};

class Link
{
public:
	Link(std::string to, int cost) : to(to), cost(cost) { }
	std::string to;
	int cost;
};

class Node
{
public:
	Node() { }
	Node(int id, std::vector<Link> links) : id(id), links(links) { }
	int id;
	std::vector<Link> links = {};
};

class PathInfo
{
public:
	PathInfo() { }
	PathInfo(int from, int to, std::vector<int> hops) : from(from), to(to), hops(hops) { }
	int from;
	int to;
	std::vector<int> hops = {};
};

/*	
	auto PrintMatrix = [](int **m, int N)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (m[i][j] != INFINITE_VAL)
					printf("%d ", m[i][j]);
				else
					printf("X ");
			}
			printf("\n");
		}
		printf("\n");
	};
	auto PrintVector = [](int *m, int N)
	{
		for (int i = 0; i < N; i++)
		{
			if (m[i] != INFINITE_VAL)
				printf("%d ", m[i]);
			else
				printf("X ");
		}
		printf("\n");
	};
*/

class Network
{
public:
	int nodeCount = 0;
	std::map<std::string, Node> nodes = {};

	// adiciona um no na rede
	void AddNode(std::string name, std::vector<Link> links)
	{
		nodes[name] = Node(nodeCount++, links);
	}

	// retorna o nome de um no a partir de seu id
	std::string FindNameById(int id)
	{
		for (const auto& node : nodes)
			if (node.second.id == id)
				return node.first;
		return "NOT FOUND";
	}
	
	// exibe a matriz com os nomes dos nos colocando X em valores infinitos
	void PrintNodeMatrixNotInf(const Matrix<int>& matrix)
	{
		printf("   ");
		for (int i = 0; i < matrix.N; i++)
		{
			printf("%3s ", FindNameById(i).c_str());
		}
		printf("\n");
		for (int i = 0; i < matrix.N; i++)
		{
			printf("%3s ", FindNameById(i).c_str());
			for (int j = 0; j < matrix.N; j++)
			{
				if (matrix[i][j] < INFINITE_VAL)
					printf("%-3d ", matrix[i][j]);
				else
					printf("XXX ");
			}
			printf("\n");
		}
	}

	// exibe a matriz com os nomes dos nos
	void PrintNodeMatrix(const Matrix<int>& matrix)
	{
		printf("   ");
		for (int i = 0; i < matrix.N; i++)
		{
			printf("%3s ", FindNameById(i).c_str());
		}
		printf("\n");
		for (int i = 0; i < matrix.N; i++)
		{
			printf("%3s ", FindNameById(i).c_str());
			for (int j = 0; j < matrix.N; j++)
			{
				printf("%-3d ", matrix[i][j]);
			}
			printf("\n");
		}
	}

	// exibe o valores dos pares da matriz
	void PrintPairMatrix(const Matrix<int>& matrix)
	{
		for (int i = 0; i < matrix.N; i++)
		{
			for (int j = 0; j < matrix.N; j++)
			{
				if (i == j || matrix[i][j] == 0)
					continue;
				printf("(%s,%s) ", FindNameById(i).c_str(), FindNameById(j).c_str());
				printf("%d", matrix[i][j]);
				printf("\n");
			}
		}
	}
};

class StaticSimulation
{
public:
	Network *pNetwork;

	Matrix<int> connectionsMatrix; // matriz que diz se existe conexao entre (i,j)
	Matrix<int> costMatrix; // matriz que diz qual o custo para ir diretamente (de i, ate j), infinito caso nao seja possivel
	Matrix<int> hopCostMatrix; // matriz que diz se e possivel ir diretamente (de i, ate j), infinito caso nao seja possivel
	Matrix<int> minimumDistanceMatrix; // matriz que possui a distancia minima para ir, passando por um ou mais nos, (de i, ate j), infinito caso nao seja possivel
	Matrix<int> utilizationMatrix; // matriz que possui o numero de vezes que um caminho minimo passa por (i, j)

	std::vector<PathInfo> paths;

	float avgHops;

	StaticSimulation(Network *pNetwork) 
		: pNetwork(pNetwork)
	{
		int N = pNetwork->nodeCount;
		
		connectionsMatrix.Create(N, N);
		costMatrix.Create(N, N);
		hopCostMatrix.Create(N, N);
		minimumDistanceMatrix.Create(N, N);
		utilizationMatrix.Create(N, N);

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				connectionsMatrix[i][j] = 0;
				costMatrix[i][j] = INFINITE_VAL;
				hopCostMatrix[i][j] = INFINITE_VAL;
				minimumDistanceMatrix[i][j] = 0;
				utilizationMatrix[i][j] = 0;
			}
		}
		
		for (const auto& node : pNetwork->nodes)
		{
			auto i = node.second.id;
			costMatrix[i][i] = 0;
			hopCostMatrix[i][i] = 0;
			for (const auto& link : node.second.links)
			{
				auto j = pNetwork->nodes[link.to].id;
				connectionsMatrix[i][j] = 1;
				costMatrix[i][j] = link.cost;
				hopCostMatrix[i][j] = 1;
			}
		}
	}

	void PreparePathsAndMinimumDistanceMatrix(bool useUniformCost)
	{
		paths.clear();

		for (const auto& node : pNetwork->nodes)
		{
			auto i = node.second.id;
			FindShortestPaths(i, useUniformCost ? hopCostMatrix : costMatrix, minimumDistanceMatrix[i], pNetwork->nodeCount);
		}
	}

	void Run()
	{
		PreparePathsAndMinimumDistanceMatrix(true);

		// numero medio de hops
		int sum = 0;
		int k = 0;
		int N = pNetwork->nodeCount;

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (minimumDistanceMatrix[i][j] != INFINITE_VAL && minimumDistanceMatrix[i][j] > 0)
				{
					sum += minimumDistanceMatrix[i][j];
					k++;
				}
			}
		}

		avgHops = (float)sum / (float)k;

		// matriz de utilizacao de enlaces por caminhos minimos
		for (auto& p : paths)
		{
			int from = p.from;
			for (unsigned i = 0; i < p.hops.size(); i++)
			{
				int to = p.hops[i];
				utilizationMatrix[from][to]++;
				from = to;
			}
		}
	}

	void FindShortestPaths(int from, const Matrix<int>& cost, int *distance, int N)
	{
		std::vector<bool> visited(N);
		std::vector<int> previous(N);

		for (int i = 0; i < N; i++)
		{
			if (cost[from][i] != INFINITE_VAL)
			{
				distance[i] = cost[from][i];
				previous[i] = from;
			}
			else
			{
				distance[i] = INFINITE_VAL;
				previous[i] = INFINITE_VAL;
			}

			visited[i] = false;
		}

		distance[from] = 0;
		visited[from] = true;
		int nextNode = -1;

		for (int count = 1; count < N - 1; count++)
		{
			int minDistance = INFINITE_VAL;

			for (int i = 0; i < N; i++)
			{
				if (visited[i])
					continue;

				if (distance[i] < minDistance)
				{
					minDistance = distance[i];
					nextNode = i;
				}
			}

			visited[nextNode] = true;
			for (int i = 0; i < N; i++)
			{
				if (visited[i])
					continue;

				if (minDistance + cost[nextNode][i] < distance[i])
				{
					distance[i] = minDistance + cost[nextNode][i];
					previous[i] = nextNode;
				}
			}
		}

		for (int i = 0; i < N; i++)
		{
			if (i == from)
				continue;

			std::vector<int> tmp = {};

			int j = i;
			while (j != from && previous[j] != INFINITE_VAL)
			{
				tmp.push_back(j);
				j = previous[j];
			};

			std::reverse(tmp.begin(), tmp.end());
			paths.push_back(PathInfo(from, i, tmp));
		}
	}
};

class NetworkConfigParser
{
public:
	static void Parse(const char *path, Network *pNetwork) 
	{
		FILE *file = NULL;
		fopen_s(&file, path, "r");

		if (!file)
		{
			printf("Cannot read file.\n");
			return;
		}

		char buf[2048] = {};
		char buf2[2048] = {};

		while (!feof(file))
		{
			if (!fgets(buf, sizeof(buf), file))
				break;

			if (buf[0] == '#' || buf[0] == '\r' || buf[0] == '\n' || buf[0] == ' ')
				continue;
			
			char nodeName[256] = {};
			int linkCount = 0;
			sscanf(buf, "%s %d", nodeName, &linkCount);

			std::vector<Link> links = {};

			for (int i = 0; i < linkCount; i++)
			{
				if (!fgets(buf2, sizeof(buf2), file))
					break;

				char nodeName2[256] = {};
				int cost = 0;
				sscanf(buf2, "%s %d", nodeName2, &cost);

				links.push_back(Link(nodeName2, cost));
			}

			pNetwork->AddNode(nodeName, links);
		}

		fclose(file);
	}
};

int main()
{
	Network network;
	/*
	1 - 4
	|   |
	2 - 3	
	*/
	/*network.AddNode("n1", { Link("n2", 1), Link("n4", 1) });
	network.AddNode("n2", { Link("n1", 1), Link("n3", 1) });
	network.AddNode("n3", { Link("n2", 1), Link("n4", 1) });
	network.AddNode("n4", { Link("n1", 1), Link("n3", 1) });*/

	NetworkConfigParser::Parse("config.txt", &network);

	StaticSimulation staticSim = StaticSimulation(&network);
	staticSim.Run();

	printf("* Matriz de conexoes:\n");
	network.PrintNodeMatrix(staticSim.connectionsMatrix);
	printf("\n");

	printf("* Matriz de distancia minima:\n");
	network.PrintNodeMatrixNotInf(staticSim.minimumDistanceMatrix);
	printf("\n");

	printf("* Rotas:\n");
	int last = -1;
	for (auto& p : staticSim.paths)
	{
		auto fromName = network.FindNameById(p.from);
		auto toName = network.FindNameById(p.to);

		if (p.from != last)
		{
			printf("Partindo de %s\n", fromName.c_str());
		}
		if (p.hops.size() > 0)
		{
			printf(" ate %s: ", toName.c_str());

			printf("%s->", fromName.c_str());
			for (unsigned i = 0; i < p.hops.size(); i++)
			{
				auto& h = p.hops[i];
				printf("%s", network.FindNameById(h).c_str());
				if (i != p.hops.size() - 1)
					printf("->");
			}
		}
		else
		{
			printf(" nao pode chegar ate %s.", toName.c_str());
		}

		last = p.from;
		printf("\n");
	}
	printf("\n");

	printf("* Numero medio de hops: ");
	printf("%f\n", staticSim.avgHops);
	printf("\n");

	printf("* Matriz de utilizacao de enlace:\n");
	network.PrintNodeMatrix(staticSim.utilizationMatrix);
	printf("\n");
	network.PrintPairMatrix(staticSim.utilizationMatrix);
	printf("\n");
	
	getchar();

	return 0;
}


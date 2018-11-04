#pragma once

//TODO:finish me
enum LambdaAllocationStrategy
{
	BASIC,
	RANDOM,
	FIRST_FIT,
	LEAST_USED,
	STRATEGIES_COUNT,
};

//TODO:finish me
class StaticSimulation
{
public:
	Network *pNetwork;

	Matrix<int> connectionsMatrix; // matriz que diz se existe conex�o entre (i,j)
	Matrix<int> costMatrix; // matriz que diz qual o custo para ir diretamente (de i, at� j), infinito caso n�o seja poss�vel
	Matrix<int> hopCostMatrix; // matriz que diz se e poss�vel ir diretamente (de i, at� j), infinito caso n�o seja poss�vel
	Matrix<int> minimumDistanceMatrix; // matriz que possui a distancia m�nima para ir, passando por um ou mais nos, (de i, at� j), infinito caso n�o seja poss�vel
	Matrix<int> utilizationMatrix; // matriz que possui o numero de vezes que um caminho minimo passa por (i, j)
	Matrix<std::vector<LambdaAllocInfo>> lambdaMatrix; // matriz que possui as aloca��es de comprimento de onda (de i, at� j) das rotas de caminho m�nimo, utilizando o menor lambda poss�vel por caminho
	Matrix<std::vector<LambdaAllocInfo>> lambdaMatrixWithConversor; // matriz que possui as aloca��es de comprimento de onda (de i, at� j) das rotas de caminho m�nimo, utilizando conversores em todos os n�s
	
	Matrix<std::vector<PathInfo>> allPaths; // matriz com todos os caminhos poss�veis (de i, at� j)
	std::vector<PathInfo> minimumCostPaths; // lista de caminhos m�nimos

	float avgHops; // n�mero m�dio de hops, utilizando o caminho m�nimo

	StaticSimulation(Network *pNetwork);
	
	//TODO:finish me
	void Run();

	/// encontra todos os caminhos minimos partindo de todos os nos ate todos os nos
	/// useUniformCost: indica se deve usar a matriz com custo 1 entre todos os n�s
	void PreparePathsAndMinimumDistanceMatrix(bool useUniformCost);

	/// encontra o menor n�mero poss�vel N no set (sendo N >= 1 e N < infinito)
	/// used: set com n�meros N (sendo N >= 1 e N < infinito)
	int FindSmallestLambdaAvailable(const std::set<int>& used);

	//TODO:finish me
	void LambdaAllocation(Matrix<std::vector<LambdaAllocInfo>>& lambdaMatrix, const std::vector<PathInfo>& paths, std::set<int>& usedLambdas, bool useConversor, LambdaAllocationStrategy strategy);

	//TODO:finish me
	void DiscoverUsedLambdasInThePath(Matrix<std::vector<LambdaAllocInfo>>& lambdaMatrix, const PathInfo& path, std::set<int>& usedLambdasInThePath);

	//TODO:finish me
	void AllocateLambda(Matrix<std::vector<LambdaAllocInfo>>& lambdaMatrix, const PathInfo& path, std::set<int>& usedLambdas);
	
	//TODO:finish me
	void AllocateLambdaWithConversor(Matrix<std::vector<LambdaAllocInfo>>& lambdaMatrix, const PathInfo& path, std::set<int>& usedLambdas);
	
	/// m�todo auxiliar do m�todo FindAllPaths parar encontrar todos os caminhos poss�veis em um grafo dada uma matriz adjac�ncia
	/// from: n� de origem
	/// to: n� de destino
	/// current: n� atual
	/// cost: matriz com custo para ir (de i, at� j)
	/// N: n�mero de n�s na matriz (n�mero de linhas e colunas)
	/// visited: lista de n�s j� visitados
	/// path: lista de n�s visitados para o caminho atual
	/// level: n�vel atual de busca 
	/// allPaths: matriz com todos os caminhos (de i, at� j)
	void FindAllPathsDfs(int from, int to, int current, const Matrix<int>& cost, int N, std::vector<bool>& visited, std::vector<int>& path, int& level, Matrix<std::vector<PathInfo>>& allPaths);

	/// encontra todos os caminhos poss�veis em um grafo dada uma matriz de adjac�ncia
	/// from: n� de origem
	/// to: n� de destino
	/// cost: matriz com custo para ir (de i, at� j)
	/// allPaths: matriz com todos os caminhos (de i, at� j)
	/// N: n�mero de n�s na matriz (n�mero de linhas e colunas)
	void FindAllPaths(int from, int to, const Matrix<int>& cost, Matrix<std::vector<PathInfo>>& allPaths, int N);

	/// encontra todos os caminhos m�nimos em um grafo dada uma matriz de adjac�ncia
	/// from: n� de origem
	/// cost: matriz com custo para ir (de i, at� j)
	/// paths: lista de caminhos
	/// N: n�mero de n�s na matriz (n�mero de linhas e colunas)
	void FindShortestPaths(int from, const Matrix<int>& cost, std::vector<PathInfo>& paths, int *distance, int N);
};
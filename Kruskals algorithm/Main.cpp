#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Edge;
class node_set;

int menu();
void getEdges(std::vector<Edge>& edges, node_set& v);
void sortEdges(std::vector<Edge>& edges);
void sortMST(std::vector<Edge>& MST);
void kruskalsAlgorithm(std::vector<Edge>& edges, node_set& v);
void swapEdges(Edge& a, Edge& b);
void displayEdges(std::vector<Edge>& edges);
int getNodeNumber(char node, node_set& nodes);
int getRoot(int v, std::vector<int>& parent);
void createAdjacencyMatrix(std::vector<Edge>& edges);

// вершина
struct Node
{
	char name; // имя вершины
	int n; // номер вершины
};

// ребро
struct Edge
{
	Node start; // первая вершина
	Node end; // вторая вершина
	int weight; // вес ребра
};

class node_set
{
public:
	node_set()
	{
		setsize = 0;
		nodes = nullptr;
	}
	~node_set()
	{
		delete[] nodes;
	}

	void add(char v)
	{
		if (setsize < 1)
		{
			resize(v);
		}
		else
		{
			if (!find(v))
			{
				resize(v);
			}
		}
	}

	int size() { return setsize; }
	void display()
	{
		for (int i = 0; i < setsize; i++)
		{
			cout << nodes[i] << '\n';
		}
		cout << "Количество вершин: " << setsize << '\n';
	}

	char operator[](int index)
	{
		return nodes[index];
	}
	int indexOf(char node)
	{
		for (int i = 0; i < setsize; i++)
		{
			if (nodes[i] == node)
				return i;
		}
		return -1;
	}
private:
	char* nodes;
	int setsize;

	void resize(char v)
	{
		char* newSet = new char[setsize + 1];
		for (int i = 0; i < setsize; i++)
		{
			newSet[i] = nodes[i];
		}
		delete[] nodes;
		newSet[setsize] = v;
		nodes = newSet;
		setsize++;
		newSet = nullptr;
	}

	bool find(char v)
	{
		for (int i = 0; i < setsize; i++)
		{
			if (nodes[i] == v)
				return true;
		}
		return false;
	}
};


int menu()
{
	bool correctInput = false;
	cout << "-----Алгоритм Крускала-----\n\n";
	int choice = 0;
	while (!correctInput) // если число введено неправильно, повторить запрос ввода
	{
		cout << "1. Использовать ручной ввод\n";
		cout << "2. Использовать ввод из файла\n";
		cout << "Введите число: ";

		cin >> choice;
		if (choice == 1 || choice == 2) correctInput = true;
		else cout << "Вы ввели неверное число!\n";
	}
	return choice;
}

void getEdges(std::vector<Edge>& edges, node_set& nodes)
{
	cout << "Введите ребра графа(Начало-Конец-Вес, через пробел)\n";
	cout << "Пример: A B 3\n";
	cout << "Введите 0 для конца ввода\n";
	while (true)
	{
		Node start;
		Node end;
		int weight;

		cin >> start.name;
		if (start.name == '0') break;
		cin >> end.name >> weight;

		nodes.add(start.name);
		nodes.add(end.name);

		start.n = getNodeNumber(start.name, nodes);
		end.n = getNodeNumber(end.name, nodes);

		edges.push_back(Edge{ start, end, weight });
	}
}

void sortEdges(std::vector<Edge>& edges)
{
	bool sorted = true;

	do {
		sorted = true;

		for (int i = 0; i < edges.size() - 1; i++) {
			if (edges[i].weight > edges[i + 1].weight) {
				swapEdges(edges[i], edges[i + 1]);
				sorted = false;
			}
		}
	} while (!sorted);
}

void sortMST(std::vector<Edge>& MST)
{
	bool sorted = true;

	do {
		sorted = true;

		for (int i = 0; i < MST.size() - 1; i++) {
			if (MST[i].end.name > MST[i + 1].end.name) {
				swapEdges(MST[i], MST[i + 1]);
				sorted = false;
			}
		}
	} while (!sorted);
}

void kruskalsAlgorithm(std::vector<Edge>& edges, node_set& v)
{
	std::vector<Edge> MST(v.size() - 1); // искомый остов
	int size = edges.size(); // кол-во ребер
	// сортировка массива ребер в возрастающем порядке по весу ребра
	sortEdges(edges);
	std::vector<int> parent(v.size());

	for (int i = 0; i < parent.size(); i++)
		parent[i] = i;

	int count = 0;
	int i = 0;
	while (count < v.size() - 1) { // Runs till V-1 times

		int sourceParent = getRoot(edges[i].start.n, parent);
		int destParent = getRoot(edges[i].end.n, parent);

		if (sourceParent != destParent) { // checks if no cycle
			MST[count] = edges[i];
			count++;
			parent[sourceParent] = destParent;
		}

		i++;
	}

	sortMST(MST);

	cout << "Минимальный остов:\n";
	int wSum = 0;
	for (int i = 0; i < MST.size(); i++)
	{
		wSum += MST[i].weight;
		cout << MST[i].start.name << " " << MST[i].end.name << "   " << MST[i].weight << '\n';
	}
	cout << "Минимальный вес: " << wSum << '\n';
}

void swapEdges(Edge& a, Edge& b)
{
	Edge temp = a;
	a = b;
	b = temp;
}

void displayEdges(std::vector<Edge>& edges)
{
	for (int i = 0; i < edges.size(); i++)
	{
		std::cout << edges[i].start.name << " " << edges[i].end.name << " " << edges[i].weight << '\n';
	}
}

int getNodeNumber(char node, node_set& nodes)
{
	return nodes.indexOf(node);
}

int getRoot(int v, std::vector<int>& parent)
{
	if (parent[v] == v)
		return v;

	return getRoot(parent[v], parent);
}

void loadEdgesFromFile(const std::string& filename, std::vector<Edge>& edges, node_set& nodes)
{
	ifstream file(filename);

	if (file.is_open())
	{
		Node start;
		Node end;
		int weight;

		while (file >> start.name >> end.name >> weight)
		{
			nodes.add(start.name);
			nodes.add(end.name);

			start.n = getNodeNumber(start.name, nodes);
			end.n = getNodeNumber(end.name, nodes);

			edges.push_back(Edge{ start, end, weight });
		}

		cout << "Ребра графа:\n\n";
		displayEdges(edges);
		cout << endl;
	}
	else
	{
		cerr << "Невозможно открыть файл " << filename << '\n';
	}

	file.close();
}

void createAdjacencyMatrix(std::vector<Edge>& edges)
{

}

int main()
{
	setlocale(LC_ALL, "Russian"); // использовать русский язык в консоли

	std::vector<Edge> edges;
	node_set nodes;
	int nVerticies; // количество вершин графа

	int choice = menu();

	if (choice == 1)
	{
		getEdges(edges, nodes);
	}
	else if (choice == 2)
	{
		bool correctChoice = false;
		int choice;
		while (!correctChoice) // если число введено неправильно, повторить запрос ввода
		{
			cout << "1. Тест 1\n"
				<< "2. Тест 2\n"
				<< "3. Тест 3\n";
			cout << "Введите число: ";

			cin >> choice;
			if (choice == 1 || choice == 2 || choice == 3) correctChoice = true;
			else cout << "Вы ввели неверное число!\n";
		}

		loadEdgesFromFile("test" + std::to_string(choice) + ".txt", edges, nodes);
	}



	kruskalsAlgorithm(edges, nodes);

	return 0;
}
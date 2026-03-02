#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>


class AdjacencyMatrix {
    bool** matrix;
    int vertexCount;

public:
    explicit AdjacencyMatrix(const int vertexCount) {
        this->vertexCount = vertexCount;
        matrix = new bool*[vertexCount];
        for (int i = 0; i < vertexCount; i++) {
            matrix[i] = new bool[vertexCount]{false};
        }
    }

    ~AdjacencyMatrix() {
        for (int i = 0; i < vertexCount; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    void addEdge(const int vertex1, const int vertex2) {
        matrix[vertex1][vertex2] = true;
        matrix[vertex2][vertex1] = true;
    }

    [[nodiscard]] std::vector<int> getNeighbors(const int vertex) const {
        std::vector<int> neighbors;
        for (int i = 0; i < vertexCount; i++) {
            if (matrix[vertex][i]) neighbors.push_back(i);
        }
        return neighbors;
    }

    [[nodiscard]] int getVertexCount() const {
        return vertexCount;
    }
};

AdjacencyMatrix readAdjacencyMatrix(std::istream& input) {
    int vertexCount, vertex;
    input >> vertexCount;
    AdjacencyMatrix adjacencyMatrix(vertexCount);
    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            input >> vertex;
            if (vertex) adjacencyMatrix.addEdge(i, j);
        }
    }

    return adjacencyMatrix;
}

class ComponentsDecomposer {

    void dfs(
        const AdjacencyMatrix& adjacencyMatrix, bool* visited,
        const int startVertex, std::vector<int>& component) const {

        auto stack = std::stack<int>();
        stack.push(startVertex);
        visited[startVertex] = true;

        while (!stack.empty()) {
            int vertex = stack.top();
            stack.pop();
            component.push_back(vertex);

            for (int neighbor: adjacencyMatrix.getNeighbors(vertex)) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    stack.push(neighbor);
                }
            }
        }
    }

public:
    [[nodiscard]] std::vector<std::vector<int>> decompose(const AdjacencyMatrix &matrix) const {
        int vertexCount = matrix.getVertexCount();
        auto visited = new bool[vertexCount]{false};

        std::vector<std::vector<int>> components = std::vector<std::vector<int>>();
        for (int i = 0; i < vertexCount; i++) {
            if (!visited[i]) {
                std::vector<int> component = std::vector<int>();
                this->dfs(matrix, visited, i, component);
                components.push_back(component);
            }
        }
        delete[] visited;
        return components;
    }
};

void showComponents(std::ostream& output, const std::vector<std::vector<int>>& components) {
    output << components.size();
    for (const auto& component : components) {
        output << std::endl;
        for (const auto vertex: component) output << vertex + 1 << " ";
    }
}

void sortComponentVertices(std::vector<std::vector<int>>& components) {
    for (auto& component : components) std::ranges::sort(component);
}


int main() {
    std::ifstream input("in.txt");
    std::ofstream output("out.txt");

    const AdjacencyMatrix adjacencyMatrix = readAdjacencyMatrix(input);
    constexpr auto componentsDecomposer = ComponentsDecomposer();

    std::vector<std::vector<int>> components = componentsDecomposer.decompose(adjacencyMatrix);
    sortComponentVertices(components);
    showComponents(output, components);
}

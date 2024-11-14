#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// generates a random matrix of size n x n given the size n
std::vector<int> gen_matrix(int n) {
  std::vector<int> matrix;
  for (int i = 0; i < n * n; i++) {
    matrix.push_back(rand() % 100);
  }
  return matrix;
}

// writes the matrix to a file
void write_matrices(const std::string& filename, const std::vector<int>& matrix_a,
                    const std::vector<int>& matrix_b, const int n) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: could not open file " << filename << std::endl;
    return;
  }

  // print out how many elements need to be read
  int num_elements = 2 + matrix_a.size() + matrix_b.size();
  file << num_elements << " ";

  file << n << " ";
  for (int i = 0; i < matrix_a.size(); i++) {
    file << matrix_a[i] << " ";
  }

  file << n << " ";
  for (int i = 0; i < matrix_b.size(); i++) {
    file << matrix_b[i] << " ";
  }
  // file << std::endl;

  file.close();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: ./gen_mat <size of matrix>" << std::endl;
    return 1;
  }

  int n = std::stoi(argv[1]);
  std::vector<int> matrix_a = gen_matrix(n);
  std::vector<int> matrix_b = gen_matrix(n);
  write_matrices("input.txt", matrix_a, matrix_b, n);
  return 0;
}
// Advent of Code day 08 -- binary aoc22.08

#include <iostream>
#include <iomanip>
#include <numeric>

#include <libaoc.h>

int count_visible(size_t rows, size_t cols, const std::vector<std::vector<int>>& board){
	std::vector<std::vector<std::array<int, 4>>> view(rows);
	for (auto & row: view)  row.resize(cols);

	// Store the view at each spot as a 4-digit number
	auto scope_out = [&](size_t row, size_t col, int vis, unsigned dir){
		view[row][col][dir] = vis;
		auto b =board[row][col];
		if (b > vis) vis = b;
		return vis;
	};
	
	int vis{0};
	for (size_t row = 0; row < rows; ++row){
		vis = 0;
		for (size_t col = 0; col < cols; ++col) vis = scope_out(row, col, vis, 0);
		vis = 0;
		for (size_t col = cols; col-- > 0;) vis = scope_out(row, col, vis, 2);
	}
	for (size_t col = 0; col < cols; ++col){
		vis = 0;
		for (size_t row = 0; row < rows; ++row) vis = scope_out(row, col, vis, 3);
		vis = 0;
		for (size_t row = rows; row-- > 0;) vis = scope_out(row, col, vis, 1);
	}

	std::vector<std::vector<int>> lowest_vis(rows);
	for (size_t row=0; row<rows; ++row){
		for (size_t col=0; col<cols; ++col){
			int lowest=9;
			for (const auto v: view[row][col]) if (v < lowest) lowest=v;
			lowest_vis[row].push_back(lowest);
		}
	}

	// Count the visible trees; remembering that even zero-height trees on the boarder can be seen
	auto visible = static_cast<int>(2*rows + 2*cols - 4);
	for (size_t row = 1; row < rows-1; ++row){
		for (size_t col = 1; col < cols-1; ++col){
			if (lowest_vis[row][col] < board[row][col]) visible++;
		}
	}

	return visible;
}

int best_view(size_t rows, size_t cols, const std::vector<std::vector<int>> & board){
	std::vector<std::vector<std::array<int, 4>>> views(rows);

	for (size_t r=0; r<rows; ++r){
		auto & row = views[r];
		row.resize(cols);
	  for (size_t c=0; c<cols; ++c){
			auto & tree = row[c];
			tree = {{0, 0, 0, 0}};
			const int b{board[r][c]};
			for (size_t n=r; n-- >0;){
				++tree[0];
				if (board[n][c] >= b) break;
			}
			for (size_t e=c+1; e<cols; ++e){
				++tree[1];
				if (board[r][e] >= b) break;
			}
			for (size_t s=r+1; s<rows; ++s){
				++tree[2];
				if (board[s][c] >= b) break;
			}
			for (size_t w=c; w-- > 0; ){
				++tree[3];
				if (board[r][w] >= b) break;
			}
		}
	}

	std::vector<std::vector<int>> score(rows);
	for (size_t r=0; r<rows; ++r){
		for (size_t c=0; c<cols; ++c){
			score[r].push_back(std::accumulate(views[r][c].begin(), views[r][c].end(), 1, std::multiplies<int>()));
		}
	}
	
	auto grth = [](const int a, const int b){return b<a;};
	std::vector<int> per_row(rows);
	for (size_t r=0; r<rows; ++r){
		std::partial_sort(score[r].begin(), score[r].begin()+1, score[r].end(), grth);
		per_row[r] = score[r][0];
	}
	std::partial_sort(per_row.begin(), per_row.begin()+1, per_row.end(), grth);
	return per_row[0];
}

int main(int argc, const char *argv[]){
  if (argc > 4 || argc < 3) {
      std::cout << "usage: " << argv[0] << " [filename|input] [part] {[expected result]}" << std::endl;
    return 1;
  }

  std::string filename = argv[1];
  int part = std::stoi(argv[2]);
  bool do_test = argc > 3;
  int result = (argc > 3) ? std::stoi(argv[3]) : -1;

	auto board_line_parser = [](const std::string & line){
	  std::vector<int> out;
		out.reserve(line.size());
		for (const auto & c: line) out.push_back(c - '0');
		return out;
	};
	auto board = aoc::read_vector_from_lines<std::vector<int>>(filename, board_line_parser);
	// Do a sanity check on the board size:
	size_t rows{board.size()}, cols{board[0].size()};
	for (const auto & row: board) if (row.size() != cols) throw std::runtime_error("Board is not full?");
	if (rows != cols) throw std::runtime_error("Board is not square?!");

	int answer = (part == 1) ? count_visible(rows, cols, board) : best_view(rows, cols, board);

  std::cout << (do_test ? "Test " : "Part ") << part << " results in " << answer << "\n";

  /* Compare the expected result and indicate failure for CTest */
  return (do_test && result != static_cast<int>(answer)) ? 1 : 0;
}

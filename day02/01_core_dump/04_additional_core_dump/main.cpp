#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <set>

class Position {
    int row_;
    int col_;
public:
    Position(int r, int c) : row_(r), col_(c) {}
    int row() const { return row_; }
    int col() const { return col_; }
    auto operator<=>(const Position& other) const = default;
    auto neighbors() const {
        return std::array{
            Position{row_, col_ + 1},
            Position{row_ + 1, col_},
            Position{row_, col_ - 1},
            Position{row_ - 1, col_}
        };
    }
    friend std::ostream& operator<<(std::ostream& out, const Position& pos) {
        return out << '{' << pos.col_ << ", " << pos.row_ << '}';
    }
};

class Maze {
private:
    std::vector<std::string> grid_;
    Position topLeft() const {
        return {0, 0};
    }
    Position bottomRight() const {
        return {rows() - 1, cols() - 1};        
    }
public:
    explicit Maze(std::vector<std::string> grid) : grid_(std::move(grid)) {
        if (grid_.empty()) {
            throw std::runtime_error("Maze cannot be empty");
        }
    }

    int rows() const { return static_cast<int>(grid_.size()); }
    int cols() const { return static_cast<int>(grid_[0].size()); }

    bool inBounds(const Position& p) const {
        return p >= topLeft() && p <= bottomRight();
    }

    char at(const Position& p) const {
        return grid_[p.row()][p.col()];
    }

    char& at(const Position& p) {
        return grid_[p.row()][p.col()];
    }

    bool isWall(const Position& p) const {
        return at(p) == '#';
    }

    bool isBounty(const Position& p) const {
        return at(p) == '$';
    }    

    Position find(char target) const {
        for (int r = 0; r < rows(); ++r) {
            for (int c = 0; c < cols(); ++c) {
                if (grid_[r][c] == target) {
                    return Position(r, c);
                }
            }
        }
        throw std::runtime_error("Target not found");
    }

    void markPath(const std::vector<Position>& path) {
        for (const auto& p : path) {
            if (at(p) == '.') {
                at(p) = '*';
            }
        }
    }

    void print() const {
        size_t counter = 0;
        std::cout << "  ";
        while (counter < grid_[0].size()) {
            std::cout << (counter % 10);
            ++counter;
        }
        std::cout << std::endl;
        counter = 0;
        for (const auto& row : grid_) {
            std::cout << (counter % 10) << ' ' << row << '\n';
            ++counter;
        }
    }
};

class MazeSolver {
    std::set<Position> visited_;
    std::vector<Position> path_;
    std::vector<Position> bounties_;
    const Maze* pMaze;
    bool bounty_greedy_;
    Position* back_from_bounty_ = nullptr;
    bool solvePath(Position from, Position to) {
        visited_.insert(from);
        path_.push_back(from);
        if(from == to) return true;
        for (const auto& next : from.neighbors()) {
            if (!pMaze->isWall(next) && !visited_.contains(next)) {
                back_from_bounty_ = nullptr;
                visited_.insert(next);
                if (pMaze->isBounty(next)) bounties_.push_back(next);
                if(solvePath(next, to)) return true;
            }
        }
        if(!bounties_.empty() && path_.back() == bounties_.back()) {
            if(bounty_greedy_) {
                // size must be >= 2, as we have the start + bounty
                back_from_bounty_ = path_.data() + path_.size() - 2;
                path_.push_back(*back_from_bounty_);
            }
            else {
                path_.pop_back();
                bounties_.pop_back();
            }
        }
        else if(back_from_bounty_ == nullptr) path_.pop_back();
        else {
            back_from_bounty_ -= 1;
            path_.push_back(*back_from_bounty_);
        }
        return false;
    }
    struct SolverResult {
        bool solved;
        std::vector<Position> path;
        std::vector<Position> bounties;
    };
public:
    auto solve(const Maze& maze, bool bounty_greedy = true) {
        pMaze = &maze;
        bounty_greedy_ = bounty_greedy;
        visited_.clear();
        path_.clear();
        bounties_.clear();
        Position start = maze.find('S');
        Position goal = maze.find('E');
        return SolverResult{
            solvePath(start, goal),
            path_,
            bounties_
        };
    }
};

void solve_and_print(const Maze& maze, bool bounty_greedy = true) {
    MazeSolver solver;
    std::cout << "Maze:\n";
    maze.print();
    auto [solved, path, bounties] = solver.solve(maze, bounty_greedy);
    if(solved) {
        std::cout << "There is a path" << std::endl;
        for(const auto& pos: path) {
            std::cout << pos << " => ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "There isn't a path" << std::endl;
    }
    std::cout << "Bounties (" << (bounty_greedy? "" : "non-") << "greedy solution):" << std::endl;
    for(const auto& pos: bounties) {
        std::cout << pos << ' ';
    }
    std::cout << std::endl;
    if(solved) {
        std::cout << "Solution:\n";
        Maze solution = maze;
        solution.markPath(path);
        solution.print();
    }
}

int main() {
    std::vector<std::string> grid1 = {
        "###########",
        "#S.$#.$..E#",
        "##.##.#####",
        "#$......$.#",
        "###########"
    };

    std::vector<std::string> grid2 = {
        "###########",
        "#$..#.$..E#",
        "#.#.#####.#",
        "#S#.....#.#",
        "#.###..$#.#",
        "#$#$..#.$.#",
        "###########"
    };

    std::vector<std::string> grid3 = {
        "###########",
        "#$..#.$..E#",
        "#.#######.#",
        "#S$.$#..#.#",
        "#$###..$#.#",
        "#$.$..#.$.#",
        "###########"
    };

    solve_and_print(Maze(grid1));
    solve_and_print(Maze(grid1), false /*not bounty-greedy*/);
    solve_and_print(Maze(grid2));
    solve_and_print(Maze(grid2), false /*not bounty-greedy*/);
    solve_and_print(Maze(grid3));
    solve_and_print(Maze(grid3), false /*not bounty-greedy*/);
}
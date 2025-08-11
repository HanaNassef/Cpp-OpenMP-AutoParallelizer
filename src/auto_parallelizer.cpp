#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

// List of assignment operators
const vector<string> assignment_operators = {
    "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", ">>=", "<<="
};

// Trim whitespace from both ends of a string
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Extract variable name from a line containing assignment
string extract_variable(const string& line) {
    for (const string& op : assignment_operators) {
        size_t pos = line.find(op);
        if (pos != string::npos) {
            // Extract substring before operator
            string left = line.substr(0, pos);
            left = trim(left);

            // Get last token (likely variable name)
            size_t last_space = left.find_last_of(" \t");
            if (last_space == string::npos) {
                return left;
            } else {
                return left.substr(last_space + 1);
            }
        }
    }
    return "";
}

int main() {
    string input_file, output_file;
    cout << "Enter the input file name: ";
    getline(cin, input_file);
    cout << "Enter the output file name: ";
    getline(cin, output_file);

    ifstream fin(input_file);
    if (!fin) {
        cerr << "Error opening input file.\n";
        return 1;
    }

    vector<string> lines;
    string line;
    while (getline(fin, line)) {
        lines.push_back(line);
    }
    fin.close();

    vector<string> parallelized_lines;
    parallelized_lines.push_back("#include <omp.h>\n");

    set<string> declared_variables;
    bool inside_loop = false;

    for (size_t i = 0; i < lines.size(); ++i) {
        string stripped = trim(lines[i]);

        // Step 1: Collect declared/assigned variables before any loop
        if (!inside_loop && stripped.find("for (") != 0) {
            string var_name = extract_variable(stripped);
            if (!var_name.empty()) {
                declared_variables.insert(var_name);
            }
        }

        // Step 2: Detect start of a for loop
        if (stripped.find("for (") == 0) {
            parallelized_lines.push_back("#pragma omp parallel for\n");
            inside_loop = true;
        }

        // Step 3: If inside loop, check for shared variable assignments
        if (inside_loop) {
            bool atomic_needed = false;

            for (const string& var : declared_variables) {
                size_t assign_pos = string::npos;
                for (const string& op : assignment_operators) {
                    size_t pos = stripped.find(op);
                    if (pos != string::npos) {
                        assign_pos = pos;
                        break;
                    }
                }

                if (assign_pos != string::npos &&
                    stripped.find(var) != string::npos &&
                    stripped.find(var) < assign_pos) {
                    // It's a shared variable being assigned — insert atomic
                    parallelized_lines.push_back("#pragma omp atomic\n");
                    break;
                }
            }
        }

        // Step 4: Append original line
        parallelized_lines.push_back(lines[i] + "\n");

        // Step 5: Detect end of loop (naive but works for one-level)
        if (stripped.find('}') != string::npos && inside_loop) {
            inside_loop = false;
        }
    }

    // Step 6: Write transformed output
    ofstream fout(output_file);
    if (!fout) {
        cerr << "Error opening output file.\n";
        return 1;
    }

    for (const auto& l : parallelized_lines) {
        fout << l;
    }
    fout.close();

    cout << "Done! ";
    return 0;
}

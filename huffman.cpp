#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <string>
#include <cassert>
#include <stdexcept>
#include <iostream>

using namespace std;

// Nodo del arbol
struct HuffmanTree {
    char c; 
    int cfreq;
    struct HuffmanTree *left;
    struct HuffmanTree *right;
    HuffmanTree(char c, int cfreq, struct HuffmanTree *left=NULL,
                struct HuffmanTree *right=NULL) :
        c(c), cfreq(cfreq), left(left), right(right) {
    }
    ~HuffmanTree() {
        delete left, delete right;
    }
    class Compare {
    public:
        bool operator()(HuffmanTree *a, HuffmanTree *b) {
            return a->cfreq > b->cfreq;
        }
    };
};

// Construye el arbol
HuffmanTree *build_tree(vector< pair<char, unsigned> > &alph) {
    // First build a min-heap
    // Build leaf nodes first
    priority_queue<HuffmanTree *, vector<HuffmanTree *>, HuffmanTree::Compare > alph_heap;
    for (vector< pair<char, unsigned> >::iterator it = alph.begin();
         it != alph.end(); ++it) {
        HuffmanTree *leaf = new HuffmanTree(it->first, it->second);
        alph_heap.push(leaf);
    }

    HuffmanTree *root = NULL;
    while (alph_heap.size() > 1) {
        HuffmanTree *l, *r;
        l = alph_heap.top();
        alph_heap.pop();
        r = alph_heap.top();
        alph_heap.pop();
        root = new HuffmanTree(0, l->cfreq + r->cfreq, l, r);
        alph_heap.push(root);
    }

    return root;
}


void print_tree(HuffmanTree *t) {
    deque< pair<HuffmanTree *, int> > q;

    q.push_back(make_pair(t, 0));
    int curlevel = -1;
    while (!q.empty()) {
        HuffmanTree *parent = q.front().first;
        int level = q.front().second;
        q.pop_front();
        if (curlevel != level) {
            curlevel = level;
            cout << "Level " << curlevel << endl;
        }
        cout << parent->cfreq << " " << parent->c << endl;
        if (parent->left)
            q.push_back(make_pair(parent->left, level + 1));
        if (parent->right)
            q.push_back(make_pair(parent->right, level + 1));
    }
}

typedef vector<bool> code_t;
typedef map<char, code_t> codetable;
map<char, code_t> build_lookup_table(HuffmanTree *htree) {
    codetable lookup;
    deque< pair<HuffmanTree *, code_t> > q;

    q.push_back(make_pair(htree, code_t()));
    while (!q.empty()) {
        HuffmanTree *node, *lc, *rc;
        code_t code;
        node = q.front().first;
        code = q.front().second;
        q.pop_front();
        lc = node->left;
        rc = node->right;
        if (lc) {
        
            code_t code_cp(code);
            q.push_back(make_pair(lc, (code.push_back(0), code)));
            q.push_back(make_pair(rc, (code_cp.push_back(1), code_cp)));
        } else {
            lookup.insert(make_pair(node->c, code));
            cout << "(" << node->c << ", ";
            for (unsigned i = 0; i < code.size(); i++) {
                cout << code[i];
            }
            cout << ")" << endl;
        }
    }

    return lookup;
}

code_t encode(string input, codetable &lookup) {
    code_t result;

    for (string::iterator it = input.begin(); it != input.end(); ++it) {
            code_t b = lookup[*it];
            result.insert(result.end(), b.begin(), b.end());
    }

    return result;
}

char code_lookup(code_t::iterator &biter, const code_t::iterator &biter_end,
                 const HuffmanTree *htree) {
    const HuffmanTree *node = htree;

    while (true) {
        if (!node->left) {
            // Huffman tree is full: always contains both children or none.
            break;
        }
        if (biter == biter_end) {
            throw std::out_of_range("No more bits");
        }
        if (*biter) {
            node = node->right;
        } else {
            node =node->left;
        }
        ++biter;
    }

    return node->c;
}


string decode(code_t &compressed, const HuffmanTree *htree) {
    string result;

    code_t::iterator biter = compressed.begin();
    while (true) {
        try {
            result += code_lookup(biter, compressed.end(), htree);
        } catch (const std::out_of_range &oor) {
            // Iterator exhausted.
            break;
        }
    }

    return result;
}

vector< pair<char, unsigned> > make_freq_table(string inp) {
    map<char, unsigned> cfmap;
    vector< pair<char, unsigned> >cfvec;

    for (unsigned i = 0; i < inp.size(); i++) {
        if (cfmap.find(inp[i]) == cfmap.end()) {
            cfmap.insert(make_pair(inp[i], 1));
        }
        cfmap[inp[i]] += 1;
    }

    for (map<char, unsigned>::iterator it = cfmap.begin();
         it != cfmap.end(); ++it) {
        cfvec.push_back(make_pair(it->first, it->second));
    }

    return cfvec;
}

string bitvec_to_string(code_t &bitvec) {
    string result;
    size_t nbits;

    nbits = bitvec.size() & 7;

    // Write the number of "hanging bits" at the first byte
    result += static_cast<char>(nbits); // at most 7

    char byte = 0;
    for (unsigned i = 0; i < bitvec.size(); i++) {
        unsigned boff = i & 7;
        byte |= bitvec[i] << boff;
        if (boff == 7) {
            // Write a byte
            result += byte;
            byte = 0;
        }
    }
    if (nbits) {
        result += byte;
    }

    return result;
}

code_t string_to_bitvec(string packed) {
    code_t result;

    assert(packed.size());
    if (packed.size() == 1) {
        assert(packed[0] == 0);
        return result;
    }
    unsigned nbits = packed[0];
    for (string::iterator it = packed.begin() + 1; it != packed.end(); ++it) {
        for (unsigned i = 0; i < 8; i++) {
            result.push_back((*it >> i) & 1);
        }
    }
    // fix the last byte
    if (nbits) {
        for (unsigned i = 0; i < (8 - nbits); i++) {
            result.pop_back();
        }
    }

    return result;
}

#include <cstdio>
void hexdump(const unsigned char *bytes, int nbytes) {
    int i, j;

    for (i = 0; i < nbytes; i += 16) {
        printf("%06x: ", i);
        for (j = 0; j < 16; j++) {
            if (i + j < nbytes) {
                printf("%02x ", bytes[i + j]);
            } else {
                printf("   ");
            }
        }
        printf(" ");
        for (j = 0; j < 16; j++) 
            if (i + j < nbytes)
                printf("%c", isprint(bytes[i+j]) ? bytes[i+j] : '.');
        printf("\n");
    }
}

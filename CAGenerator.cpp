#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//#include "CAGenerator.h"

using namespace std;

bool errdeb = false;

void carrodel(std::string& seqn) {
    if (*seqn.rbegin() == '\r')     {
        seqn.erase(seqn.length() - 1);
    }
}

vector<vector <int> > leerAffmx(const std::string &affmarch) {
   
    vector<vector<int> > affvec;
    ifstream affiarch(affmarch);

    if (!affiarch) {
        cerr << "Unable to open affinity file";
        exit(1);
    }

    string seqn;

    int nfeat = 0;
    while (std::getline(affiarch, seqn))
        ++nfeat;

    affiarch.clear();
    affiarch.seekg(0, std::ios::beg);

    affvec = vector<vector<int> >(nfeat, vector<int>(nfeat, 0));

    int fila = 0;
    while (getline(affiarch, seqn)) {
        carrodel(seqn);
        istringstream iss(seqn);
        string lineaff;
        getline(iss, lineaff);

        size_t actual;
        size_t siguinte = -1;
        int pilar = 0;
        do {
            actual = siguinte + 1;
            siguinte = lineaff.find_first_of(' ', actual);
            if (siguinte != string::npos) {
                affvec[fila][pilar] = stoi(lineaff.substr(actual, siguinte - actual));
            }
            pilar++;
        } while (siguinte != string::npos);
        fila++;
    }

    return affvec;
}

int limib (int cellone, int celltwo, int indices, const vector<vector<int> > &affvec, vector<int> &claro) {
    if (cellone < 0 || celltwo < 0) {
        return 0;
    }
    if (cellone > indices || celltwo > indices) {
        return 0;
    }

    int sum = 0;
    
    int z = 0;
    while (z < affvec[0].size()) {
        sum += affvec[z][claro[cellone]] * affvec[z][claro[celltwo]];
        z++;
    }

    return sum;
}

int contribution (int cellone, int celltwo, int cellthree, const vector<vector<int> > &affvec, vector<int> &claro) {
    int indices = celltwo;
    return 2*limib(cellone, celltwo, indices, affvec, claro)
           + 2*limib(celltwo, cellthree, indices, affvec, claro)
           - 2*limib(cellone, cellthree, indices, affvec, claro);
}


vector<vector<int> > affclust(vector<vector<int> > affvec) {
    unsigned long nfeat = affvec[0].size();
    vector<vector<int> > claf = vector<vector<int> >(nfeat, vector<int>(nfeat, 0));

    vector<int> claro;
    
    int i = 0;
    while (i < nfeat){
        claro.push_back(i);
        i++;
    }

    claf[0] = affvec[0];
    claf[1] = affvec[1];

    int indices = 2;
    
    
    while (indices < nfeat) {
        int maximo = -1;
        int rott = -1;
        int nghtf = 0;
        
        int i = 0;
        while (i < indices) {
            nghtf = contribution(i-1, indices, i, affvec, claro);
            if (nghtf > maximo) {
                maximo = nghtf;
                rott = i;
            }
            i++;
        }

        nghtf = contribution(indices - 1, indices, indices + 1, affvec, claro);
        if (nghtf > maximo) {
            rott = indices;
        }

        int j = indices;
        while (j > rott) {
            claf[j] = claf[j-1];
            claro[j] = claro[j-1];
            j--;
        }
        
        claf[rott] = affvec[indices];
        claro[rott] = indices;

        if (errdeb) {
            cout << "Order: ";
            
            int i = 0;
            while (i < claro.size()) {
                cout << claro[i] << ' ';
                i++;
            }
            
            cout << endl;
        }

        indices++;
    }

    if (errdeb) {
        cout << "Order: ";
        int i = 0;
        while (i < claro.size()) {
            cout << claro[i] << ' ';
            i++;
        }
        
        cout << endl;
        
        int uk = 0;
        while (uk < claf.size()) {
            int j = 0;
            while (claf[uk].size()) {
                cout << claf[uk][j] << ' ';
                j++;
            }
            cout << endl;
            uk++;
        }
        
        cout << endl;
    }
    
    int gh = 0;
    while (gh < nfeat) {
        int j = 0;
        while (j < nfeat) {
            claf[gh][j] = affvec[claro[gh]][claro[j]];
            j++;
        }
        gh++;
    }
   
    
    int gp = 0;
    while (gp < nfeat) {
        cout << "A" << claro[gp] + 1 << "\t";
        gp++;
    }
    
    cout << endl;

    int gy = 0;
    while (gy < claf.size()) {
        cout << "A" << claro[gy] + 1 << "\t";
        int j = 0;
        while (j < claf[gy].size()) {
            cout << claf[gy][j] << ' ';
            j++;
        }
        cout << endl;
        gy++;
    }
    

    return claf;
}


int main (int argc, char *argv[]) {

    std::string affmaxarch;

    if (argc != 2) {
        if (errdeb) {
            affmaxarch = "aa.txt";
        } else {
            return -1;
        }
    } else {
        affmaxarch = argv[1];
    }

    vector<vector<int> > affvec = leerAffmx(affmaxarch);

    if (errdeb) { 
        
        int pfi = 0;
        while ( pfi < affvec.size()) {
            int pfj = 0;
            while (pfj < affvec[pfi].size()) {
                cout << affvec[pfi][pfj] << ' ';
                pfj++;
            }
            pfi++;
        }
        
    }

    vector<vector<int> > claf = affclust(affvec);
}

#include <utility>

#include <utility>


//#include "AAGenerator.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

bool errdeb = false;

class atrt {
    std::string etiqueta;
public:
    atrt(string etiqueta, string nombre) {
        this->etiqueta = std::move(etiqueta);
        this->nombre = std::move(nombre);
    }

    std::string nombre;
};

class consulta {
    std::string etiqueta;
public:
    consulta(string etiqueta, string consulta) {
        this->etiqueta = std::move(etiqueta);
        this->consulta_s = std::move(consulta);
    }

    std::string consulta_s;
};

void carrodel(std::string& seqn) {
    if (*seqn.rbegin() == '\r')     {
        seqn.erase(seqn.length() - 1);
    }
}

std::vector<atrt> leyendoatrt(const std::string &attrfnombre) {
 
    vector<atrt> atrassign;
    ifstream attribute_file(attrfnombre);

    if (!attribute_file) {
        cerr << "Unable to open attribute file";
        exit(1);
    }

    string seqn;
    while (getline(attribute_file, seqn)) {
        carrodel(seqn);
        istringstream iss(seqn);
        string etiqueta; string nombre;
        getline(iss, etiqueta, ' ');
        getline(iss, nombre, ' ');
        atrassign.emplace_back(etiqueta, nombre);
    }
    atrassign.erase(atrassign.begin());

    if (errdeb) {
        cout << "Attributes:" << endl;
        
        int i = 0;
        while(i < atrassign.size()){
            cout << atrassign[i].nombre << endl;
            i++;
        }
        
        cout << endl;
    }

    return atrassign;
}

std::vector<consulta> leerconsulta(const std::string &consultarchnombre) {
    vector<consulta> consultass;
    ifstream archivconsulta(consultarchnombre);

    if (!archivconsulta) {
        cerr << "Unable to open query file";
        exit(1);
    }

    string seqn;
    while (getline(archivconsulta, seqn)) {
        carrodel(seqn);
        istringstream iss(seqn);
        string etiqueta; string consulta_s;
        getline(iss, etiqueta, ' ');
        getline(iss, consulta_s);
        consultass.emplace_back(etiqueta, consulta_s);
    }

    if (errdeb) {
        cout << "Queries:" << endl;
        
        int i = 0;
        while (i < consultass.size()){
            cout << consultass[i].consulta_s << endl;
            i++;
        }
        
        cout << endl;
    }

    return consultass;
}

vector<vector <int> > leerfrecuenc(const std::string &freqarchnom, unsigned long numcons) {
    
    vector<vector<int> > entran;
    ifstream entranfile(freqarchnom);

    if (!entranfile) {
        cerr << "Unable to open access file";
        exit(1);
    }

    string seqn;
    int consulta = -1;
    int pages = 0;
    while (getline(entranfile, seqn)) {
        carrodel(seqn);
        istringstream iss(seqn);
        string etiqueta; string entranflin;
        getline(iss, etiqueta, ' ');
        getline(iss, entranflin);

        size_t actual;
        size_t siguinte = -1;
        int page = 0;
        do {
            actual = siguinte + 1;
            siguinte = entranflin.find_first_of(' ', actual);
            if (consulta < 0) {
                pages++;
                if (siguinte == string::npos) {
                    entran = vector<vector<int> >(numcons, vector<int>(pages, 0));
                }
            } else {
                entran[consulta][page] = stoi(entranflin.substr(actual, siguinte - actual));
                page++;
            }
            if (siguinte == string::npos) {
                consulta++;
            }
        }
        while (siguinte != string::npos);
    }

    if (errdeb) {
        cout << "Access:" << endl;
        
        int i = 0;
        while (i < entran.size()){
            cout << "q" << i+1 << ' ';
            int j = 0;
            while (j < entran[i].size()){
                cout << entran[i][j] << ' ';
                j++;
            }
            i++;
            cout << endl;
        }
        
        cout << endl;
    }

    return entran;
}

vector<vector<int> > llenaruso(vector<atrt> atrassign, vector<consulta> consultass) {
    vector<vector<int> > trato = vector<vector<int> >(consultass.size(), vector<int>(atrassign.size(), 0));
    
    int i = 0;
    while(i < consultass.size()) {
        int j = 0;
        while (j < atrassign.size()) {
            if (consultass[i].consulta_s.find(atrassign[j].nombre) != std::string::npos &&
                    !isalnum(consultass[i].consulta_s.at(consultass[i].consulta_s.find(atrassign[j].nombre) - 1))) {
                trato[i][j] = 1;
                
                if (errdeb) {
                    cout << consultass[i].consulta_s << " uses " << atrassign[j].nombre << endl;
                }
            } else {
                trato[i][j] = 0;
            }
        j++;
        }
    i++;
    }
    
    if (errdeb) {
        cout << "Usage:" << endl;
        
        int i = 0;
        while (i < trato.size()){
            cout << "q" << i+1 << ' ';
            int j = 0;
            while(trato[i].size()){
                cout << trato[i][j] << ' ';
                j++;
            }
            cout << endl;
            i++;
        }
        
        
        cout << endl;
    }

    return trato;
}

bool paraglibro = false;
vector<vector<int> > llenaraffmatrix(vector<vector<int> > trato, vector<vector<int> > entran, unsigned long nfeat, unsigned long numcons) {
    vector<vector<int> > affvec = vector<vector<int> >(nfeat, vector<int>(nfeat, 0));
    int eject = 1;

    int i = 0;
    while (i < nfeat) {
        int j = 0;
        while (j < nfeat){
            affvec[i][j] = 0;
            if (paraglibro) {
                int k = 0;
                while (k < numcons){
                    if (trato[k][i] == 1 && trato[k][j] == 1){
                        int x = 0;
                        while (x < entran[k].size()) {
                            affvec[i][j] += eject * entran[k][x];
                            x++;
                        }
                    }
                    k++;
                }
                
            } else {
                int totalsuma[numcons];
                
                int z = 0;
                while (z < numcons) {
                    totalsuma[z] = 0;
                    int y = 0;
                    while (y < entran[z].size()){
                        totalsuma[z] += entran[z][y];
                        y++;
                    }
                    z++;
                }
                
                float numerad = 0;
                float denominuno = 0;
                float denomintwo = 0;
                
                int k = 0;
                while ( k < numcons){
                    numerad += (trato[k][i] * totalsuma[k]) * (trato[k][j] * totalsuma[k]);
                    denominuno += trato[k][i] * totalsuma[k];
                    denomintwo += trato[k][j] * totalsuma[k];
                    k++;
                }
                
                affvec[i][j] = static_cast<int>(ceil(numerad / sqrt(denominuno * denomintwo)));
                if (affvec[i][j] < 0) {
                    affvec[i][j] = 0;
                }
                
                
            }
            j++;
        }
        i++;
    }
    
    return affvec;
}


int main (int argc, char *argv[]) {

    std::string attr_arch;
    std::string archivconsulta;
    std::string accfrearch;

    if (argc != 4) { 
        if (errdeb) {
            attr_arch = "att.txt";
            archivconsulta = "query.txt";
            accfrearch = "acc.txt";
        } else {
            return -1;
        }
    } else {
        attr_arch = argv[1];
        archivconsulta = argv[2];
        accfrearch = argv[3];
    }

    vector<atrt> atrassign = leyendoatrt(attr_arch);
    vector<consulta> consultass = leerconsulta(archivconsulta);
    vector<vector<int> > trato = llenaruso(atrassign, consultass);
    vector<vector<int> > entran = leerfrecuenc(accfrearch, consultass.size());

    vector<vector<int> > affvec = llenaraffmatrix(trato, entran, atrassign.size(), consultass.size());

    int i = 0;
    while (i < affvec.size()) {
        int j = 0;
        while (j < affvec[i].size()){
            cout << affvec[i][j] << ' ';
            j++;
        }
        cout << endl;
        i++;
    }
    
}
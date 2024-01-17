#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <functional>
#include <algorithm>
using namespace std;


struct Wezel {
    int x, y;
    int g, h;
    Wezel* rodzic;

    Wezel(int x, int y, int g, int h, Wezel* rodzic) : x(x), y(y), g(g), h(h), rodzic(rodzic) {}

    int pobierzF() const {
        return g + h;
    }
};

vector<vector<int>> wczytajMape(const string& nazwaPliku) {
    ifstream plik(nazwaPliku);
    vector<vector<int>> mapa;

    if (plik.is_open()) {
        int wartosc;
        vector<int> wiersz;

        while (plik >> wartosc) {
            wiersz.push_back(wartosc);

            if (plik.peek() == '\n' || plik.peek() == EOF) {
                mapa.push_back(wiersz);
                wiersz.clear();
            }
        }

        plik.close();
    }
    else {
        cerr << "Nie mozna otworzyc pliku " << nazwaPliku << endl;
    }

    return mapa;
}

bool czyPunktPoprawny(int x, int y, const vector<vector<int>>& mapa) {
    return x >= 0 && x < mapa.size() && y >= 0 && y < mapa[0].size();
}

vector<pair<int, int>> znajdzSciezke(const vector<vector<int>>& mapa, const pair<int, int>& start, const pair<int, int>& meta) {
    if (!czyPunktPoprawny(start.first, start.second, mapa) || !czyPunktPoprawny(meta.first, meta.second, mapa) || mapa[start.first][start.second] == 5) {
        cerr << "O NIE!! Start znajduje sie na przeszkodzie, lub Start/Meta znajduja sie poza Mapa :(" << endl;
        return vector<pair<int, int>>();
    }

    // Ruchy Klocuchy
    const int dx[] = { 1, -1, 0, 0 };
    const int dy[] = { 0, 0, 1, -1 };

    Wezel* wezelStartowy = new Wezel(start.first, start.second, 0, 0, nullptr);
    Wezel* wezelMeta = new Wezel(meta.first, meta.second, 0, 0, nullptr);

    priority_queue<Wezel*, vector<Wezel*>, function<bool(Wezel*, Wezel*)>> zbiorOtwarty([](Wezel* a, Wezel* b) {
        return a->pobierzF() > b->pobierzF();
        });

    zbiorOtwarty.push(wezelStartowy);

    vector<vector<bool>> odwiedzone(mapa.size(), vector<bool>(mapa[0].size(), false));

    int kroki = 0;

    while (!zbiorOtwarty.empty()) {
        Wezel* obecny = zbiorOtwarty.top();
        zbiorOtwarty.pop();
        odwiedzone[obecny->x][obecny->y] = true;

        if (obecny->x == wezelMeta->x && obecny->y == wezelMeta->y) {
            vector<pair<int, int>> sciezka;
            while (obecny != nullptr) {
                sciezka.push_back({ obecny->x, obecny->y });
                obecny = obecny->rodzic;
            }
            delete wezelStartowy;
            delete wezelMeta;
            reverse(sciezka.begin(), sciezka.end());

            // Wyświetlenie ścieżki
            cout << "Znaleziona sciezka:" << endl;
            for (const auto& punkt : sciezka) {
                cout << "(" << punkt.first << ", " << punkt.second << ") ";
            }
            cout << endl;

            // Wyświetlenie liczby kroków
            cout << "Liczba krokow: " << kroki << endl;
            return sciezka;
        }

        for (int i = 0; i < 4; ++i) {
            int nowyX = obecny->x + dx[i];
            int nowyY = obecny->y + dy[i];
            if (czyPunktPoprawny(nowyX, nowyY, mapa) && mapa[nowyX][nowyY] == 0 && !odwiedzone[nowyX][nowyY]) {
                int noweG = obecny->g + 1;
                int noweH = abs(nowyX - wezelMeta->x) + abs(nowyY - wezelMeta->y);
                Wezel* nowyWezel = new Wezel(nowyX, nowyY, noweG, noweH, obecny);
                zbiorOtwarty.push(nowyWezel);

                // Zliczanie kroków
                kroki++;
            }
        }
    }

    cerr << "Brak sciezki" << endl;
    delete wezelStartowy;
    delete wezelMeta;
    return vector<pair<int, int>>();
}

int main() {

    vector<vector<int>> mapa = wczytajMape("grid.txt");

    int start_x, start_y, meta_x, meta_y;
    cout << "Skad startujesz? (x y): \n";
    cout << "Podaj X = ";
    cin >> start_x;
    cout << "Podaj Y = ";
    cin >> start_y;
    cout << "Gdzie wyladujesz (x y): \n";
    cout << "Podaj X = ";
    cin >> meta_x;
    cout << "Podaj Y = ";
    cin >> meta_y;

    pair<int, int> start = { start_x, start_y };
    pair<int, int> meta = { meta_x, meta_y };

    vector<pair<int, int>> sciezka = znajdzSciezke(mapa, start, meta);

    return 0;
}
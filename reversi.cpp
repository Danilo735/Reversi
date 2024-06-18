#include <bits/stdc++.h>
#include <vector>
#include <limits>

using namespace std;

// postavljamo velicinu table na 8, prazno se oznacava ".", crni ceker sa "C", beli ceker sa "B"
const int VELICINA_TABLE = 8;
const char PRAZNO = '.';
const char CRNI = 'C';
const char BELI = 'B';

//svi mogući smerovi za proveru okolnih ćelija na tabli
const int pravci[8][2] = {              // 8 redova, dve tacke
    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1},// gore levo, gore, gore desno, levo
    {0, 1}, {1, -1}, {1, 0}, {1, 1}     // desno, dole levo, dole, dole desno
};

void inicijalizujTablu(vector<vector<char>>& tabla) {//radimo sa originalom. ako bi radili sa kopijama, promene bi bile lokalne
    tabla = vector<vector<char>>(VELICINA_TABLE, vector<char>(VELICINA_TABLE, PRAZNO)); //pravi se tabla i sve ste postavlja na prazno
    tabla[3][3] = BELI;
    tabla[3][4] = CRNI;
    tabla[4][3] = CRNI;
    tabla[4][4] = BELI;
    //pocetne pozicije cekera}

void prikaziTablu(const vector<vector<char>>& tabla) { //radimo sa originalom i prikazujemo tablu
    cout << "  ";
    for (int i = 0; i < VELICINA_TABLE; i++) { //postavljamo redne brojeve kolona
        cout << i << " ";}
    cout << endl;
    for (int i = 0; i < VELICINA_TABLE; i++) {
        cout << i << " ";
        for (int j = 0; j < VELICINA_TABLE; j++) {//postavljamo redne brojeve redova
            cout << tabla[i][j] << " ";}
        
        cout << endl;}}

bool potezispravan(const vector<vector<char>>& tabla, char igrac, int x, int y) { // radimo sa originalom, zatim igrac koji je na redu i kordinate x i y za odredjeni ceker
    if (x < 0 || x >= VELICINA_TABLE || y < 0 || y >= VELICINA_TABLE || tabla[x][y] != PRAZNO) return false; //provera da li su kordinate x i y u redu

    //odredjivanje simbola protivnika
    char protivnik = (igrac == CRNI) ? BELI : CRNI;
    //petlja za proveru vertikalno i horizontalno
    for (int i = 0; i < 8; i++) {
        int nx = x + pravci[i][0]; //sledeca kordinata na x osi u zadatom i u petlji
        int ny = y + pravci[i][1]; //sledeca kordinata na y osi u zadatom i u petlji
        bool pronadjenProtivnik = false; //da li je pronadjen protivnik tokom provere u petlji, na pocetku se postavlja na false posto pre igre jos uvek nismo nasli protivnika da mozemo da je psotavimo na false 
        
        //provera tokom igre
        while (nx >= 0 && nx < VELICINA_TABLE && ny >= 0 && ny < VELICINA_TABLE && tabla[nx][ny] == protivnik) { //da li su kordinate x i y u zadatoj tabli(da idu do 8)
                                                                                   // da li se na zadatom polju nalazi protivnik, ako se nalazi onda se petlja nastavlja    
            nx += pravci[i][0]; //pomeramo x i y 
            ny += pravci[i][1]; //prvo polje se preskace
            pronadjenProtivnik = true; //pronasli smo protivnika pa postavljamo na true
        }
        //Da li smo nasli protivnika i da li je on vec postavio ceker na zadatu poziciju
        if (pronadjenProtivnik && nx >= 0 && nx < VELICINA_TABLE && ny >= 0 && ny < VELICINA_TABLE && tabla[nx][ny] == igrac) {
            return true;//potez je ispravan
        }}
    return false;//potez nije ispravan
}

vector<pair<int, int>> dobaviValidnePoteze(const vector<vector<char>>& tabla, char igrac) {//da li su svi mogući potezi na toj poziciji ispravni za određenog igrača
    vector<pair<int, int>> potezi;//vektor parova celih brojeva u koji se dodaju koordinate validnih poteza.
    for (int i = 0; i < VELICINA_TABLE; i++) {
        for (int j = 0; j < VELICINA_TABLE; j++) {
            if (potezispravan(tabla, igrac, i, j)) { //proverava da li je potez validan za igrača na trenutnoj poziciji
                potezi.emplace_back(i, j); //koordinate se vracaju u poteze
            }}}
    return potezi;}

void napraviPotez(vector<vector<char>>& tabla, char igrac, int x, int y) { //izvrsavamo potez
    tabla[x][y] = igrac; //igracevu figuru postavlja na poziciju x,y
    char protivnik = (igrac == CRNI) ? BELI : CRNI; // odredjuje simbol protivnika  (ako je crni, protivnik je beli)
    for (int i = 0; i < 8; i++) { //sve osam koordinate
        int nx = x + pravci[i][0];
        int ny = y + pravci[i][1]; //koordinate za proveru
        vector<pair<int, int>> zaOkretanje; // vector koji cuva koordinate koje treba preokrenuti
        while (nx >= 0 && nx < VELICINA_TABLE && ny >= 0 && ny < VELICINA_TABLE && tabla[nx][ny] == protivnik) { // izvršava se dok su koordinate unutar granica table i dok se na toj poziciji nalazi protivnička figura. 
            zaOkretanje.emplace_back(nx, ny); //koordinate se vracaju u zaokretanje
            nx += pravci[i][0]; // koordinate se pomeraju kako bi se nastavilo sa ispitivanjem
            ny += pravci[i][1];}
        if (nx >= 0 && nx < VELICINA_TABLE && ny >= 0 && ny < VELICINA_TABLE && tabla[nx][ny] == igrac) { // proverava se da li se na kraju nalazi figura igrača, tj validan potez 
            for (const auto& p : zaOkretanje) { // p = koordinate jedne figure protivnika
                tabla[p.first][p.second] = igrac;
            }//ako su pronadjene svi cekeri se menjaju u boju igraca koji je stavio
        }}}

bool imaValidnihPoteza(const vector<vector<char>>& tabla, char igrac) { // vraca tacno ili netacno
    return !dobaviValidnePoteze(tabla, igrac).empty(); // da li je vektor prazan
    // ako je prazan, ! vraca false
    // ako nije prazan, ! vraca true
    // Funkcija vraća true ako igrač ima barem jedan validan potez, a false ako nema nijedan validan potez
}

void igrajReversi() { // funkicja za pokretanje igre
    vector<vector<char>> tabla;
    inicijalizujTablu(tabla);
    char trenutniIgrac = CRNI; // prvi igrac je uvek crni

    while (true) { //sve dok moze(tacno)
        prikaziTablu(tabla);
        vector<pair<int, int>> potezi = dobaviValidnePoteze(tabla, trenutniIgrac); //kordinate poteza koje smo dobili iz funkcije
        if (potezi.empty()) { // ako nema poteza
            if (!imaValidnihPoteza(tabla, (trenutniIgrac == CRNI) ? BELI : CRNI)) { //ako niko vise ne moze da igra
                cout << "Nema vise validnih poteza za oba igraca. Kraj igre!" << endl;
                break; //izlazi se
            }
            cout << "Nema validnih poteza za " << trenutniIgrac << ". Preskakanje poteza." << endl; // kad jedan igrac nema potez, drugi igrac mora da igra umesto njega
            trenutniIgrac = (trenutniIgrac == CRNI) ? BELI : CRNI;
            continue; //nastavljamo dalje
        }

        cout << "Igrac " << trenutniIgrac << ", unesite svoj potez (red i kolona): "; //radi lakseg redosleda ko je na redu
        int x, y;
        cin >> x >> y;

        if (cin.fail() || !potezispravan(tabla, trenutniIgrac, x, y)) { //neispravan unos ili potes nije ispravan
            cin.clear(); // Očisti greške
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Odbaci nevalidan unos
            cout << "Nevazeci potez. Pokusajte ponovo." << endl; // daje sansu da igrac ponovo pokusa
            continue; // nastavljamo dalje
        }

        napraviPotez(tabla, trenutniIgrac, x, y);
        trenutniIgrac = (trenutniIgrac == CRNI) ? BELI : CRNI;}

    int brojCrnih = 0, brojBelih = 0; // broj cekera na tabli se deklarise
    for (const auto& red : tabla) { 
        for (char celija : red) {
            if (celija == CRNI) brojCrnih++;
            if (celija == BELI) brojBelih++;
            //uvecava se broj cekera na tabli
        }}
    cout << "CRNI: " << brojCrnih << " BELI: " << brojBelih << endl; //kolko koji igrac ima cekera na tabli
    cout << "Pobednik: " << (brojCrnih > brojBelih ? "CRNI" : (brojBelih > brojCrnih ? "BELI" : "Nerešeno")) << endl; // ko je pobednik ili da li je nereseno
}

int main() {
    igrajReversi(); //pokrecemo igru
    return 0;}

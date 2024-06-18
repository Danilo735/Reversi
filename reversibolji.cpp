#include <iostream>
#include <vector>
#include <limits>   //ogranicenja
#include <cstdlib>  //generisanje brojeva, uprvavljanje memorijom, 
#include <ctime>    //rad sa vremenom
//ucitavamo neophodne biblioteke za rad

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
    tabla = vector<vector<char>>(VELICINA_TABLE, vector<char>(VELICINA_TABLE, PRAZNO));//pravi se tabla i sve ste postavlja na prazno
    tabla[3][3] = BELI;
    tabla[3][4] = CRNI;
    tabla[4][3] = CRNI;
    tabla[4][4] = BELI;
    //pocetne pozicije cekera
}

void prikaziTablu(const vector<vector<char>>& tabla) {//radimo sa originalom i prikazujemo tablu
    cout << "  ";
    for (int i = 0; i < VELICINA_TABLE; i++) {//postavljamo redne brojeve kolona
        cout << i << " ";
    }
    cout << endl;
    for (int i = 0; i < VELICINA_TABLE; i++) {
        cout << i << " ";
        for (int j = 0; j < VELICINA_TABLE; j++) {//postavljamo redne brojeve redova
            cout << tabla[i][j] << " ";
        }
        cout << endl;
    }
}

bool potezispravan(const vector<vector<char>>& tabla, char igrac, int x, int y) {// radimo sa originalom, zatim igrac koji je na redu i kordinate x i y za odredjeni ceker
    if (x < 0 || x >= VELICINA_TABLE || y < 0 || y >= VELICINA_TABLE || tabla[x][y] != PRAZNO) return false;//provera da li su kordinate x i y u redu

    //odredjivanje simbola protivnika
    char protivnik = (igrac == CRNI) ? BELI : CRNI;

    //petlja za proveru vertikalno i horizontalno
    for (int i = 0; i < 8; i++) {
        int nx = x + pravci[i][0];//sledeca kordinata na x osi u zadatom i u petlji
        int ny = y + pravci[i][1];//sledeca kordinata na y osi u zadatom i u petlji
        bool pronadjenProtivnik = false;//da li je pronadjen protivnik tokom provere u petlji, na pocetku se postavlja na false posto pre igre jos uvek nismo nasli protivnika da mozemo da je psotavimo na false 

        //provera tokom igre
        while (nx >= 0 && nx < VELICINA_TABLE && ny >= 0 && ny < VELICINA_TABLE && tabla[nx][ny] == protivnik) {//da li su kordinate x i y u zadatoj tabli(da idu do 8)
                                                                                   // da li se na zadatom polju nalazi protivnik, ako se nalazi onda se petlja nastavlja
            nx += pravci[i][0];//pomeramo x i y
            ny += pravci[i][1];//prvo polje se preskace
            pronadjenProtivnik = true;//pronasli smo protivnika pa postavljamo na true
        }
        ////Da li smo nasli protivnika i da li je on vec postavio ceker na zadatu poziciju
        if (pronadjenProtivnik && nx >= 0 && nx < VELICINA_TABLE && ny >= 0 && ny < VELICINA_TABLE && tabla[nx][ny] == igrac) {
            return true;//potez je ispravan
        }
    }
    return false;//potez nije ispravan
}

vector<pair<int, int>> dobaviValidnePoteze(const vector<vector<char>>& tabla, char igrac) {//da li su svi mogući potezi na toj poziciji ispravni za određenog igrača
    vector<pair<int, int>> potezi;//vektor parova celih brojeva u koji se dodaju koordinate validnih poteza.
    for (int i = 0; i < VELICINA_TABLE; i++) {
        for (int j = 0; j < VELICINA_TABLE; j++) {
            if (potezispravan(tabla, igrac, i, j)) {//proverava da li je potez validan za igrača na trenutnoj poziciji
                potezi.emplace_back(i, j);//koordinate se vracaju u poteze
            }
        }
    }
    return potezi;
}

void napraviPotez(vector<vector<char>>& tabla, char igrac, int x, int y) {//izvrsavamo potez
    tabla[x][y] = igrac;//igracevu figuru postavlja na poziciju x,y
    char protivnik = (igrac == CRNI) ? BELI : CRNI;// odredjuje simbol protivnika  (ako je crni, protivnik je beli, i obrnuto)
    for (int i = 0; i < 8; i++) {//sve osam koordinate
        int nx = x + pravci[i][0];
        int ny = y + pravci[i][1];//koordinate za proveru
        vector<pair<int, int>> zaOkretanje;// vector koji cuva koordinate koje treba preokrenuti
        while (nx >= 0 && nx < VELICINA_TABLE && ny >= 0 && ny < VELICINA_TABLE && tabla[nx][ny] == protivnik) {// izvršava se dok su koordinate unutar granica table i dok se na toj poziciji nalazi protivnička figura. 
            zaOkretanje.emplace_back(nx, ny);//koordinate se vracaju u zaokretanje
            nx += pravci[i][0];// koordinate se pomeraju kako bi se nastavilo sa ispitivanjem
            ny += pravci[i][1];
        }
        if (nx >= 0 && nx < VELICINA_TABLE && ny >= 0 && ny < VELICINA_TABLE && tabla[nx][ny] == igrac) {// proverava se da li se na kraju nalazi figura igrača, tj validan potez 
            for (const auto& p : zaOkretanje) {// p = koordinate jedne figure protivnika
                tabla[p.first][p.second] = igrac;
            }//ako su pronadjene svi cekeri se menjaju u boju igraca koji je stavio
        }
    }
}

bool imaValidnihPoteza(const vector<vector<char>>& tabla, char igrac) {// vraca tacno ili netacno
    return !dobaviValidnePoteze(tabla, igrac).empty();// da li je vektor prazan
    // ako je prazan, ! vraca false
    // ako nije prazan, ! vraca true
    // Funkcija vraća true ako igrač ima barem jedan validan potez, a false ako nema nijedan validan potez
}

//Dodatak proslog koda


pair<int, int> odaberiNajboljiPotez(const vector<vector<char>>& tabla, char igrac) { //funkcija za najbolji potez koja ce vratiti koordinate
    vector<pair<int, int>> potezi = dobaviValidnePoteze(tabla, igrac); //Poziva funkciju da bi se dobili svi validni potezi za trenutnog igraca
    pair<int, int> najboljiPotez = potezi[0]; //Postavlja se najbolji potez za sada
    int maxOkretanje = -1; //maksimalni broj okretanja tokom analize poteza

    for (const auto& potez : potezi) { //prolazi kroz sve validne poteze
        vector<vector<char>> kopijaTable = tabla; //Pravi se kopija trenutne table kako bi se analizirao svaki potez posebno
        napraviPotez(kopijaTable, igrac, potez.first, potez.second);//Izvršava se potez na kopiji table kako bi se analizirali rezultati tog poteza
        int brojOkretanja = 0;//brojac koji će računati koliko se čekera okreće tokom ovog poteza

        for (int i = 0; i < VELICINA_TABLE; i++) {//petlja za izracunavanje broja okretanja
            for (int j = 0; j < VELICINA_TABLE; j++) {
                if (kopijaTable[i][j] == igrac && tabla[i][j] != igrac) {//da li je na odredjenoj poziciji bila protivnicki ceker a sada ceker trenutnog igraca (nakon izvrsavanja)
                    brojOkretanja++;
                }
            }
        }

        if (brojOkretanja > maxOkretanje) {//da li je broj okretanja za ovaj potez veći od trenutno maksimalnog broja okretanja.
            maxOkretanje = brojOkretanja;
            najboljiPotez = potez;
            //azuriramo parametre
        }
    }

    return najboljiPotez;
}

void igrajReversi() {// funkicja za pokretanje igre
    vector<vector<char>> tabla;
    inicijalizujTablu(tabla);
    char trenutniIgrac = CRNI;// prvi igrac je uvek crni
    bool protivKompjutera = false; //trenutno nije odabrana opcija igranja protiv kompjutera
    char igracBoja, kompjuterBoja; //cuva boje

    cout << "Da li zelite da igrate protiv kompjutera? (da/ne): ";
    string odgovor;//cuva odgovor na zadato pitanje
    cin >> odgovor;
    if (odgovor == "da" || odgovor == "DA" || odgovor == "Da") { // proverava se da li je igrac odgovorio sa da
        protivKompjutera = true; //omogucavamo da se igra protiv kompjutera
    }

    cout << "Koju boju zelite da igrate? (crnu/belu): "; // koju boju zeli igrac da igra
    cin >> odgovor;
    if (odgovor == "crnu" || odgovor == "CRNU" || odgovor == "Crnu") { //proverava se odgovor igraca
        igracBoja = CRNI; //ako je dao odgovor crnu, onda ce on igrati crnu
        kompjuterBoja = BELI; //kompjuter je bela u tom slucaju
    } else { //ako nije dao taj odgovor
        igracBoja = BELI; //igrac je beli
        kompjuterBoja = CRNI;//kompjuter je u tom slucaju crni
    }

    while (true) {//sve dok moze(tacno)
        prikaziTablu(tabla);
        vector<pair<int, int>> potezi = dobaviValidnePoteze(tabla, trenutniIgrac);//kordinate poteza koje smo dobili iz funkcije
        if (potezi.empty()) {// ako nema poteza
            if (!imaValidnihPoteza(tabla, (trenutniIgrac == CRNI) ? BELI : CRNI)) {//da li nijedan od igrača više nema validne poteze
                cout << "Nema vise validnih poteza za oba igraca. Kraj igre!" << endl;
                break;//izlazi se
            }
            cout << "Nema validnih poteza za " << trenutniIgrac << ". Preskakanje poteza." << endl;// kad jedan igrac nema potez, drugi igrac mora da igra umesto njega
            trenutniIgrac = (trenutniIgrac == CRNI) ? BELI : CRNI;//ako trenutni igrač nema potez, prelazi se na sledećeg igrača.
            continue;//nastavljamo dalje
        }

        if ((trenutniIgrac == igracBoja) || (!protivKompjutera && trenutniIgrac != igracBoja)) { //da li je trenutni igrač igrač čiji je potez na redu ili je igrač koji igra protiv kompjutera
            cout << "Igrac " << trenutniIgrac << ", unesite svoj potez (red i kolona): ";//radi lakseg sporazumevanja ko je na redu
            int x, y;
            cin >> x >> y; //unosi se red i kolona

            if (cin.fail() || !potezispravan(tabla, trenutniIgrac, x, y)) {//neispravan unos ili potes nije ispravan
                cin.clear();// Očisti se unos
                cin.ignore(numeric_limits<streamsize>::max(), '\n');// Odbaci nevalidan unos
                cout << "Nevazeci potez. Pokusajte ponovo." << endl;// daje sansu da igrac ponovo pokusa
                continue;// nastavljamo dalje
            }

            napraviPotez(tabla, trenutniIgrac, x, y); //izvrsava se potez
        } else {
            pair<int, int> potez = odaberiNajboljiPotez(tabla, kompjuterBoja); //najbolji potez za kompjuter ako je na redu
            napraviPotez(tabla, kompjuterBoja, potez.first, potez.second);//izvrsava se potez
            cout << "Kompjuter (" << kompjuterBoja << ") je odigrao potez: " << potez.first << " " << potez.second << endl; //poruka sta je kompjuter odigrao
        }

        trenutniIgrac = (trenutniIgrac == CRNI) ? BELI : CRNI; // prebacuje se na sledeceg igraca 
    }

    int brojCrnih = 0, brojBelih = 0;// broj cekera na tabli se deklarise
    for (const auto& red : tabla) {
        for (char celija : red) {
            if (celija == CRNI) brojCrnih++;
            if (celija == BELI) brojBelih++;
            //uvecava se broj cekera na tabli
        }
    }
    cout << "CRNI: " << brojCrnih << " BELI: " << brojBelih << endl;//kolko koji igrac ima cekera na tabli
    cout << "Pobednik: " << (brojCrnih > brojBelih ? "CRNI" : (brojBelih > brojCrnih ? "BELI" : "Nerešeno")) << endl;// ko je pobednik ili da li je nereseno
}

int main() {
    srand(time(0));
    //srand uzima tzv seme, kada se time postavi na 0 uzima se svaka sekunda od 1.januara 1970. 
    //uzima se ta vrednost i ona se koristi za generisanje brojeva
    //svaki put kada pokrenemo program mozemo ocekivati druge nizove slucajnih brojeva
    igrajReversi();//pokrecemo igru
    return 0;
}

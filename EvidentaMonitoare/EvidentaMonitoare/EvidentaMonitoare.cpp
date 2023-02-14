#include <iostream>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

enum TipLCD { TN, IPS, VA };
enum FormatLED { HD, FHD };


class Monitor {
private:
    string prod;          // producator monitor
    int diag;             // diagonala 
    int pret;             // pret monitor
public:
    Monitor(string prod, int diag, int pret)
    {
        this->prod = prod;
        this->diag = diag;
        this->pret = pret;
    }

    string getProd()
    {
        return prod;
    }

    int getDiag()
    {
        return diag;
    }

    virtual void Afisare()
    {
        cout << "Producator: " << prod << "\n";
        cout << "Diagonala: " << diag << "\n";
        cout << "Pret: " << pret << "\n";
    }
};

class MonitorLCD : public Monitor {
private:
    int timp;           // timp de raspuns
    TipLCD tip;         // tip ecran
public:
    MonitorLCD(string prod, int diag, int pret, int timp, TipLCD tip) : Monitor(prod, diag, pret)
    {
        this->timp = timp;
        this->tip = tip;
    }
    void Afisare() override
    {
        Monitor::Afisare();
        cout << "Timp de raspuns: " << timp << "\n";
        switch (tip)
        {
        case 0:
            cout << "Ecran TN\n";
            break;
        case 1:
            cout << "Ecran IPS\n";
            break;
        case 2:
            cout << "Ecran VA\n";
            break;
        }
    }
};

class MonitorLED : public Monitor {
private:
    FormatLED format;           // format ecran
    bool iluminare;             // iluminare (DA/NU)
    bool boxe;                  // boxe integrate (DA/NU)
public:
    MonitorLED(string prod, int diag, int pret, FormatLED format, bool iluminare, bool boxe) : Monitor(prod, diag, pret)
    {
        this->format = format;
        this->iluminare = iluminare;
        this->boxe = boxe;
    }
    void Afisare() override
    {
        Monitor::Afisare();
        switch (format)
        {
        case 0:
            cout << "Format HD\n";
            break;
        case 1:
            cout << "Format FHD\n";
            break;
        }
        switch (iluminare)
        {
        case 0:
            cout << "Fara iluminare\n";
            break;
        default:
            cout << "Cu iluminare\n";
            break;
        }
        switch (boxe)
        {
        case 0:
            cout << "Fara boxe\n";
            break;
        default:
            cout << "Cu boxe\n";
            break;
        }
    }
};

void AdaugareM(list<Monitor*>& m) // functia de adaugare a unui monitor
{
    int tip;
    string prod;
    int diag, pret;
    do {
        cout << "Introduceti tipul de monitor dorit (1-LCD, 2-LED): ";
        cin >> tip;
        switch (tip)
        {
        default:
            cout << "Optiune invalida!\n";
            break;
        case 1:
            int timp, tip;
            cout << "Nume producator: ";
            cin >> prod;
            cout << "Dimensiune diagonala: ";
            cin >> diag;
            cout << "Pret: ";
            cin >> pret;
            cout << "Timp de raspuns: ";
            cin >> timp;
            do {
                cout << "Tip LCD (0-TN, 1-IPS, 2-VA): ";
                cin >> tip;
            } while (tip < 0 || tip > 2);
            m.push_back(new MonitorLCD(prod, diag, pret, timp, (TipLCD)tip));
            return;
            break;
        case 2:
            int format;
            int iluminare, boxe;
            cout << "Nume producator: ";
            cin >> prod;
            cout << "Dimensiune diagonala: ";
            cin >> diag;
            cout << "Pret: ";
            cin >> pret;
            do {
                cout << "Format LED(0-HD, 1-UHD): ";
                cin >> format;
            } while (format < 0 || format > 1);
            // presupunem ca pentru orice valoare diferita de 0, optiunile de mai jos sunt incluse
            cout << "Cu/fara iluminare (0-NU): ";
            cin >> iluminare;
            cout << "Cu/fara boxe (0-NU): ";
            cin >> boxe;
            if (iluminare != 0) iluminare = 1;
            if (boxe != 0) boxe = 1;
            m.push_back(new MonitorLED(prod, diag, pret, (FormatLED)format, iluminare, boxe));
            return;
            break;
        }
        cout << "Eroare: tipul nu exista - incercati din nou!\n";
    } while (1);
}

void AfisareM(list<Monitor*>m) // functia de afisare a monitoarelor
{
    if (m.size() == 0) cout << "Nu exista monitoare in stoc\n";
    for (auto i = m.begin(); i != m.end(); i++)
    {
        (*i)->Afisare();
        cout << "\n";
    }
}

void StergereM(list<Monitor*>& m) // functia de stergere a unui monitor
{
    string nume;
    cout << "Introdu numele producatorului: ";
    cin >> nume;
    bool ok = 0;
    // va sterge prima aparitie a numelui de producator
    for (auto i = m.begin(); i != m.end();)
        if ((*i)->getProd() == nume)
        {
            ok = 1;
            // metoda erase() din cadrul listei returneaza un pointer la urmatorul element din lista dupa stergere
            if (i != m.end()) i = m.erase(i);
            else return;
        }
        else i++;
    if (ok == 0) cout << "Producatorul " << nume << "nu a fost gasit in stoc!\n";
}

void CautareM(list<Monitor*>m) // functia de cautare a unui monitor
{
    string nume;
    cout << "Introdu numele producatorului: ";
    cin >> nume;
    for (auto i = m.begin(); i != m.end(); i++)
        if ((*i)->getProd() == nume)
            (*i)->Afisare();
}

bool Ordonare(Monitor* a, Monitor* b)
{
    return  a->getDiag() < b->getDiag();
}

int main()
{
    list<Monitor*>monitoare;    // lista eterogena pentru monitoare
    int opt;                    // optiune meniu
    do {
        system("CLS");
        cout << "1. Introducerea unui monitor\n";
        cout << "2. Afisarea monitoarelor din stoc\n";
        cout << "3. Scoaterea unui monitor din stoc dupa producator\n";
        cout << "4. Cautarea unui monitor dupa producator\n";
        cout << "5. Ordonare monitoare dupa diagonala\n";
        cout << "6. Inchidere program\n";
        cout << "Optiunea dvs. este ";
        cin >> opt;
        system("CLS");
        switch (opt)
        {
        default:
            cout << "Optiune invalida! Incercati din nou!";
            system("pause");
            break;
        case 1:
            AdaugareM(monitoare);
            break;
        case 2:
            AfisareM(monitoare);
            system("PAUSE");
            break;
        case 3:
            StergereM(monitoare);
            system("pause");
            break;
        case 4:
            CautareM(monitoare);
            break;
        case 5:
            monitoare.sort(Ordonare);
            cout << "Ordonarea a fost efectuata!\n";
            break;
        case 6:
            cout << "Programul se va inchide...\n";
            exit(0);
            break;
        }
    } while (1);
    return 0;
}

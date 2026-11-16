#include <iostream>
#include <string>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <cctype>
#include <windows.h>
using namespace std;

/*
    generator morseovky (umi uz take pismeno 'CH' a apostrof a dalsi znaky)
    jeste trochu poupraveno, do pole pridany jako nahradni znaky, znaky z diakritikou
    tato verze je ze vsech nejpropracovanejsi a proto doporucuju tuto
*/

// funkce pro nastaveni barvy
void nastav_barvu(WORD attr) {
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
}

struct paleta { const char* name; WORD attr;};		        
paleta barva[]={
{ "0: Black", 0 },
{ "1: Blue", FOREGROUND_BLUE },
{ "2: Green", FOREGROUND_GREEN },
{ "3: Cyan", FOREGROUND_GREEN | FOREGROUND_BLUE },
{ "4: Red", FOREGROUND_RED },
{ "5: Magenta", FOREGROUND_RED | FOREGROUND_BLUE },
{ "6: Yellow", FOREGROUND_RED | FOREGROUND_GREEN },
{ "7: White",FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE },
{ "8: Bright Black (Gray)", FOREGROUND_INTENSITY },
{ "9: Bright Blue", FOREGROUND_INTENSITY | FOREGROUND_BLUE },
{ "10: Bright Green", FOREGROUND_INTENSITY | FOREGROUND_GREEN },
{ "11: Bright Cyan", FOREGROUND_INTENSITY | FOREGROUND_GREEN  | FOREGROUND_BLUE },
{ "12: Bright Red", FOREGROUND_INTENSITY | FOREGROUND_RED },
{ "13: Bright Magenta", FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE },
{ "14: Bright Yellow", FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN },
{ "15: Bright White", FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE },
{ "16: puvodni barva konzole", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE },
};

const int FREKVENCE = 750; // ton 750Hz

void vysliSignaly(const string& morseKod, int dotMs) {
nastav_barvu(barva[11].attr);
for (char symbol : morseKod) {
if (symbol == '.') {
cout<<"."<<flush; 
Beep ( FREKVENCE, dotMs);
}
else if (symbol == '-') {
cout<<"-"<<flush;
Beep ( FREKVENCE, dotMs * 3 );
}
// mezera mezi jednotlivymi symboly v ramci jednoho pismene (1 tecka)
this_thread::sleep_for(chrono::milliseconds(dotMs));
}
}

int main(int argc, char* argv[]) {
cout<<"https://github.com/Pavek-Martin-2"<<endl;

// 5 kroku rychlosti, delka jedne tecky v milisekundách (index 1 - 5)
// 1 = 300ms (velmi pomale) az 5 = 50ms (velmi rychle)
const int rychlostiMs[6] = {0, 300, 220, 150, 90, 50};
int urovenRychlosti = 3; // default stredni rychlost, pokud se nezada zadny parametr

// zpracovani parametru argv[1]
if (argc > 1) {
try {
int zadanaUroven = stoi( argv[1] );
if ( zadanaUroven >= 1 && zadanaUroven <= 5 ) {
urovenRychlosti = zadanaUroven;
}else{
cout<<"rychlost musi byt v rozsahu 1 - 5. nastavuju vychozi (3)"<<endl;
}
} catch (...) {
cout<<"neplatny parametr rychlosti. nastavuju vychozi (3)"<<endl;
}
}

int dotMs = rychlostiMs[urovenRychlosti];
cout<<"aktualni rychlost vysilani: ";
cout<<urovenRychlosti<<"/5 (delka tecky: "<<dotMs<<" ms)"<<endl;

const unordered_map<char, string> morseSlovnik = {
{'A', ".-"},   {'B', "-..."}, {'C', "-.-."}, {'D', "-.."},
{'E', "."},    {'F', "..-."}, {'G', "--."},  {'H', "...."},
{'I', ".."},   {'J', ".---"}, {'K', "-.-"},  {'L', ".-.."},
{'M', "--"},   {'N', "-."},   {'O', "---"},  {'P', ".--."},
{'Q', "--.-"}, {'R', ".-."},  {'S', "..."},  {'T', "-"},
{'U', "..-"},  {'V', "...-"}, {'W', ".--"},  {'X', "-..-"},
{'Y', "-.--"}, {'Z', "--.."},
{'0', "-----"},{'1', ".----"},{'2', "..---"},{'3', "...--"},
{'4', "....-"},{'5', "....."},{'6', "-...."},{'7', "--..."},
{'8', "---.."},{'9', "----."},
{'.', ".-.-.-"},{',', "--..--"},{'?', "..--.."},{'!', "-.-.--"},
{':', "---..."},{';', "-.-.-."},{'-', "-....-"},{'_',"..--.."},
{'/',"-..-."}, {'(', "-.--."}, {')', "-.--.-"}, {'"', ".-..-."},
{'@', ".--.-."}, {'&', ".-..."}, {'=', "-...-"}, {'+', ".-.-."},
{'$',"...-..-"},
{'`',"........"}, // # OMYL 8 TECEK, nastaven na klavesu '`' (pod klavesou Esc)
// '`' omyl - vyznam, spletl jsem se, posledni slovo skrtam a posilam ho znovu
{'è',"----"}, // 'è' je jednoznakova nahrada za dvouznakove pismeno 'CH'
{'á',".----."}, // 'á' nahradni znak za apostrof (nejde dat do pole ''')
{' ',""} // toto je mezera mezi slovama (nechat jak je !)
};

/*
  Jednopismenove zkratky v radioprovozu (Mazinarodnim)
  ----------------------------------------------------
  Pri samotnem vysilani opetatori extreme setri cas. Kdyz odvysilaji jedno jedine pismeno
  samostatne, ma to v mezinarodnim radiotelegrafickem provozu tento pevny vyznam:

  K (.-.) - "Prepinam" (Vyzva pro jakoukoliv stanici aby zacala vysilat)
  R (.-.) - "Rozumim" (Potrvzeni, ze zprava byla kompletne a spravne prijata )
  N (-.) - "Ne" (Zaporna obpoved nebo nesouhlas)
  C (-.-.) - "Ano" (Zkratka z italskeho Si nebo spanelskeho Sí pro potvrzeni)
  E (.) - "Chyba" (Pokud se vysila samostatne, casto se opakuje za sebou
  jako serie tecek pro nahlaseni omylu)
  V (...-) - "Zkouska" (Vysila se opakovane pri testovani pristroju a ladeni signalu)
*/

string vstupniSlovo;
string tiskni;
int n; // pro ascii hodnoty znaku jako je apostrof atd.

while (true) {
nastav_barvu(barva[16].attr);
cout<<endl<<"zadej slovo bez diakritiky (nebo 'quit' pro konec): ";
getline(cin, vstupniSlovo);  // precte cely radek (vcetne pripadnych mezer)
		
string kontrolaKonce = vstupniSlovo;
for (char& c : kontrolaKonce){
c = tolower(static_cast<unsigned char>(c));
}
        
// quit ?
if (kontrolaKonce == "quit"){
cout<<"konec"<<endl;
Sleep(3000);
break;
}

int d_vstupniSlovo = vstupniSlovo.length();
for (int aa = 0; aa <= d_vstupniSlovo -1; aa++ ){

tiskni = toupper(vstupniSlovo[aa]); // pro tisk odvysilaneho pismena	

if (vstupniSlovo[aa] == 'è' || vstupniSlovo[aa] == 'á'){
//cout<<"vynechavam nahradni znaky";
vstupniSlovo[aa] = 'ž'; // neco co nejni v poli napø. 'ž'
}

// specialne jenom pro pismeno 'ch' nebo 'CH' (dvouznakove)
// ale take pro 'cH' a 'Ch' :)
if ((((vstupniSlovo[aa] == 'c' && vstupniSlovo[aa+1] == 'h') ||
(vstupniSlovo[aa] == 'C' && vstupniSlovo[aa+1] == 'H') ||
(vstupniSlovo[aa] == 'c' && vstupniSlovo[aa+1] == 'H') ||
(vstupniSlovo[aa] == 'C' && vstupniSlovo[aa+1] == 'h')))) { 	

aa++; // pro 'CH' je nutne [aa] posunout o jeden znak dopredu...
vstupniSlovo[aa] = 'è'; // 'è' je nahradni znak za 'CH' 
// nevadi mu diakritika v poly - morseSlovnik {}
// a ta se nepouziva, cili pouzito jako pro nahradni znaky
tiskni = "CH"; // predefinuje nahradni znak
}

// pro apostrof
n = vstupniSlovo[aa]; // prevod znaku na ascii hodnotu
if (n == 39){ // apostrof ascii hodnota = 39 ( nejde porovnat jako ''')
vstupniSlovo[aa] = 'á'; // 'á' je nahradni znak za ''' (nejde dat do pole ''')
tiskni = "'"; // predefinuje nahradni znak
}

char velkePismeno = toupper(static_cast<unsigned char>(vstupniSlovo[aa]));

if (morseSlovnik.count(velkePismeno)) { // podminka pousti jen znaky  obsazene v poly
        
// funkce ktera vysila
vysliSignaly(morseSlovnik.at(velkePismeno), dotMs);
                
if (velkePismeno != ' '){
nastav_barvu(barva[12].attr);
cout<<tiskni<<" ";
}else{
// mezera mezi slovy ( bilou barvou )
nastav_barvu(barva[16].attr);
cout<<"| ";	// mezera mezi slovama znak svislitko
}

this_thread::sleep_for(chrono::milliseconds(dotMs * 2));
}
}
}
return 0;

//system("pause");
}


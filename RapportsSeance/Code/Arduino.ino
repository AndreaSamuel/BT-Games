#include <SoftwareSerial.h>
#define RX 10
#define TX 11
SoftwareSerial BlueT(RX,TX);

String rep = "";
int mode;

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(9600);
  delay(500);
  Serial.println("~~~~~~~~~~~~~~~~");
  BlueT.begin(9600);
  delay(500);
}



void loop(){ //Menu Principal
//  config();
  delay(25);
  rep = lireBT();
  
  if(rep!=""){
    Serial.print("Commande recue : ");
    Serial.println(rep);
    
    if(rep=="M"){
      Serial.println("Mode Multi");
      mode = 2;      
    }
    else if(rep=="S"){
      Serial.println("Mode Solo");
      mode = 1;
    }
    else if(rep=="1"){ //Lance Reflex
     ReflexCLY(mode);  
    }
    else if(rep=="2"){ //Lance Simon
      SimonCLY(mode);
    }
    else if(rep=="3"){ //Lance Mastermind
      MastermindCLY(mode);
    }
    else if(rep=="4"){ //Lance BlackJack
      BlackJackCLY(mode);
    }
    else{
      Serial.println("Mauvaise commande");
    }
  rep=""; //reset commande
  }
}

String lireBT() {
  int n;
  char cBT;
  String rBT="";
  String rBT2="";
  if (BlueT.available()){ 
    if ((char)BlueT.read()=='X'){
      while (true) {
        cBT = (char)BlueT.read();
        if (cBT=='X') {
          break;
        }
        rBT+=cBT;
      }
    }
  }
  return rBT;
}

void viderBT() {
  while (BlueT.available()) {
  BlueT.read();
  }
}

void ReflexCLY(int mode){
  int Score = 0;
  String pos[4] = {"A","B","C","D"};
  
  for (int i = 0; i<4 ;i++) {
    viderBT();
    
    int r1 = random(4);
    int r2 = random(6);
    int tps = 0;


    Serial.println("~~~~~~~~~~~~~");
    Serial.print("Round : ");
    Serial.print(i+1);
    Serial.println("/4");
    delay(r2*1000);
    Serial.print("La lettre est :");
    Serial.println(pos[r1]);

    delay(100);
    long timer = millis();
    
    while (true) {
      rep = "";
      tps = (millis()-timer);
      rep = lireBT();
      
      if (rep==""){
        if (tps > 7000) {
          Serial.println("Temps écoulé");
          break;
        }
      }
      else if((rep=="A")or(rep=="B")or(rep=="C")or(rep=="D")){
        if (rep == pos[r1]){
            Serial.println("Bonne Réponse !!");
            Serial.print("Temps de réponse :");
            Serial.print(tps);
            Serial.println(" ms");
            Score += 7000 - tps;
            delay(2000);
            break;
        }
        else{
          Serial.println(rep + " est une mauvaise réponse !!");
          Serial.print("Temps de réponse :");
          Serial.print(tps);
          Serial.println(" ms");
          delay(2000);
          break;
        }
      }
      delay(25);
    }
  }
  Serial.println("~~~~~~~~~~~~~");
  Serial.print("Score : ");
  Serial.println(Score);
  Serial.println("~~~~~Fini~~~~~");
  viderBT();
  BlueT.print("1");
}



void SimonCLY(int mode){ //jeuSimon
  bool game = true;
  int j = 0;
  int Tapp = 1000;
  String Couleur[4] = {"rouge","vert","bleue","jaune"};
  String SuiteCouleur[40];

    viderBT();
    
  while (game) {
    j++;
    int i = 0;
    int r = random(4);
    String Cr = Couleur[r];
    SuiteCouleur[j-1]=Cr;
    
    Serial.println("~~~~~~~~~~~~~");
    Serial.print("Round : ");
    Serial.println(j);

    for (i=0; i<j; i++){
      Serial.println(SuiteCouleur[i]);
      delay(Tapp);
      Tapp = Tapp*0.96;
    }
    viderBT();
    
    for (i=0; i<j; i++){
      while (true) {
        delay(25);
        rep = "";
        rep = lireBT();
        Serial.println(rep);
      
        if (rep=="j") {
          if (SuiteCouleur[i]=="jaune") {
            break;          
          }
          else {
            Serial.println("PERDU !!");
            game = false;
            break;
          }
        }
        
        if (rep=="r") {
          if (SuiteCouleur[i]=="rouge") {
            break;          
          }
          else {
            game = false;
            Serial.println("PERDU !!");
            break;
          }        
        }
        
        if (rep=="v") {
          if (SuiteCouleur[i]=="vert") {
            break;          
          }
          else {
            game = false;
            Serial.println("PERDU !!");
            break;
          }        
        }
        
        if (rep=="b") {
          if (SuiteCouleur[i]=="bleue") {
            break;          
          }
          else {
            game = false;
            Serial.println("PERDU !!");
            break;
          }
        }
      }
    
      if (not game) {
        break;
      }
    }
  }
  delay(500);
  Serial.println("~~~~~~~~~~~~~");
  Serial.print("Score : ");
  Serial.println(j-1);
  Serial.println("~~~~~Fini~~~~~");
  viderBT();
  BlueT.print("2");
}



void MastermindCLY(int mode){
  int nbR = 0;
  int nbV = 0;
  int nbB = 0;
  int nbJ = 0;
  int RnbR = 0;
  int RnbV = 0;
  int RnbB = 0;
  int RnbJ = 0;
  int Bcouleur = 0;
  int Bordre = 0;
  int Ess = 1;
  int i = 0;
  
  String couleur[4] = {"rouge","vert","bleue","jaune"};
  String SCouleur[4];
  String RepCouleur[4];

  if (mode==1) {
    for(i=0; i<4; i++) {
      int r1 = random(4);
      SCouleur[i] = couleur[r1];
      if (r1 == 0) {
        nbR++;
      }
      else if (r1 == 1) {
        nbV++;
      }
      else if (r1 == 2) {
        nbB++;
      }
      else {
        nbJ++;
      }
    }
  }
  else {
    Serial.println("Joueur 1 choisissez la suite a deviné");
    viderBT();
    while(i<4){
      delay(25);
      rep = "";
      rep = lireBT();
      
      if (rep == "r") {
        SCouleur[i] = "rouge";
        Serial.print("rouge,");
        nbR++;
        i++;
      }
      else if (rep == "v") {
        SCouleur[i] = "vert";
        Serial.print("vert,");
        nbV++;
        i++;
      }
      else if (rep == "b") {
        SCouleur[i] = "bleue";
        Serial.print("bleue,");
        nbB++;
        i++;
      }
      else if (rep == "j") {
        SCouleur[i] = "jaune";
        Serial.print("jaune,");
        nbJ++;
        i++;
      }
    }
    Serial.println("");
    Serial.println("Joueur 2 a vous de jouer");
  }

  Serial.println("~~~~~~~~~~~~~");
  Serial.println("C'est parti !!");
  while (true) {
    Serial.print("Essaie(s) n°");
    Serial.print(Ess);
    Serial.println(" :");
    i = 0;
    Bcouleur = 0;
    Bordre = 0;
    RnbR = 0;
    RnbV = 0;
    RnbB = 0;
    RnbJ = 0;
    viderBT();
    
    while(i<4){
      delay(25);
      rep = "";
      rep = lireBT();
      
      if (rep == "r") {
        RepCouleur[i] = "rouge";
        Serial.print("rouge,");
        RnbR++;
        i++;
      }
      else if (rep == "v") {
        RepCouleur[i] = "vert";
        Serial.print("vert,");
        RnbV++;
        i++;
      }
      else if (rep == "b") {
        RepCouleur[i] = "bleue";
        Serial.print("bleue,");
        RnbB++;
        i++;
      }
      else if (rep == "j") {
        RepCouleur[i] = "jaune";
        Serial.print("jaune,");
        RnbJ++;
        i++;
      }
    }
    
    Bcouleur = min(RnbR,nbR);
    Bcouleur += min(RnbV,nbV);
    Bcouleur += min(RnbB,nbB);
    Bcouleur += min(RnbJ,nbJ);
    for(i=0;i<4;i++) {
      if(SCouleur[i]==RepCouleur[i]){
        Bcouleur--;
        Bordre++;
      }
    }
    if (Bordre==4) {
      break;
    }
    else {
      Ess++;
    }
    Serial.println("");
    Serial.print("Couleur Mal Placé : ");
    Serial.println(Bcouleur);
    Serial.print("Couleur Bien Placé : ");
    Serial.println(Bordre);
    Serial.println("~~~~~~~~~~~~~");
  }
  Serial.println("");
  Serial.println("~~~~~~~~~~~~~");
  Serial.println("Bravo !!!");
  Serial.print("Essai(s) : ");
  Serial.println(Ess);
  Serial.println("~~~~~Fini~~~~~");
  viderBT();
  BlueT.print("3");
}



void BlackJackCLY(int mode){
  int EtP = 0;
  int CarteTB = 1;
  int CarteTJ = 1;
  int totB = 0;
  int totJ = 0;
  int ASb = 0;
  int ASj = 0;
  String Couleur[4] = {"Pique","Trèfle","Coeur","Carreau"};
  String Valeur[13] = {"AS","2","3","4","5","6","7","8","9","10","Valet","Dame","Roi"};

  Serial.println("~~~~~~~~~~~~~");
  Serial.println("C'est parti !!");

  //Tirage premiere carte Banque
  int r1 = random(4);
  int r2 = random(13);
  if (r2==0) {ASb++;}
  totB += calBJ(r2);
  Serial.print("La Carte de la Banque: ");
  Serial.print(Valeur[r2]);
  Serial.print(" de ");
  Serial.println(Couleur[r1]);
  
  //Tirage premiere carte Joueur
  r1 = random(4);
  r2 = random(13);
  if (r2==0) {ASj++;}
  totJ += calBJ(r2);
  Serial.print("Votre Carte: ");
  Serial.print(Valeur[r2]);
  Serial.print(" de ");
  Serial.println(Couleur[r1]);

  //Tour du Joueur
  Serial.println("~~~~~~~~~~~~~");
  Serial.println("A vous de jouer");
  Serial.print("Total Joueur Actuel: ");
  Serial.println(totJ);
  viderBT();
  while (true) {
    rep = "";
    rep = lireBT();
    
    if (rep=="C"){
      CarteTJ++;
      r1 = random(4);
      r2 = random(13);
      Serial.print("Nouvelle Carte Joueur: ");
      Serial.print(Valeur[r2]);
      Serial.print(" de ");
      Serial.println(Couleur[r1]);
      if (r2==0) {ASj++;}
      totJ += calBJ(r2);
      if (totJ>21){
        if (ASj>0){
          totJ -= 10;
          ASj--;
        }
        else{
          Serial.print("Total Joueur Actuel: ");
          Serial.println(totJ);
          EtP=2;
          delay(10);
          break;
        }
      }
      Serial.print("Total Joueur Actuel: ");
      Serial.println(totJ);
    }
    
    else if (rep=="R") {
      break;
    }
    delay(25);
  }

  //Tour de la Banque
  Serial.println("~~~~~~~~~~~~~");
  Serial.println("A la banque de jouer");
  Serial.print("Total Banque Actuel: ");
  Serial.println(totB);
  while ((totB<17)and(EtP==0)) {
    CarteTB++;
    int r1 = random(4);
    int r2 = random(13);
    Serial.print("Nouvelle Carte Banque: ");
    Serial.print(Valeur[r2]);
    Serial.print(" de ");
    Serial.println(Couleur[r1]);
    if (r2==0) {ASb++;}
    totB += calBJ(r2);
    if (totB>21) {
      if(ASb>0){
        totB -= 10;
        ASb--;
      }
      else{
        EtP = 1;
      }
    }
    Serial.print("Total Banque Actuel: ");
    Serial.println(totB);
  }

    //Calcul Gagnant
    Serial.println("~~~~~~~~~~~~~~");
    if (EtP == 0) {
      if (totJ==totB) {
        Serial.print("Cartes Joueur :");
        Serial.println(CarteTJ);
        Serial.print("Cartes Banque :");
        Serial.println(CarteTB);
        if (CarteTJ<=CarteTB) {
          EtP=1;
        }
        else{
          EtP=2;
        }
      }
      else if (totJ>totB) {
        EtP=1;
      }
      else {
        EtP=2;
      }
    }
    if (EtP == 1) {
      Serial.println("Le joueur remporte la victoire !!");
    }
    else {
      Serial.println("Le joueur a perdu . . .");
    }
    Serial.println("~~~~~Fini~~~~~");
    viderBT();
    BlueT.print("4");
}

int calBJ(int a) {
  int res = 0;
    if (a==0) {
    res += 11;
  }
  else if (a<9) {
    res += a+1;
  }
  else {
    res += 10;
  }
  return res;
}

void config(){
  while (BlueT.available()) {
    Serial.print(char(BlueT.read())); }
  while (Serial.available()) {
    BlueT.write(char(Serial.read())); }
}

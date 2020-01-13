#include <SoftwareSerial.h>
#define RX 10
#define TX 11
SoftwareSerial BlueT(RX,TX);

String rep = "";
int mode;

String couleur[4] = {"rouge","vert","bleue","jaune"};
String Couleur[4] = {"Pique","Trèfle","Coeur","Carreau"};
String Valeur[13] = {"AS","2","3","4","5","6","7","8","9","10","Valet","Dame","Roi"};
int RecBJ[2];

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
      BlueT.print("2");
      Serial.println("Mode Multi");
      mode = 2;      
    }
    else if(rep=="S"){
      BlueT.print("1");
      Serial.println("Mode Solo");
      mode = 1;
    }
    else if(rep=="1"){ //Lance Reflex
     BlueT.print("R");
     ReflexCLI(mode);
    }
    else if(rep=="2"){ //Lance Simon
      BlueT.print("S");
      SimonCLI(mode);
    }
    else if(rep=="3"){ //Lance Mastermind
      BlueT.print("M");
      MastermindCLI(mode);
    }
    else if(rep=="4"){ //Lance BlackJack
      BlueT.print("B");
      BlackJackCLI(mode);
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

void ReflexCLI(int mode){
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
  BlueT.print("F");
}



void SimonCLI(int mode){ //jeuSimon
  bool game = true;
  int j = 0;
  int Tapp = 1000;
  String SuiteCouleur[40];

    viderBT();
    
  while (game) {
    j++;
    int i = 0;
    int r = random(4);
    String Cr = couleur[r];
    SuiteCouleur[j-1]=Cr;
    
    Serial.println("~~~~~~~~~~~~~");
    Serial.print("Round : ");
    Serial.println(j);

    for (i=0; i<j; i++){
      Serial.println(SuiteCouleur[i]);
      delay(Tapp);
      Tapp = Tapp*0.95;
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
  BlueT.print("F");
}



void MastermindCLI(int mode){
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
  BlueT.print("F");
}



void BlackJackCLI(int mode){
  int dejaT = 0;
  int JeuB[3] = {0,0,0};
  int JeuJ1[3] = {0,0,0};
  int JeuJ2[3] = {0,0,0};
  int EtP = 0;

  Serial.println("~~~~~~~~~~~~~");
  Serial.println("C'est parti !!");

  //Tirage premiere carte Banque
  Serial.print("La Carte de la Banque: ");
  tirageC(JeuB[0],JeuB[1]);
  JeuB[0] = RecBJ[0];
  JeuB[1] = RecBJ[1];
  JeuB[2]++;
  
  //Tirage premiere carte Joueur
  Serial.print("Carte du ");
  if (mode==2){Serial.print("Premier ");}
  Serial.print("Joueur: ");
  tirageC(JeuJ1[0],JeuJ1[1]);
  JeuJ1[0] = RecBJ[0];
  JeuJ1[1] = RecBJ[1];
  JeuJ1[2]++;

  //Tirage deuxieme carte Joueur
  if (mode==2) {
    Serial.print("Carte du Deuxième Joueur: ");
    tirageC(JeuJ2[0],JeuJ2[1]);
    JeuJ2[0] = RecBJ[0];
    JeuJ2[1] = RecBJ[1];
    JeuJ2[2]++;
  }

  //Tour du Joueur 1
  Serial.println("~~~~~~~~~~~~~");
  Serial.print("Au tour du ");
  if (mode==2){Serial.print("Premier ");}
  Serial.println("Joueur");
  Serial.print("Total Joueur Actuel: ");
  Serial.println(JeuJ1[0]);
  viderBT();
  while (true) {
    rep = "";
    rep = lireBT();
    
    if (rep=="C"){
      Serial.print("Nouvelle Carte du Joueur: ");
      tirageC(JeuJ1[0],JeuJ1[1]);
      JeuJ1[0] = RecBJ[0];
      JeuJ1[1] = RecBJ[1];
      JeuJ1[2]++;
      Serial.print("Total Joueur Actuel: ");
      Serial.println(JeuJ1[0]);
      if (JeuJ1[0]>21){
        dejaT+=1;
        EtP = 1;
        delay(10);
        break;
      }
    }
    else if (rep=="R") {
        break;
    }
    delay(25);
  }

  //Tour du Joueur 2
  if (mode==2) {
    Serial.println("~~~~~~~~~~~~~");
    Serial.println("Au tour du Deuxième Joueur");
    Serial.print("Total Joueur Actuel: ");
    Serial.println(JeuJ2[0]);
    viderBT();
    while (true) {
      rep = "";
      rep = lireBT();
      
      if (rep=="C"){
        Serial.print("Nouvelle Carte du Joueur: ");
        tirageC(JeuJ2[0],JeuJ2[1]);
        JeuJ2[0] = RecBJ[0];
        JeuJ2[1] = RecBJ[1];
        JeuJ2[2]++;
        Serial.print("Total Joueur Actuel: ");
        Serial.println(JeuJ2[0]);
        if (JeuJ2[0]>21){
            dejaT+=2;
            EtP+=2;
            delay(10);
            break;
          }
        }
        else if (rep=="R") {
          break;
        }
        delay(25);
    }
  }
  
  //Tour de la Banque
  Serial.println("~~~~~~~~~~~~~");
  Serial.println("A la banque de jouer");
  Serial.print("Total Banque Actuel: ");
  Serial.println(JeuB[0]);
  while ( (JeuB[0]<17) and ( ((EtP!=3)and(mode==2)) or ((EtP!=1)and(mode==1)) ) ) {
    Serial.print("Nouvelle Carte Banque: ");
    tirageC(JeuB[0],JeuB[1]);
    JeuB[0] = RecBJ[0];
    JeuB[1] = RecBJ[1];
    JeuB[2]++;
    Serial.print("Total Banque Actuel: ");
    Serial.println(JeuB[0]);
    if (JeuB[0]>21) {
        EtP += 4;
    }
  }

    //Calcul Gagnant
    Serial.println("~~~~~~~~~~~~~~");
    if ((EtP==0)or(EtP==1)or(EtP==2)) {
      if((JeuJ1[0]==JeuB[0])or(JeuJ2[0]==JeuB[0])) {
        Serial.print("Cartes Banque :");
        Serial.println(JeuB[2]);
        if (JeuJ1[0]==JeuB[0]) {
          Serial.print("Cartes Premier Joueur :");
          Serial.println(JeuJ1[2]);
          dejaT+=1;
          if (JeuJ1[2]>=JeuB[2]) {EtP+=1;}
          else {EtP+=4;}
        }
        if (mode == 2) {
          if (JeuJ2[0]==JeuB[0]) {
            Serial.print("Cartes Deuxieme Joueur :");
            Serial.println(JeuJ2[2]);
            dejaT+=2;
            if (JeuJ2[2]>=JeuB[2]) {EtP+=2;}
            else {EtP+=4;}
          }
        }
      }
      
      if ((JeuJ1[0]<JeuB[0])and((dejaT!=1)or(dejaT!=3))) {EtP+=1;}
      else {EtP+=4;}

      if (mode==2) {
        if (JeuJ2[0]<JeuB[0]and((dejaT!=2)or(dejaT!=3))) {EtP+=2;}
        else {EtP+=4;}
      }
    }
    
    if ((EtP == 3)) {
      Serial.println("Les Joueurs ont perdu . . .");
    }
    else if ((EtP == 4)or(EtP == 8)) {
      if (mode==2) {Serial.println("Les deux Joueurs remporte la victoire !!");}
      else {Serial.println("Le Joueur remporte la victoire !!");}
    }
    else if ((EtP==1)or(EtP == 5)) {
      if (mode==2) {Serial.println("Le Deuxième Joueur remporte la victoire !!");}
      else {Serial.println("Le Joueur a perdu . . .");}
    }
    else if (EtP == 6) {
      Serial.print("Le Premier Joueur remporte la victoire !!");
    }
    Serial.println(EtP);
    Serial.println("~~~~~Fini~~~~~");
    viderBT();
    BlueT.print("F");
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

void tirageC(int a, int b) {
  int r1 = random(4);
  int r2 = random(13);
  if (r2==0) {
    b++;
  }
  a += calBJ(r2);
  if (a>21){
    if (b>0) {
      a -= 10;
      b--;
    }
  }
  Serial.print(Valeur[r2]);
  Serial.print(" de ");
  Serial.println(Couleur[r1]);
  RecBJ[0]=a;
  RecBJ[1]=b;
}

void config(){
  while (BlueT.available()) {
    Serial.print(char(BlueT.read())); }
  while (Serial.available()) {
    BlueT.write(char(Serial.read())); }
}
  
